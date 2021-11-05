// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
// ota 참조유튜브 https://www.youtube.com/watch?v=UiAc3yYBsNU
#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

Ticker ticker;
#define TRIGGER_PIN 0 // trigger pin 0(D3)
HTTPClient http;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const String FirmwareVer={"1.2"}; 
String FirmwareVerServer={"0.0"};  // 다운로드 서버 버젼
String nameDownloadFile="";
#define URL_fw_Version "http://i2r.link/download/version.txt"
#define URL_fw_Bin "http://i2r.link/download/"

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


char email[50] = "";
char ipMqtt[40]="";
char userMqtt[40]="";
char passwordMqtt[40]="";

String ipAct="";
char mac[20];  //mac address
String sMac;
int act=0;
int bootMode=0; //0:station  1:AP
const int led = 2;
int counter=0;

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

void readConfig();
void saveConfig();
void factoryDefault();
void handleRoot();
void handleOn();
void GoHome();
void GoHomeWifi();
void handleNotFound();
void handleWifi();
void handleWifiSave();
void handleScan();
void handleConfig();
void handleDownload();
void handleManual();
void displayOled(int no);

void tick()
{

  Serial.println ( WiFi.localIP() );
  counter++;
  //HTML로 보냄
  String json = "{\"temperature\":";
  json += counter;
  json += "}";
  webSocket.broadcastTXT(json.c_str(), json.length());

  //OLED로 표시
  String s;
  s="Temp  "+(String)counter;
  oled.setTextXY(6,7);              // Set cursor position, start of line 0
  oled.putString(s);
}

void displayOled(int no) {
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setFont(font5x7);
  if(no==1) {
    oled.setTextXY(2,6);              
    oled.putString(" Set WiFi");
    oled.setTextXY(4,6);             
    oled.putString(" 192.168.4.1");
  }
  else if(no==2) {
    oled.setTextXY(2,6);
    oled.putString(WiFi.localIP().toString());    
  }
  else if(no==3) {
    oled.setTextXY(2,6);             
    oled.putString(" Download .....    ");
  }
  else if(no==4) {
    GoHome();
    oled.setTextXY(2,6);             
    oled.putString(" Booting .....    ");
  }
  else if(no==5) {
    GoHome();
    oled.setTextXY(2,6);             
    oled.putString(" Reset .....    ");
  }
}


void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(115200);
  //Oled Setup
  Wire.begin(); 
  displayOled(1);

  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  Serial.println(mac);
  
  readConfig();
  Serial.println("===================================");
  Serial.println(ssid);
  if(ssid[0]==0)
    bootWifiAp();
  else {
    bootWifiStation();
    displayOled(2);
  }

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/download", handleDownload);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/config", handleConfig);
  server.on("/manual", handleManual);
  server.on("/scan", handleScan);
  server.onNotFound(handleNotFound);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("HTTP server started");
  ticker.attach(5, tick);  //0.1 초도 가능
  //ticker.detach();

  checkPermVersion();
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  char i2rMac[30];
  sMac="i2r-"+sMac;
  sMac.toCharArray(i2rMac, sMac.length()+1);
  WiFi.softAP(i2rMac, "");
    ipAct=WiFi.softAPIP().toString();
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(ipAct);
  digitalWrite(led, 0); //상태 led로 표시
  // 접속 성공이면 led 5초간 점등 후 소등
  digitalWrite(led, 0);
  delay(5000);
  digitalWrite(led, 1);
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  bootMode=0; //0:station  1:AP
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //공장리셋
    if ( digitalRead(TRIGGER_PIN) == LOW ) 
      factoryDefault();
  }
  ipAct=WiFi.localIP().toString();
  digitalWrite(led, 0);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(ipAct);
  // 접속 성공이면 led 5번 점등
  for(int i=0;i<5;i++) {
    digitalWrite(led, 0);
    delay(500);
    digitalWrite(led, 1);
    delay(500);
  }
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

int checkPermVersion() {
  http.begin(URL_fw_Version);
  delay(100);
  int httpCode = http.GET();
  delay(100);
  if (httpCode == HTTP_CODE_OK)         // if version received
  {
    FirmwareVerServer=http.getString();
    Serial.println("server-"+FirmwareVerServer+"  board-"+FirmwareVer );
    if(FirmwareVerServer.equals(FirmwareVer)) {
      return 1;
    }
    else
      return 0;
  }
  else
  {
    Serial.print("error in downloading version file:");
    Serial.println(httpCode);

  }
  return 0;
}

void download_program(String fileName) {
  Serial.println(fileName);
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);

    String ss;
    //ss=(String)URL_fw_Bin+fileName;
    ss="http://i2r.link/download/"+fileName;
    Serial.println(ss);
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, ss);
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
    // Or:
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");
    
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;
  
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
  
      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }
}

// act=1 basic firmware를 내려받는다.
// act=2 내려받을 파일이름을 전역변수 nameDownloadFile 에 저장
// act=3 nameDownloadFile 파일을 서버에서 내려 받음
void onAct(uint8_t * payload, size_t length) {
  String s;
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  int act = root["act"];
  Serial.println(act);
  if(act==1) {
    displayOled(3);
    //GoHome();
    download_program("down-permwareBasic.bin");
  }
  else if(act==2) {
    const char* nameD = root["value"];
    nameDownloadFile=nameD;
    Serial.println(nameDownloadFile);
  }
  else if(act==3) {
    displayOled(3);
    //GoHome();
    download_program(nameDownloadFile);
  }
    
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    if(payload[0] == '#'){
      for(int i = 0; i < length; i++)
        payload[i]=payload[i+1];
      payload[length]=0;
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      onAct(payload,length);
    }
    else {
      for(int i = 0; i < length; i++)
        Serial.print((char) payload[i]);
      Serial.println();
    }
  }
}


void loop() {
  webSocket.loop();
  server.handleClient();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
