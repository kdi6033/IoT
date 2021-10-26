#include <ESP32httpUpdate.h> //WiFi WiFiClient WiFiUdp HTTPClient FS.h 포함 -펌웨어 업데이트할때 사용
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <PubSubClient.h>
#include "heltec.h" 
#include <WiFi.h>

#define BAND    923E6
#define TRIGGER_PIN 13 // trigger pin 13

HTTPClient http;
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
WiFiUDP Udp;

unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets

//LoRa
String rssi = "RSSI --";
String packSize = "--";
String packet ;

int type=13; // 기기 인식번호 -> display에 사용
String nameDownloadFile="";

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


char email[50] = "";
char ipMqtt[40]="";
unsigned int countTick=0;
unsigned int countMqtt=0;
unsigned int countMeasure=0;
unsigned int countwifi=0;

const char* outTopic = "/i2r/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/i2r/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
char msg[100];
int mqttConnected=0; // 1=연결 0=끊김

String ipAct="";
char mac[20];  //mac address
String sMac;
int act=0,outPlc=0;
int bootMode=0; //0:station  1:AP
int counter=0;
String inputString = "";         // 받은 문자열
int timeMqtt=5;

// String으로 보내는거 생각
int Out[8]={0},In[10]={0};  // plc 입력과 출력 저장 
int noOut=0,valueOut=0;
int orderCurtain=0,orderCurtainPre=0; //0=stop, 1=up, 2=down  plc입력
int orderCurtainWeb=0,orderCurtainWebPre=0; //0=stop, 1=up, 2=down  cpu의 web
String orderCurtainNow, saveInt="";
int OutLed[3]={0};
//STRING CHIP ID
String chip;

unsigned long previousMillis = 0;     
const long interval = 1000;  

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

WiFiClient espClient;
PubSubClient client(espClient);

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
void upWebSocket();

void tick()
{
  countTick++;
  if(countTick > 10000)
    countTick=0;  
  if((countTick%3)==0) {
  Serial.println ( WiFi.localIP() ); 
    tickMeasure();
  }
  if((countTick%timeMqtt)==0) {
    //Serial.println("$$$$$$$$$$$$$$$$$$$$$$$");
    countMqtt++;
    tickMqtt();
  }
}

void LoRaData()
{
  Heltec.display->clear(); 
  Heltec.display->drawString(0, 0, WiFi.localIP().toString());
  Heltec.display->drawString(0, 8, "=================");
  Heltec.display->drawString(0, 15, "Sender: "+String(chip));  
    Heltec.display->drawString(0, 24, "0:stop | 1:up | 2:down");
    Heltec.display->drawString(0, 33, "State: "+String(orderCurtainWeb));
    String sIn="";
    for(int i=0; i<7; i++) sIn +=String(In[i])+String(" ");
    Heltec.display->drawString(0, 40, "=================");
    Heltec.display->drawString(0, 49, "In: "+String(sIn)); // 정지 올림 내림 표시 up... down....

  Heltec.display->display();  
}
void tickMeasure()
{
  upWebSocket();
  sendData();

}
// 정리
void sendData() {
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(orderCurtainWeb);
  LoRa.print("|"); 
  LoRa.endPacket();
}
void upWebSocket() {
  //HTML로 보냄
  String json = "{\"in0\":"; json += In[0];
  json += ",\"in1\":"; json += In[1];
  json += ",\"in2\":"; json += In[2];
  json += ",\"in3\":"; json += In[3];
  json += ",\"in4\":"; json += In[4];
  json += ",\"in5\":"; json += In[5];
  json += ",\"in6\":"; json += In[6];
  json += ",\"in7\":"; json += In[7];

  json += ",\"out0\":"; json += OutLed[0];
  json += ",\"out1\":"; json += OutLed[1];
  json += ",\"out2\":"; json += OutLed[2];
  json += "}";
  webSocket.broadcastTXT(json.c_str(), json.length());
  //Serial.println(json);  
}

void tickMqtt()
{ 
  if (!client.connected()) {
    reconnect();
  }
  if(mqttConnected != 1)
    return;
  String json;
  //MQTT로 보냄
  json = "{";
  json += "\"mac\":\""; json += chip;  json += "\"";
  json += ",\"ip\":\""; json += WiFi.localIP().toString();  json += "\"";
  json += ",\"type\":"; json += type;
  json += ",\"state\":"; json += orderCurtainNow;
  json += "}";
  json.toCharArray(msg, json.length()+1);
  Serial.println(msg);
  client.publish(outTopic, msg);
}


