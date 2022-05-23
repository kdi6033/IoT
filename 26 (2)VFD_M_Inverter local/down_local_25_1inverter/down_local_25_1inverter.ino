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
#include <Ticker.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <WiFiUdp.h>

SoftwareSerial mySerial(D7, D5); // RX, TX 
//송신은 Serial1 수신은 mySerial로 하기 때문에 mySerial의 TX는 필요없음(임의로 D5지정) 
#define TRIGGER_PIN 0 // trigger pin 0(D3)
HTTPClient http;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
WiFiUDP Udp;

unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets

int type=25; // 기기 인식번호 -> display에 사용 6= LS PLC XEC-DR14E
#define URL_fw_Bin "http://i2r.link/download/"

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);


char email[50] = "";
char ipMqtt[40]="";
unsigned int countTick=0;
unsigned int countMqtt=0;
unsigned int countMeasure=0;

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
String inputString = "";    // 받은 문자열
int Out[8]={0},In[10]={0};  // plc 입력과 출력 저장 
String sIn="",sInPre="";  // 입력값이 달라질 때만 mqtt로 송신
int noOut=0,valueOut=0; //mqtt로 수신된 plc출력 명령 noOut=출력핀번호 valueOut=출력값

char str[24];
int si,sj,len=6,dir=0,vel=0,predir,prevel;

unsigned long previousMillis = 0;     
const long interval = 1000;  

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

WiFiClient espClient;
PubSubClient client(espClient);

void crd16Rtu();
void displayOled(int no);
void factoryDefault();
void GoHome();
void GoHomeWifi();
void handleConfig();
void handleDownload();
void handleManual();
void handleNotFound();
void handleRoot();
void handleScan();
void handleWifi();
void handleWifiSave();
void readConfig();
void saveConfig();
void serialEvent();
void tick();
void tickMeasure();
void tickMqtt();
void upWebSocket();
void velocity(int vel);
void Direction(int dir);

void tick()
{ 
  countTick++;
  if(countTick > 10000)
    countTick=0;
  //if((countTick%5)==0)
  //tickMeasure();
    tickMqtt();
  //if((countTick%3)==0)
  //  tickMqtt();
}

void tickMeasure()
{
  //Serial.println ( WiFi.localIP() );
}

void upWebSocket() {
  //HTML로 보냄
  String json = "{\"dir\":"; json += dir;
         json += ",\"vel\":"; json += vel;
         json += "}";
         webSocket.broadcastTXT(json.c_str(), json.length());
}

void tickMqtt()
{ 
  if(mqttConnected != 1)
    return;
  String json;
  //MQTT로 보냄
  json = "{";
  json += "\"mac\":\""; json += sMac;  json += "\"";
  json += ",\"ip\":\""; json += WiFi.localIP().toString();  json += "\"";
  json += ",\"type\":"; json += type;
  json += ",\"dir\":"; json += dir;
  json += ",\"vel\":"; json += vel;
  json += "}";
  json.toCharArray(msg, json.length()+1);
  Serial.println(msg);
  client.publish(outTopic, msg);
  countMqtt++;
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
  Serial.begin(9600,SERIAL_8N2);
  Serial1.begin(9600,SERIAL_8N2);
  mySerial.begin(9600);
  //Oled Setup
  //Wire.begin(); 
  //displayOled(1);

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

  //ticker.attach(1, tick);  //0.1 초도 가능
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
  WiFi.softAPConfig(apIP, apIP, netMsk);
  char i2rMac[30];
  sMac="i2r-"+sMac;
  sMac.toCharArray(i2rMac, sMac.length()+1);
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

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  String macIn = root["mac"];
  while(macIn==sMac){
      if(!root["dir"]){         //vel만 들어왔을 때
        dir = predir;
        Direction(dir);         //모터방향 함수
        delay(100);
        vel = root["vel"];
        velocity(vel);          //주파수 함수
        prevel=vel;             //들어온 vel값을 prevel에 저장
        break;
      }
      else if(!root["vel"]){    //dir만 들어왔을 때
        dir = root["dir"];
        Direction(dir);
        predir=dir;             //들어온 dir값을 predir에 저장
        delay(100);
        vel = prevel;
        velocity(vel);
        break;
      }
      else{                     //vel,dir둘 다 들어왔을 때 ( node-red에서 타임스탬프로 신호줄 때 )
        dir = root["dir"];
        Direction(dir);
        predir=dir;
        delay(100);
        vel = root["vel"];
        velocity(vel);
        prevel=vel;
        break;
      }
    }
}

// mqtt 통신에 지속적으로 접속한다.
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

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  webSocket.loop();
  server.handleClient();
  doTick();

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();

  if(bootMode !=1) {
    serialEvent();
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
}

//웹페이지에서 출력버튼을 실행한다.
// act=1 basic firmware를 내려받는다.
// act=2 plc 출력 실행
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    for(int i = 0; i < length; i++)
      Serial.print((char) payload[i]);
    Serial.println();
  }
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  if(!root["dir"]){         //vel만 들어왔을 때
        dir = predir;
        Direction(dir);         //모터방향 함수
        delay(100);
        vel = root["vel"];
        velocity(vel);          //주파수 함수
        prevel=vel;             //들어온 vel값을 prevel에 저장
        upWebSocket();
      }
      else if(!root["vel"]){    //dir만 들어왔을 때
        dir = root["dir"];
        Direction(dir);
        predir=dir;             //들어온 dir값을 predir에 저장
        delay(100);
        vel = prevel;
        velocity(vel);
        upWebSocket();
      }
  //act = root["act"];
  //Serial.println(act);
  /*if(act==1) {
    displayOled(3);
    download_program("down-permwareBasic.bin");
  }*/
  /*if(act==2) {  //plc 출력
    noOut=root["no"];
    valueOut=root["value"];
    //Serial.println(no);
    //Serial.println(value);
    //Serial.println(act);
    outPlc=1;
    Out[noOut]=valueOut;
    upWebSocket();
  }*/
}


void Direction(int dir) {
  /*str[0]=0x01; str[1]=0x06; str[2]=0x20; str[3]=0x00; str[4]=0x00; str[5]=0x12;
  str[6]=0x02;str[7]=0x07;*/
  str[0]=0x02; str[1]=0x06; str[2]=0x20; str[3]=0x00; str[4]=0x00; str[5]=0x12;
    str[6]=0x02;str[7]=0x34;
  if(dir==0)     //정지
    str[5]=0x01;
  else if(dir==1)//정회전
    str[5]=0x12;
  else if(dir==2)//역회전
    str[5]=0x22;
  crd16Rtu();
}

void velocity(int vel) {
  /*str[0]=0x01; str[1]=0x06; str[2]=0x20; str[3]=0x01; str[4]=0x17; str[5]=0x00;
  str[6]=0xDC; str[7]=0x3A;*/
  str[0]=0x02; str[1]=0x06; str[2]=0x20; str[3]=0x01; str[4]=0x17; str[5]=0x00;
  //str[6]=0xDC; str[7]=0x09;
    uint8_t * data = (uint8_t *) &str[0];
  str[5]=vel&0xff;
  int sj=vel>>8;
  str[4]=sj&0xff;
  crd16Rtu();
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  //len=6;
  inputString = "";
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;
  
  for(int i=0;i<len+2;i++) {
    Serial1.print(str[i]);
  }
}
