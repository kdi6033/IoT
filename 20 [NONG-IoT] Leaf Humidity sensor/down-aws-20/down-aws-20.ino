// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
// ota 참조유튜브 https://www.youtube.com/watch?v=UiAc3yYBsNU
#include "FS.h"
#include "CRC.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "credentials.h"

SoftwareSerial mySerial(D7, D4); // RX, 
#define TRIGGER_PIN 0 // trigger pin 0(D3)
HTTPClient http;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

int type=20; // 기기 인식번호 -> display에 사용
#define URL_fw_Bin "http://i2r.link/download/"

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

char email[50] = "";
unsigned int countTick=0;
unsigned int countMqtt=0;
unsigned int countMeasure=0;

const char *clientName = "";          // 사물 이름 (thing ID) 자동생성
const char *host = "***-ats.iot.us-west-2.amazonaws.com"; // AWS IoT Core 주소
const char* outTopic = "/i2r/outTopic"; 
const char* inTopic = "/i2r/inTopic"; 

char msg[100];
int mqttConnected=0; // 1=연결 0=끊김

String ipAct="";
char mac[20];  //mac address
String sMac="",sEmail="";
int act=0;
int bootMode=0; //0:station  1:AP
int saveValue=0;
String inputString = "";         // 받은 문자열
int timeMqtt=5;
float hum=0.,temp=0.;

unsigned long previousMillis = 0;     
const long interval = 1000;  

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
void crd16Rtu();
void serialEvent();
void setClock();
void tick();
void tickMeasure();
void tickMqtt();
void upWebSocket();

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void tick() {
  countTick++;
  if(countTick > 10000)
    countTick=0;

  if((countTick%2)==0)
    tickMeasure();

  if((countTick%timeMqtt)==0) {
    countMqtt++;
    tickMqtt();
  }
}

void tickMeasure() {
  Serial.println ( WiFi.localIP() );
  //act=0;
  upWebSocket();
  crd16Rtu();

  //OLED로 표시
  String s;
  s="HUM  "+(String)hum;
  oled.setTextXY(4,7);              // Set cursor position, start of line 0
  oled.putString(s);
  s="Temp  "+(String)temp;
  oled.setTextXY(5,7);              // Set cursor position, start of line 0
  oled.putString(s);
}

void upWebSocket() {
  //HTML로 보냄
  String json = "{\"hum\":"; json += hum;
  json += ",\"temp\":"; json += temp;
  json += "}";
  webSocket.broadcastTXT(json.c_str(), json.length());
  //Serial.println(json); 
}

void tickMqtt() {
  if (!client.connected()) {
    reconnect();
  }
  if(mqttConnected != 1)
    return;
  
  String json;
  //MQTT로 보냄
  json = "{";
  json += "\"email\":\""; json += sEmail;  json += "\"";
  //json += "\"email\":\""; json += "kdi6033@gmail.com";  json += "\"";
  json += ",\"mac\":\""; json += sMac;  json += "\"";
  json += ",\"ip\":\""; json += WiFi.localIP().toString();  json += "\"";
  json += ",\"type\":"; json += String(type);
  json += ",\"hum\":"; json += String(hum);
  json += ",\"temp\":"; json += String(temp);
  json += "}";
  json.toCharArray(msg, json.length()+1);
  Serial.println(msg);
  client.publish(outTopic, msg);
}

void displayOled(int no) {
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setFont(font5x7);
  if(no==1) {
    oled.setTextXY(2,6);              
    oled.putString(" Set WiFi");
    oled.setTextXY(6,6);             
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
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  //Oled Setup
  Wire.begin(); 
  displayOled(1);

  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  clientName=mac;
  Serial.println(clientName);
  
  readConfig();
  if(ssid[0]==0)
    bootWifiAp();
  else {
    bootWifiStation();
    displayOled(2);
  }

  server.on("/", handleRoot);
  server.on("/control", handleControl);
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

  if(bootMode !=1) {
    // mqtt 설정
    wifiClient.setTrustAnchors(&ca);
    wifiClient.setClientRSACert(&cert, &key);
    Serial.println("Certifications and key are set");
  
    setClock();
    //client.setServer(host, 8883);
    client.setCallback(callback);
    delay(3000);
    reconnect();
  }
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  char i2rMac[30];
  sMac="i2r-"+sMac;
  sMac.toCharArray(i2rMac, sMac.length());
  WiFi.softAP(i2rMac, "");
    ipAct=WiFi.softAPIP().toString();
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(ipAct);
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
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(ipAct);
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
    Serial.println("=======================================");
    download_program("down-permwareBasic.bin");
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

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  if(WiFi.status() != WL_CONNECTED)
    return;
  // Loop until we're reconnected
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, "Reconnected");
      // ... and resubscribe
      client.subscribe(inTopic);
      mqttConnected=1;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      mqttConnected=0;
      // Wait 5 seconds before retrying
      delay(5000);
    }
  //}
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void loop() {
  webSocket.loop();
  server.handleClient();
  doTick();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();

  if(bootMode !=1) {
    serialEvent();
    if(mqttConnected==1)
      client.loop();
  }
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    tick();
  }  
}

void serialEvent() {
  if(mySerial.available() == false)
    return;
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    //Serial.print(inChar,HEX);
    // add it to the inputString:
    inputString += inChar;
  }
  //Serial.println("");
  if(inputString.length() >= 9) {
    String ss="";
    ss=((float)inputString.charAt(3)*255+(float)inputString.charAt(4))/10;
    hum=ss.toFloat();
    Serial.print("HUM : "); Serial.println(hum);
    ss=((float)inputString.charAt(5)*255+(float)inputString.charAt(6))/100-20;
    temp=ss.toFloat();
    Serial.print("온도 : "); Serial.println(temp);
    inputString="";
    Serial.println("");

  }
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0xff,0x03,0x00,0x00,0x00,0x02,0xD1,0xD5};  
  String s;
  int si,sj,len;

  len=6;
  
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;

  for(int i=0;i<len+2;i++) {
    mySerial.print(str[i]);
    //Serial.println((int)str[i]);
  }
}