void displayOled(int no) {
  DisplaySetup();
  Heltec.display->clear();            
  if(no==1) {
    Heltec.display->drawString(0, 0, " Set WiFi");              
    Heltec.display->drawString(0, 20, " 192.168.4.1");
  }
  else if(no==2) {
    Heltec.display->drawString(0, 0, WiFi.localIP().toString());
  }
  else if(no==3) {
    Heltec.display->drawString(0, 0, " Download .....    "); 
  }
  else if(no==4) {
    GoHome();
    Heltec.display->drawString(0, 0, " Booting .....    ");    
  }
  else if(no==5) {
    GoHome();
    Heltec.display->drawString(0, 0, " Reset .....    ");     
  }
  Heltec.display->display(); 
}

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);  
  pinMode(TRIGGER_PIN, INPUT_PULLUP);  
  Serial.begin(9600);
  displayOled(1);
 
  drawFontFaceDemo(); //ip 주소 잠깐 등장
  LoRa.receive();
  
  Serial.println("mac address");
  //이름 자동으로 생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  clientName=mac;
  Serial.println(mac);
  
  readConfig();
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
  
  if(bootMode !=1) {
    // mqtt 설정
    client.setServer(ipMqtt, 1883);
    client.setCallback(callback);
  }
  Udp.begin(localUdpPort);
}

void bootWifiAp() {
  bootMode=1; //0:station  1:AP
  /* Soft AP network parameters */
  Serial.println("AP Mode");
  WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(apIP, apIP, netMsk);
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
    countwifi++;
    //wifi 연결안될시 재부팅
    if(countwifi==4){
    countwifi=0;   
    ESP.restart();
    }
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
  if (WiFi.status() != WL_CONNECT_FAILED) {
    
    String ss;
    ss="http://i2r.link/download/"+fileName;
    Serial.println(ss);
        t_httpUpdate_return ret = ESPhttpUpdate.update("ss");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
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

void reconnect() {
  if(ipMqtt[0]==0)
    return;
  Serial.println("reconnect");
  Serial.println(ipMqtt);
  //if(WiFi.status() == WL_DISCONNECTED)
  //  return;
  Serial.println("====================================");
  Serial.println(countMqtt);
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
      Serial.println(" try again in 180 seconds");
      mqttConnected=0;
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  //}
}

void DisplaySetup() {
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}
void drawFontFaceDemo() {
    Heltec.display->clear();
    String s;
    if(WiFi.status()!=WL_DISCONNECTED)
      s="ip "+WiFi.localIP().toString();
    else
      s="wifi disconnected";
    Heltec.display->drawString(0, 0, s);
    Heltec.display->drawString(0, 20, packet);
    Heltec.display->display();
}

// 센서값 string값으로 받음
void onReceive(int packetSize) 
{
  if (packetSize == 0) return;          // 받은값이 없다면 아무것도 X
  if (packetSize) {     
      //read packet
      while (LoRa.available()) {
      chip = LoRa.readStringUntil('|');  
      orderCurtainNow = LoRa.readStringUntil('|');
      for(int i=0; i<=7; i++){
      saveInt = LoRa.readStringUntil('|');
      In[i] = saveInt.toInt();
      }      
    }

  }  
  
  packet.toCharArray(msg, packet.length()+1);
  client.publish(outTopic, msg);
}

void loop() {
    
  onReceive(LoRa.parsePacket());
  LoRaData();    
  webSocket.loop();
  server.handleClient();
  doTick();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();

  if(bootMode !=1) {
    udpEvent();
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
void udpEvent() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if(len > 0) {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    deserializeJson(doc,incomingPacket);
    root = doc.as<JsonObject>();
    String mqttIp = root["mqttIp"];
    mqttIp.toCharArray(ipMqtt, mqttIp.length()+1);
    Serial.println(ipMqtt);
    saveConfig();
  }  
}

// act=0 plc 입력을 읽어온다.
// act=1 basic firmware를 내려받는다.
// act=2 plc 출력 실행
// act=3 
void onAct(uint8_t * payload, size_t length) {
  String s;
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  act = root["act"];
  //int no=0,value=0;
  Serial.println(act);
  if(act==1) {
    displayOled(3);
    download_program("down-permwareBasic.bin");
  }
  if(act==2) {  //plc 출력
    noOut=root["no"];
    valueOut=root["value"];
    
    if(noOut==0) { //정지
      orderCurtainWeb = 0;
    }
    else if(noOut==1) {  // 올림
      orderCurtainWeb = 1;
    }
    if(noOut==2) {  // 내림
      orderCurtainWeb = 2;
    }
    // 기존에 눌려있던 버튼이 꺼지고 누른 버튼 색깔만 켜짐
    OutLed[0]=0;OutLed[1]=0;OutLed[2]=0;
    OutLed[noOut]=1;
    upWebSocket();
  }
}
