#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#define AWS_IOT_PUBLISH_TOPIC   "/jlee/outTopic"
#define AWS_IOT_SUBSCRIBE_TOPIC "/jlee/inTopic"
//////////////////////////////////////////////////
#include "SPI.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_system.h"
#include <sstream>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#define SCAN_TIME  10 // seconds

float humid = 30.0;
float temper = 20.0;
////////////////////////////////////////////////////
boolean METRIC = true; //Set true for metric system; false for imperial
/////////////////////////////////////////////////////
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = humid;
  doc["temperature"] = temper;
  doc["mac"] = "17:75:bc:11:28:99";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void publishing()
{
  char msg[100];
  String json;
  json = "{";
  json += "\"mac\":\""; json += "17:75:bc:11:28:99";  json += "\"";
  json += ",\"ip\":\""; json += WiFi.localIP().toString();  json += "\"";
  json += ",\"type\":"; json += 16;
  json += ",\"humidity\":\""; json += String(humid) +"\"";
  json += ",\"temperature\":\""; json += String(temper) +"\"";
  json += "}";
  json.toCharArray(msg, json.length()+1);
  Serial.println(msg);
  client.publish(AWS_IOT_PUBLISH_TOPIC, msg);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

////////////////////////////////////////////////
BLEScan *pBLEScan;

void IRAM_ATTR resetModule(){
    ets_printf("reboot\n");
    esp_restart();
}
    
bool isCollected = false; //is temp collected?

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        if (advertisedDevice.getAddress().toString() == "17:75:bc:11:28:99") { 
          //17:75:bc:11:28:99은 제 기기의 mac address여서 nRF connect나 Xiaomi Home으로 먼저 기기에 연결하시고 가지고 계신 기기의 mac address로 바꿔주셔야 합니다.
            int serviceDataCount = advertisedDevice.getServiceDataCount();
            std::string strServiceData = advertisedDevice.getServiceData(0);
            uint8_t cServiceData[100];
            char charServiceData[100];
            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
            for (int i=0;i<strServiceData.length();i++) {
                sprintf(&charServiceData[i*2], "%02x", cServiceData[i]);
            }
            unsigned long value;
            char charValue[5] = {0,};
            float temp_stored, hum_stored;
            int sl = strServiceData.length();
            if(sl == 17){
              sprintf(charValue, "%02X%02X", cServiceData[16], cServiceData[15]);
              value = strtol(charValue, 0, 16);
              if(isCollected == false && (int)strServiceData[sl-1] == 0){
                Serial.print("Temp : ");
                temper = (float)value/10;
                Serial.println(temper);
                isCollected = true;
              }
              if(isCollected == true && (int)strServiceData[sl-1] != 0){
                Serial.print("Humidity :");
                humid = (float)value/10;
                Serial.println(humid);
                Serial.println("-----------------end of sample------------------");
                publishing();
                isCollected = false;
              }
            }
            std::stringstream ss;
            ss << "fe95" << charServiceData;
        }
    }
};

void setup() {
  
  Serial.begin(115200);
  Serial.println("ESP32 XIAOMI DISPLAY");
  connectAWS();
  initBluetooth();
}

void loop() {
  
    char printLog[256];
    client.loop();
    BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
    delay(100);
}


void initBluetooth()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(0x50);
    pBLEScan->setWindow(0x30);
}
