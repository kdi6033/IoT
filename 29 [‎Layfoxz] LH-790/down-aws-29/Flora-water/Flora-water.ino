char msg[100];
char msg2[100];
char msg3[100];
char msg4[100];
char msg5[100];
char msg6[100];
char mac[20];
int type=25;
float temp=0;
String sMac;
//char* FLORA_DEVICES[] = { "C4:7C:8D:6D:E8:02", };첫번째
char* FLORA_DEVICES[] = { "C4:7C:8D:63:92:E5", };
#define SLEEP_DURATION 10
#define BATTERY_INTERVAL 6
#define RETRY 3
#include "BLEDevice.h"
#include "config.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

RTC_DATA_ATTR int bootCount = 0;
const char* ssid = "iptime"; // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* password = "19660412";  // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* mqtt_server = "broker.mqtt-dashboard.com"; //브로커 주소 //192.168.0.159 broker.mqtt-dashboard.com
const char* outTopic = "/i2r/outTopic"; // 이름이 중복되지 않게 설정 기록
//const char* inTopic = "/i2r/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* clientName = "";
// device count
static int deviceCount = sizeof FLORA_DEVICES / sizeof FLORA_DEVICES[0];

// the remote service we wish to connect to
static BLEUUID serviceUUID("00001204-0000-1000-8000-00805f9b34fb");

// the characteristic of the remote service we are interested in
static BLEUUID uuid_version_battery("00001a02-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_sensor_data("00001a01-0000-1000-8000-00805f9b34fb");
static BLEUUID uuid_write_mode("00001a00-0000-1000-8000-00805f9b34fb");

WiFiClient espClient;
PubSubClient client(espClient);

TaskHandle_t hibernateTaskHandle = NULL; 


void hibernate() {
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ll);
  Serial.println("Going to sleep now.");
  delay(100);
  esp_deep_sleep_start();
}

void connectMqtt() {
  Serial.println("Connecting to MQTT...");
  client.setServer(mqtt_server,1883);
  
  while (!client.connected()) {
    if (!client.connect(clientName)) {
      Serial.print("MQTT connection failed:");
      Serial.print(client.state());
      Serial.println("Retrying...");
      delay(5000);
    }
  }

  Serial.println("MQTT connected");
  Serial.println("");
}

void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // all action is done when device is woken up
  Serial.begin(115200);
  delay(1000);

  // increase boot count
  //bootCount++;

  Serial.println("Initialize BLE client...");
  BLEDevice::init("");
  BLEDevice::setPower(ESP_PWR_LVL_P7);
  delay(100);

  setup_wifi();
  delay(100);
  connectMqtt();

  // check if battery status should be read - based on boot count
  bool readBattery = ((bootCount % BATTERY_INTERVAL) == 0);

  // process devices
  for (int i=0; i<deviceCount; i++) {
    int tryCount = 0;
    char* deviceMacAddress = FLORA_DEVICES[i];
    BLEAddress floraAddress(deviceMacAddress);

    while (tryCount < RETRY) {
      tryCount++;
      if (processFloraDevice(floraAddress, deviceMacAddress, readBattery, tryCount)) {
        break;
      }
      delay(1000);
    }
    delay(1500);
  }

  // go to sleep now
  hibernate();
}


bool processFloraDevice(BLEAddress floraAddress, char* deviceMacAddress, bool getBattery, int tryCount) {
  Serial.print("Processing Flora device at ");
  Serial.print(floraAddress.toString().c_str());
  Serial.print(" (try ");
  Serial.print(tryCount);
  Serial.println(")");

  // connect to flora ble server
  BLEClient* floraClient = getFloraClient(floraAddress);
  if (floraClient == nullptr) {
    return false;
  }

  // connect data service
  BLERemoteService* floraService = getFloraService(floraClient);
  if (floraService == nullptr) {
    floraClient->disconnect();
    return false;
  }

  // process devices data
  bool success = processFloraService(floraService, deviceMacAddress, getBattery);

  // disconnect from device
  floraClient->disconnect();

  return success;
}

BLEClient* getFloraClient(BLEAddress floraAddress) {
  BLEClient* floraClient = BLEDevice::createClient();

  if (!floraClient->connect(floraAddress)) {
    Serial.println("- Connection failed, skipping");
    return nullptr;
    floraClient->connect(floraAddress);
  }

  Serial.println("- Connection successful");
  return floraClient;
}
//연결됐을 시 서비스 정보 받음
BLERemoteService* getFloraService(BLEClient* floraClient) {
  BLERemoteService* floraService = nullptr;

  try {
    floraService = floraClient->getService(serviceUUID);
  }
  catch (...) {
    // something went wrong
  }
  if (floraService == nullptr) {
    Serial.println("- Failed to find data service");
  }
  else {
    Serial.println("- Found data service");
  }

  return floraService;
} //여기까지 연결됐을 시 서비스정보 받는 내용

