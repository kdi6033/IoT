//https://aws.amazon.com/ko/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/

#include <pgmspace.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include <WebServer.h>
#include "BLEDevice.h"
#include "config.h"

#define TRIGGER_PIN 15 // trigger pin GPIO15 (D15)

// AWS
const char AWS_IOT_ENDPOINT[] = "***-ats.iot.us-east-1.amazonaws.com";
WiFiClientSecure net = WiFiClientSecure();
//MQTTClient client = MQTTClient(256);
MQTTClient client = MQTTClient(10000);

char ssid[40] = "";
char password[50] = "";
char email[50] = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
String ipAct="";

int type=23; // 999=lora gate way  
const char* outTopic = "/i2r/outTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/i2r/inTopic"; // 이름이 중복되지 않게 설정 기록
char clientName[30] = {0};  // setup 함수에서 자동생성
char msg[500];
String sMac="",sEmail="",sMsg="";
String linkMsg=""; //1분마다 Client 접속을 위한 msg 보냄
int mqttConnected=0; // 1=연결 0=끊김
char mac[20];  //mac address
int bootMode=0; //0:station  1:AP
int nWifi;

unsigned long previousMillis = 0;     
const long interval = 1000; 

WebServer server(80);

char* FLORA_DEVICES[] = { "C4:7C:8D:6D:E8:02", };//첫번째
#define SLEEP_DURATION 5
#define RETRY 3
RTC_DATA_ATTR int counter = 0;
RTC_DATA_ATTR char mqttBuffer[100];
String sMacBlue="";
int battery=0;

// device count
static int deviceCount = sizeof FLORA_DEVICES / sizeof FLORA_DEVICES[0];

// the remote service we wish to connect to
static BLEUUID serviceUUID("00001204-0000-1000-8000-00805f9b34fb");

// the characteristic of the remote service we are interested in
static BLEUUID uuid_version_battery("00001a02-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_sensor_data("00001a01-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_write_mode("00001a00-0000-1000-8000-00805f9b34fb");

TaskHandle_t hibernateTaskHandle = NULL; 

BLEClient* floraClient;
char* deviceMacAddress;
BLERemoteService* floraService;
BLERemoteCharacteristic* floraCharacteristic;


void connectAWS();
void messageHandler(String &topic, String &payload);
void publishMessage();
void setup();
void factoryDefault();//
void handleRoot();//
void handleWifiSave();//
void readConfig();//
void saveConfig();//
String sensWifi(int in);//
void setup();
void readXaiomi();


void hibernate() {
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ll);
  Serial.println("Going to sleep now.");
  delay(100);
  esp_deep_sleep_start();
}

void readXaiomi() {
  Serial.print(counter++); Serial.print("   ");
  BLEDevice::init("");
  deviceMacAddress = FLORA_DEVICES[0];
  sMacBlue=String(deviceMacAddress);
  sMacBlue.replace(":","");
  BLEAddress floraAddress(deviceMacAddress);
  BLEClient* floraClient = BLEDevice::createClient();
  floraClient->connect(floraAddress);

  // connect data service
  floraService = floraClient->getService(serviceUUID);

  floraCharacteristic = floraService->getCharacteristic(uuid_write_mode);
  // Real-time data read request
  uint8_t buf[2] = {0xA0, 0x1F};
  floraCharacteristic->writeValue(buf, 2, true);
 
  floraCharacteristic = floraService->getCharacteristic(uuid_sensor_data);
  std::string value; 
  value = floraCharacteristic->readValue();
  const char *val = value.c_str();
  Serial.print("Hex: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((int)val[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
  uint16_t temperature = (val[0] + val[1]*256) / ((float)10.0);
  uint8_t moisture = val[7];
  uint16_t light = val[3] + val[4] *256;
  int conductivity = val[8] + val[9] * 256;

  floraCharacteristic = floraService->getCharacteristic(uuid_version_battery);
  std::string value2;
  value2 = floraCharacteristic->readValue();
  const char *val2 = value2.c_str();
  int battery = val2[0];

  StaticJsonDocument<200> doc;
  doc["mac"] = sMacBlue;
  doc["type"] = type;
  doc["temp"] = temperature;
  doc["mois"] = moisture; 
  doc["light"] = light; 
  doc["condu"] = conductivity;
  doc["batt"] = battery;
  serializeJson(doc, mqttBuffer);
  //client.publish(outTopic, mqttBuffer);
  Serial.println(mqttBuffer);

  floraClient->disconnect();
  // go to sleep now
  hibernate();
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

void connectAWS()
{
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");
  reconnect();
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  if(WiFi.status() != WL_CONNECTED)
    return;
  // Loop until we're reconnected
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect(clientName)) {
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, "Reconnected");
      // ... and resubscribe
      client.subscribe(inTopic);
      mqttConnected=1;
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      mqttConnected=0;
      // Wait 5 seconds before retrying
      delay(5000);
    }
  //}
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor"] = "test";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  Serial.println(jsonBuffer);

  client.publish(outTopic, jsonBuffer);
}

//서버에서 mqtt 통신으로 전달받은 메세지
void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  /*
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  String macIn = doc["mac"];
  String sAdd=",\"macFrom\":\""+loraC.mac+"\",\"dire\":1,\"layer\":0}";
  sMsg=String(payload);
  sMsg.replace("}",sAdd);
  //Serial.println(sMsg);
  */
}

void setup() {
  Serial.begin(115200);
  if(counter==0) {
    readXaiomi();
    counter++;
  }
  Serial.println(counter);
  counter=0;
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  Serial.println("mac address");
  //이름 자동으로 생성
  uint64_t chipid;
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  sMac=String(((uint16_t)(chipid>>32)),HEX)+String(((uint32_t)chipid),HEX); 
  sMac.toCharArray(clientName,sMac.length()+1);
  Serial.println(sMac);
  
  readConfig();
  if(ssid[0]==0) {
    bootWifiAp();
    server.on("/", handleRoot);
    server.on("/wifisave", handleWifiSave);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
    nWifi = WiFi.scanNetworks();
    Serial.println(nWifi);
  }
  else {
    bootWifiStation();
    connectAWS();
    //publishMessage();
    client.publish(outTopic, mqttBuffer);
    Serial.println(mqttBuffer);
    hibernate();
  }
}

//1초 마다 실행되는 시간함수
void tick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    publishMessage();
    hibernate();
  }
}

void loop() {
  if(bootMode==1)
    server.handleClient();
  else {
    if (!client.connected()) {
      reconnect();
    }
    else
      client.loop();
    //tick();
  }
  //공장리셋
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
    factoryDefault();
}
