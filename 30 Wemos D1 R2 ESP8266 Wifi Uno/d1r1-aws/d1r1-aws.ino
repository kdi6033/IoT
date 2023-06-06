// 온도 습도 추가
// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
// ota 참조유튜브 https://www.youtube.com/watch?v=UiAc3yYBsNU

#include <ArduinoJson.h>
#include "credentials.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "FS.h"
#include <PubSubClient.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h>
#include "DHT.h"

#define TRIGGER_PIN D3// trigger pin 0(D3)
#define SW1 D2
#define SW2 D3
#define LED1 D13
#define LED2 D12
#define DHTPIN D4     // 센서를 연결할 디지털 핀
#define DHTTYPE DHT11   // DHT 11 센서를 사용

DHT dht(DHTPIN, DHTTYPE);

HTTPClient http;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

int type=30; // 기기 인식번호 -> display에 사용

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

const char *clientName = "*******";
const char *host = "a8i4lgiqa43pw-ats.iot.us-west-2.amazonaws.com"; // AWS IoT Core 주소
const char* outTopic = "/i2r/outTopic"; 
const char* inTopic = "/i2r/inTopic"; 

int mqttConnected=0; // 1=연결 0=끊김
char msg[100];
String ipAct="";
String sMac;
int act=0;
int bootMode=0; //0:station  1:AP
int Out[8]={0},In[10]={0};  // plc 입력과 출력 저장 
float humidity=0,temperature=0;

unsigned long previousMillis = 0;     
const long interval = 5000;  

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

void factoryDefault();
void GoHome();
void GoHomeWifi();
void handleDownload();
void handleManual();
void handleNotFound();
void handleRoot();
void handleOn();
void handleWifi();
void handleWifiSave();
void readConfig();
void saveConfig();
void tick();
void upWebSocket();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setClock();
void tickMqtt();
void outPlc();


// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String s;
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  const char* macIn = root["mac"];
  Serial.println(sMac);
  Serial.println(macIn);
  if( sMac.equals(String(macIn))) {
    int outNo = root["outNo"];
    int value = root["value"];
    Out[outNo] = value;
    outPlc();
  }
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void tickMqtt()
{ 
  if (!client.connected()) {
    reconnect();
  }
  if(mqttConnected != 1)
    return;

  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  String json;
  root["mac"] = sMac;
  root["type"] = type;
  root["in0"] = In[0];
  root["in1"] = In[1];
  root["h"] = String(humidity);
  root["t"] = String(temperature);
  root["ip"] = WiFi.localIP().toString();
  serializeJson(root, msg);
  client.publish(outTopic, msg);
  Serial.println(msg);  
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

void upWebSocket() {
  //HTML로 보냄
  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  String json;
  root["in0"] = In[0];
  root["in1"] = In[1];
  root["out0"] = Out[0];
  root["out1"] = Out[1];
  root["h"] = String(humidity);
  root["t"] = String(temperature);
  serializeJson(root, json);
  webSocket.broadcastTXT(json.c_str(), json.length());
  //Serial.println(json);  
}

void setup() {

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(115200);
  dht.begin();

  //이름 자동으로 생성
  uint8_t chipid[6]="";
  WiFi.macAddress(chipid);
  char* cChipID = new char[13];
  sprintf(cChipID,"%02x%02x%02x%02x%02x%02x%c",chipid[5], chipid[4], chipid[3], chipid[2], chipid[1], chipid[0],0);
  sMac=String(cChipID);
  clientName = sMac.c_str();
  Serial.println("");
  Serial.println(clientName);
  
  readConfig();
  if(ssid[0]==0)
    bootWifiAp();
  else {
    bootWifiStation();
  }

  server.on("/", handleRoot);
  server.on("/download", handleDownload);
  server.on("/wifi", handleWifi);
  server.on("/wifisave", handleWifiSave);
  server.on("/manual", handleManual);
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
  sMac.toCharArray(i2rMac, sMac.length()+1);
  WiFi.softAP(i2rMac, "");
    ipAct=WiFi.softAPIP().toString();
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP Name: ");
  Serial.println(sMac);
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
//웹페이지에서 출력버튼을 실행한다.
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  if(type == WStype_TEXT){
    for(int i = 0; i < length; i++)
      Serial.print((char) payload[i]);
    Serial.println();
    onAct(payload,length);
  }
}

void loop() {
  webSocket.loop();
  server.handleClient();
  doTick();

  if(bootMode !=1) {
    if(mqttConnected==1)
      client.loop();
  }

  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();

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

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    In[0] = (int)!digitalRead(SW1);
    In[1] = (int)!digitalRead(SW2);
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    // 값이 유효한지 확인
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("센서에서 읽기를 실패하였습니다!");
      return;
    }
    /*
    Serial.print("습도: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("온도: ");
    Serial.print(temperature);
    Serial.println(" *C");
    */
    upWebSocket();
    tickMqtt();
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
  int no=0,value=0;
  Serial.println(act);
  if(act==2) {  //plc 출력
    no=root["no"];
    value=root["value"];
    Out[no]=value;
    outPlc();
    upWebSocket();
  }
}

void outPlc() {
  digitalWrite(LED1, Out[0]);
  digitalWrite(LED2, Out[1]);
}