bool forceFloraServiceDataMode(BLERemoteService* floraService) {
  BLERemoteCharacteristic* floraCharacteristic;
  
  // get device mode characteristic, needs to be changed to read data
  Serial.println("- Force device in data mode");
  floraCharacteristic = nullptr;
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_write_mode);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    Serial.println("-- Failed, skipping device");
    return false;
  }

  // write the magic data
  uint8_t buf[2] = {0xA0, 0x1F};
  floraCharacteristic->writeValue(buf, 2, true);
  Serial.println("================================");

  delay(500);
  return true;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @@@@@@여기부터 데이터가 직접적으로 넘어오는 부분@@@@@@@@
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
bool readFloraDataCharacteristic(BLERemoteService* floraService, String baseTopic) {
  BLERemoteCharacteristic* floraCharacteristic = nullptr;

  // get the main device data characteristic
  Serial.println("- Access characteristic from device");
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_sensor_data);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    Serial.println("-- Failed, skipping device");
    return false;
  }

  // read characteristic value
  Serial.println("- Read value from characteristic");
  std::string value;
  try{
    value = floraCharacteristic->readValue();
  }
  catch (...) {
    // something went wrong
    Serial.println("-- Failed, skipping device");
    return false;
  }
  const char *val = value.c_str();

  Serial.print("Hex: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((int)val[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  /*snprintf (msg, 100, "{\"name\":\"Temperature\",\"value\":%.1f}", (val[0]+val[1]) / ((float)10.0));
  Serial.println(msg);*/
  //client.publish(outTopic,msg);
  String ss="";
  ss=( ((float)val[0]+(float)val[1]*256)/10 );
  temp=ss.toFloat();
  snprintf (msg, 100, "{\"Temperature\":%d}", (val[0] + val[1]*256)/10);
  Serial.println(msg);
  client.publish(outTopic,msg);
  

  /*snprintf (msg2, 100, "{\"name\":\"Moisture\",\"value\":%d}",val[7]);
  Serial.println(msg2);
  client.publish(outTopic,msg2);*/
  snprintf (msg2, 100, "{\"Moisture\":%d}", val[7]);
  Serial.println(msg2);
  client.publish(outTopic,msg2);
  
  /*snprintf (msg3, 100, "{\"name\":\"Light\",\"value\":%d}",val[3] + val[4] *256);
  Serial.println(msg3);
  client.publish(outTopic,msg3);*/
  snprintf (msg3, 100, "{\"Light\":%d}", val[3] + val[4] *256);
  Serial.println(msg3);
  client.publish(outTopic,msg3);
 
  /*snprintf (msg4, 100, "{\"name\":\"Conductivity\",\"value\":%d}",val[8] + val[9] * 256);
  Serial.println(msg4);*/
  snprintf (msg4, 100, "{\"Conductivity\":%d}", val[8] + val[9] * 256);
  Serial.println(msg4);
   if(val[8] + val[9] * 256>5)
  {
    client.publish(outTopic,"1");
  }
  else
  {
    client.publish(outTopic,"0");
  }
  //client.publish(outTopic,msg4);
  client.publish(outTopic,msg4);

  //이름 자동생성
  uint8_t macH[6]="";
  WiFi.macAddress(macH);
  sprintf(mac,"%02x%02x%02x%02x%02x%02x%c",macH[5], macH[4], macH[3], macH[2], macH[1], macH[0],0);
  sMac=mac;
  clientName=mac;
  Serial.println(mac);
  
  String json;
  json = "{";
  json += "\"mac\":\""; json += sMac;  json += "\"";
  json += ",\"ip\":\""; json += WiFi.localIP().toString();  json += "\"";
  json += ",\"type\":"; json += type;
  json += "}";
  json.toCharArray(msg6, json.length()+1);
  Serial.println(msg6);
  client.publish(outTopic, msg6);
}

 //배터리 잔량 표시 관련함수
bool readFloraBatteryCharacteristic(BLERemoteService* floraService, String baseTopic) {
  BLERemoteCharacteristic* floraCharacteristic = nullptr;

  // get the device battery characteristic
  Serial.println("- Access battery characteristic from device");
  try {
    floraCharacteristic = floraService->getCharacteristic(uuid_version_battery);
  }
  catch (...) {
    // something went wrong
  }
  if (floraCharacteristic == nullptr) {
    Serial.println("-- Failed, skipping battery level");
    return false;
  }

  // read characteristic value
  Serial.println("- Read value from characteristic");
  std::string value;
  try{
    value = floraCharacteristic->readValue();
  }
  catch (...) {
    // something went wrong
    Serial.println("-- Failed, skipping battery level");
    return false;
  }
  const char *val2 = value.c_str();
  char buffer[64];

  Serial.print("-- Battery: ");
  snprintf (msg5, 100, "{\"Battery\": %d}",val2[0]);
  Serial.println(msg5);
 // client.publish(outTopic,msg5);

  return true;
}

bool processFloraService(BLERemoteService* floraService, char* deviceMacAddress, bool readBattery) {
  // set device in data mode
  if (!forceFloraServiceDataMode(floraService)) {
    return false;
  }

  String baseTopic = deviceMacAddress;
  bool dataSuccess = readFloraDataCharacteristic(floraService, baseTopic);

  bool batterySuccess = true;
  if (readBattery) {
    batterySuccess = readFloraBatteryCharacteristic(floraService, baseTopic);
  }
   
  return true;
}

void loop() {
  /// we're not doing anything in the loop, only on device wakeup
  delay(10000);
}
