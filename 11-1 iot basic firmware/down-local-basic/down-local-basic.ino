// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

Ticker ticker;
#define TRIGGER_PIN 0 // trigger pin 0(D3) 2(D4)

char ssid[40] = "";
char password[50] = "";
char softAP_ssid[40] = "i2r-";
const char softAP_password[40] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

char ipMqtt[40]="";
char userMqtt[40]="";
char passwordMqtt[40]="";
String ipAct="";

int act=0;
int bootMode=0; //0:station  1:AP
const int led = 2;
int counter=0;

void tick()
{
  Serial.println ( WiFi.localIP() );
  counter++;
  String json = "{\"temperature\":";
  json += counter;
  json += "}";
  //Serial.println(json); // DEBUGGING
  webSocket.broadcastTXT(json.c_str(), json.length());
}

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.begin(115200);
  //Oled Setup
  Wire.begin(); 
  oled.init();                      // Initialze SSD1306 OLED display
  oled.clearDisplay();              // Clear screen
  oled.setFont(font5x7);
  oled.setTextXY(2,6);              // Set cursor position, start of line 0
  oled.putString(" Set WiFi");
  oled.setTextXY(4,6);              // Set cursor position, start of line 0
  oled.putString(" 192.168.4.1");

  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t mac[6]="";
  WiFi.macAddress(mac);
  sprintf(softAP_ssid,"i2r-%02x%02x%02x%02x%02x%02x%c",mac[5], mac[4], mac[3], mac[2], mac[1], mac[0],0);
  Serial.println(softAP_ssid);
  
  readConfig();
  Serial.println("===================================");
  Serial.println(ssid);
  if(ssid[0]==0)
    bootWifiAp();
  else {
    bootWifiStation();
    oled.init();
    oled.clearDisplay();
    oled.setFont(font5x7);
    oled.setTextXY(2,6);              // Set cursor position, start of line 0
    oled.putString(" Connected    ");
    oled.setTextXY(4,6);
    oled.putString(WiFi.localIP().toString());
  }
  Serial.println("Boot Mode : "+bootMode);

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
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
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

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    for(int i = 0; i < length; i++)
      Serial.print((char) payload[i]);
    Serial.println();
  }
}

void loop() {
  webSocket.loop();
  server.handleClient();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
