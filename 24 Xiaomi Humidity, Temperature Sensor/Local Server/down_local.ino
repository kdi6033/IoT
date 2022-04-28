// 햄버거 메뉴 https://youtu.be/yPOYlaGloMk
// 웹소켙 https://youtu.be/77qg_aaFGoI   https://youtu.be/TUU0CSBfgos
// SPIFFS https://youtu.be/5wgZIapHPxs  https://youtu.be/FRIgp7JI7IY
// ota 참조유튜브 https://www.youtube.com/watch?v=UiAc3yYBsNU
#include <ArduinoJson.h>
#include "FS.h"
#include <PubSubClient.h>
#include "SPIFFS.h"
#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#define SLEEP_DURATION 20
#define RETRY 3


#define TRIGGER_PIN 15 // trigger pin 15(D3)
#include <BLEDevice.h>
#include <SimpleTimer.h>

#define LYWSD03MMC_ADDR "17:75:bc:11:28:99"

BLEClient* pClient;

static BLEAddress htSensorAddress(LYWSD03MMC_ADDR);

bool connectionSuccessful = false;
int readNum = 0;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

WiFiUDP Udp;
unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets

char ssid[40] = "";
char password[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
char ipMqtt[40]="";
float temp;
float humi;



int type=24; // 999=lora gate way  
const char* outTopic = "/jlee/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/jlee/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";  // setup 함수에서 자동생성
char msg[500];
String sMsg="";
int mqttConnected=0; // 1=연결 0=끊김
char mac[20];  //mac address
String sMac;
String sMacXiaomi;
int bootMode=0; //0:station  1:AP
int nWifi;
unsigned int countTick=0;
unsigned int countMqtt=0;
BLEClient* xiaomiClient;
char* deviceMacAddress;
BLERemoteService* xiaomiService;
BLERemoteCharacteristic* xiaomiCharacteristic;


unsigned long previousMillis = 0;     
const long interval = 1000;  

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

WiFiClient espClient;
PubSubClient client(espClient);
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

RTC_DATA_ATTR int counter = 0;
String sMacFlower;
char jsonBuffer[200];

//char* deviceMacAddress;


void doTick();
void bootWifiAp();
void bootWifiStation();
void factoryDefault();
void handleNotFound();
void handleRoot();
void handleWifi();
void handleWifiSave();
void readConfig();
void saveConfig();
void setup();
void tick();
void tickMeasure();
void udpEvent();
void upWebSocket();
void callback(char* topic, byte* payload, unsigned int length);
void hibernate();
void publishMessage();

void tick()
{
  countTick++;
  if(countTick > 10000)
    countTick=0;
  if((countTick%10)==0)
    tickMeasure();
}
void tickMeasure()
{
  //Serial.println ( WiFi.localIP() );
  if(mqttConnected==1)
    publishMessage();
}


void hibernate() {
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ll);
  Serial.println("Going to sleep now.");
  delay(100);
  esp_deep_sleep_start();
}

void publishMessage() {
  BLEDevice::init("");
  deviceMacAddress = LYWSD03MMC_ADDR;
  sMacXiaomi=String(deviceMacAddress);
  sMacXiaomi.replace(":","");
  BLEAddress xiaomiAddress(deviceMacAddress);
  BLEClient* xiaomiClient = BLEDevice::createClient();
  xiaomiClient->connect(xiaomiAddress);

  // connect data service
  xiaomiService = xiaomiClient->getService(serviceUUID);

  if (xiaomiService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    xiaomiClient->disconnect();
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  xiaomiCharacteristic = xiaomiService->getCharacteristic(charUUID);
  if (xiaomiCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our characteristic");
  const uint8_t v[] = {0x1, 0x0};
  xiaomiCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)v, 2, true);
  xiaomiCharacteristic->registerForNotify(notifyCallback);
  delay(6000);
  std::string value; 
  printf("AFTER\n");
  /*const char *val = value.c_str();
  Serial.print("Hex: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((int)val[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");*/
  /*uint16_t temperature = (val[0] + val[1]*256) / ((float)10.0);
  uint8_t moisture = val[7];
  uint16_t light = val[3] + val[4] *256;
  int conductivity = val[8] + val[9] * 256;
*/
  StaticJsonDocument<200> doc;
  doc["mac"] = sMacXiaomi;
  doc["type"] = String(type);
  doc["ip"] = WiFi.localIP().toString();
  doc["temp"] = temp;
  doc["humi"] = humi;
  serializeJson(doc, jsonBuffer);
  client.publish(outTopic, jsonBuffer);
  Serial.println(jsonBuffer);

  xiaomiClient->disconnect();
  hibernate();
}


// 와이파이 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  DeserializationError error;
  JsonObject root;
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  sMsg="";
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    sMsg+=(char)payload[i];
  }
  Serial.println(sMsg); //출력
}

void setup() {
  Serial.begin(115200);
  //pinMode(TRIGGER_PIN, INPUT_PULLUP);

  
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
  }

  strcpy(ipMqtt, "broker.mqtt-dashboard.com");
  if(bootMode ==0) {
    // mqtt 설정
    printf("MQTT : %s\n", ipMqtt);
    client.setServer(ipMqtt, 1883);
    client.setCallback(callback);
    //Udp.begin(4210);
  }
  else {
    server.on("/", handleRoot);
    server.on("/wifisave", handleWifiSave);
    server.onNotFound(handleNotFound);
    server.begin();
    webSocket.begin();
    Serial.println("HTTP server started");
    nWifi = WiFi.scanNetworks();
    //Serial.println(nWifi);
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
  delay(500); // Without delay I've seen the IP address blank
  Serial.println("AP IP address: 192.168.4.1 ");
}

void bootWifiStation() {
  //referance: https://www.arduino.cc/en/Reference/WiFiStatus
  //WL_NO_SHIELD:255 WL_IDLE_STATUS:0 WL_NO_SSID_AVAIL:1 WL_SCAN_COMPLETED:2
  //WL_CONNECTED:3 WL_CONNECT_FAILED:4 WL_CONNECTION_LOST:5 WL_DISCONNECTED:6
  //WiFi 연결
  bootMode=0; //0:station  1:AP
  Serial.println("Station Mode");
  WiFi.mode(WIFI_STA);
  WiFi.begin((const char*)ssid, (const char*)password);
  //WiFi.begin("KT_GiGA_Mesh_9E8F", "8dh14ge856");
  //printf("%d       %d\n", strcmp(ssid, "KT_GiGA_Mesh_9E8F"), strcmp(password, "8dh14ge856"));

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //공장리셋
    /*if ( digitalRead(TRIGGER_PIN) == LOW ) 
      factoryDefault();
    */
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP().toString());
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  if(ipMqtt[0]==0)
    return;
  Serial.println("reconnect");
  Serial.println(ipMqtt);
  //if(WiFi.status() == WL_DISCONNECTED)
  //  return;
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
      delay(3000);
    }
  //}
}


void loop() {                                             
  //공장리셋
  /*if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
  */
  if(bootMode == 0) {
    doTick();
    //udpEvent();
    //printf("Client Connected? : %d\n", client.connected());
    if (!client.connected()) {
      reconnect();
    }
    if(mqttConnected==1)
      client.loop();
  }
  else if(bootMode==1) {
    server.handleClient();
    webSocket.loop();
  }
}

//1초 마다 실행되는 시간함수
void doTick() {
  //unsigned long currentMillis = millis();
  //if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    //previousMillis = currentMillis;
    delay(500);
    tick();
  //}  
}

void udpEvent() {
  int packetSize = Udp.parsePacket();
  if(!packetSize) 
    printf("No Packet Received from udp\n");
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

static void notifyCallback( BLERemoteCharacteristic* xiaomiCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.println(xiaomiCharacteristic->getUUID().toString().c_str());
  temp = (pData[0] | (pData[1] << 8)) * 0.01; //little endian
  humi = pData[2];
  Serial.printf("temp = %.1f : humidity = %.1f \n", temp, humi);
}
