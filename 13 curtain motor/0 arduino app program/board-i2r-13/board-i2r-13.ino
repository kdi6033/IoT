#include <WiFi.h>
#include <PubSubClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include <FS.h>
#include <Wire.h>
#include <HTTPUpdate.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

// Output pin numbers
const int outputPins[4] = { 26, 27, 32, 33 };
// Input pin numbers
const int inputPins[4] = { 16, 17, 18, 19 };
#define TRIGGER_PIN 34  // trigger pin GPIO36:i2r-04 GPIO34:i2r-03
const int ledPin = 2;

// Define the Data structure
struct DataDevice {
  String mac = "";  // Bluetooth mac address 를 기기 인식 id로 사용한다.
  int out[8];
  int in[8];
  float humi;
  float temp;
  String strIn = "0000", strInPre = "0000";  // In[] 을 string으로 저장
  String sendData = "";                      // 보드의 입력,출려,전압 데이터를 json 형태로 저장
  int noSelect = 2;  // UI의 noSelect의 값에 따라 모터가 동자 0:올림 1:내림 기타:정지
};

struct DataBle {
  char *service_uuid = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  char *characteristic_uuid = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
  bool isConnected = false;
};

struct DataWifiMqtt {
  bool selectMqtt = false;
  bool isConnected = false;
  bool isConnectedMqtt = false;
  bool use = false;
  String ssid = "";
  String password = "";
  String email = "";
  String mqttBroker = "ai.doowon.ac.kr";  // 브로커
  String mac = "";                        // BLE MAC 주소를 저장할 필드 추가
  char outTopic[50];                      // 초기값 제거
  char inTopic[50];                       // 초기값 제거
};

// Create an instance of the Data structure
DataDevice dev;
DataBle ble;
DataWifiMqtt wifi, wifiSave;

WiFiClient espClient;
PubSubClient client(espClient);

// 전역 변수
const unsigned long POLLING_INTERVAL = 100; // 폴링 간격을 밀리초 단위로 설정 (예: 100ms)
unsigned long lastPollTime = 0; // 마지막 폴링 시간을 저장할 변수

float lasthumidity = 0.0;
float lasttemperature = 0.0;
bool isFirmwareUpdating = false;
unsigned int counter = 0;
unsigned long lastTime = 0;  // 마지막으로 코드가 실행된 시간을 기록할 변수
const long interval = 10000;  // 실행 간격을 밀리초 단위로 설정 (3초)
unsigned int event = 0;
String returnMsg = "";

BLECharacteristic *pCharacteristic;

void actMachine(int noSelect);
void callback(char *topic, byte *payload, unsigned int length);
void checkFactoryDefault();
void connectToWiFi();
void doTick();
void loadConfigFromSPIFFS();
void parseJSONPayload(byte *payload, unsigned int length);
void prepareDataForMqtt();
void publishMqtt();
void readBleMacAddress();
void reconnectMQTT();
void returnMessage();
void saveConfigToSPIFFS();
void setup();
void setupBLE();
void updateCharacteristicValue();
void writeToBle(int order);
bool initializeSPIFFS();
void saveConfigToSPIFFS01();

void download_program(String fileName);
void update_started();
void update_finished();
void update_progress(int cur, int total);
void update_error(int error);

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // 각 출력 및 입력 핀을 설정
  for (int i = 0; i < 4; i++) {
    pinMode(outputPins[i], OUTPUT);
    pinMode(inputPins[i], INPUT);
  }
  // 온습도 센서 초기화
  if (!aht.begin()) {
    Serial.println("Could not find AHT sensor!");
    while (1) delay(10);
  }

  dev.humi = 0.0;
  dev.temp = 0.0;

  loadConfigFromSPIFFS();
  
  setupBLE();
  // BLE이 제대로 초기화될 수 있도록 약간의 시간을 기다립니다.
  delay(1000);
  // 이제 BLE MAC 주소를 읽어 봅니다.
  readBleMacAddress();
  Serial.println("BLE ready!");
}

/* 블루투스 함수 ===============================================*/
// 받은 order의 리턴정보
void writeToBle(int order) {
  // Create a JSON object
  DynamicJsonDocument responseDoc(1024);
  // Fill the JSON object based on the order
  if (order == 1) {
    responseDoc["order"] = order;
    responseDoc["ssid"] = wifi.ssid;
    responseDoc["password"] = wifi.password;
    responseDoc["email"] = wifi.email;
  }

  // Serialize JSON object to string
  String responseString;
  serializeJson(responseDoc, responseString);
  if (order == 0) {
    responseString = "프로그램 다운로드";
  } else if (order == 2) {
    // responseDoc["order"] = order;
    responseString = dev.sendData;
  } else if (order == 101) {
    responseString = dev.sendData;
  }
  

  // Convert String to std::string to be able to send via BLE
  std::string response(responseString.c_str());

  // Send the JSON string over BLE
  if (pCharacteristic) {
    pCharacteristic->setValue(response);
    pCharacteristic->notify();  // If notification enabled
  }
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      wifi.isConnected = true; // Set the isConnected flag to true on connection
      Serial.println("Device connected");
      ble.isConnected = true;
      //wifi.selectMqtt = false;
      event = 1;
    }

    void onDisconnect(BLEServer* pServer) {
      wifi.isConnected = false; // Set the isConnected flag to false on disconnection
      Serial.println("Device disconnected");
      ble.isConnected = false;
      //wifi.selectMqtt = true;
      BLEDevice::startAdvertising();  // Start advertising again after disconnect
    }
};

// 전송된 문자를 받는다.
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      Serial.println("Received on BLE:");
      for (int i = 0; i < value.length(); i++) {
        Serial.print(value[i]);
      }
      Serial.println();
      // std::string을 byte*로 변환
      parseJSONPayload((byte *)value.c_str(), value.length());
    }
  }
};

void setupBLE() {
  BLEDevice::init("i2r-13-IoT PLC");
  BLEServer *pServer = BLEDevice::createServer();

  // Set server callbacks
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(ble.service_uuid);
  pCharacteristic = pService->createCharacteristic(
    ble.characteristic_uuid,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setValue("");                     // 초기 값 설정
  pCharacteristic->setValue(std::string(200, ' '));  // 최대 길이를 200으로 설정

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(ble.service_uuid);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE service started");
}

void readBleMacAddress() {
  // BLE 디바이스에서 MAC 주소를 가져옵니다.
  BLEAddress bleAddress = BLEDevice::getAddress();
  // MAC 주소를 String 타입으로 변환합니다.
  dev.mac = bleAddress.toString().c_str();
  // MAC 주소를 모두 대문자로 변환합니다.
  dev.mac.toUpperCase();
  // MQTT 토픽 업데이트
  sprintf(wifi.outTopic, "i2r/%s/out", dev.mac.c_str());
  sprintf(wifi.inTopic, "i2r/%s/in", dev.mac.c_str());
  // 시리얼 모니터에 BLE MAC 주소와 MQTT 토픽을 출력합니다.
  Serial.print("BLE MAC Address: ");
  Serial.println(dev.mac);
  Serial.print("Out Topic: ");
  Serial.println(wifi.outTopic);
  Serial.print("In Topic: ");
  Serial.println(wifi.inTopic);
}

/* 블루투스 함수 ===============================================*/

/* 와이파이 MQTT 함수 ===============================================*/
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // JSON 파싱
  parseJSONPayload(payload, length);
}

void publishMqtt() {
if (client.connected()) {
    Serial.println("MQTT connected. Publishing message...");
    Serial.println(dev.sendData);
    client.publish(wifi.outTopic, dev.sendData.c_str());
    // 메시지 발행 시도
    // if (client.publish(wifi.outTopic, dev.sendData.c_str())) {
    //   Serial.println("Message published successfully.");
    //   Serial.print("Published to: ");
    //   Serial.println(wifi.outTopic);
    //   Serial.print("Message: ");
    //   Serial.println(dev.sendData);
    // } else {
    //   Serial.println("Failed to publish message.");
    // }
  } else {
    Serial.println("MQTT not connected. Attempting to reconnect...");
    reconnectMQTT(); // MQTT에 재연결 시도
  }
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(wifi.ssid, wifi.password);
  
  int wCount = 0;
  wifi.isConnected = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    checkFactoryDefault();
    wCount++;
    if(wCount > 10) {
      wifi.isConnected = false;
      returnMsg="와이파이 정보가 잘못되었습니다.";
      writeToBle(101);
      break; // while 루프를 벗어납니다.
    }
  }
  if(wifi.isConnected == true)
    Serial.println("\nConnected to Wi-Fi");
  else
    Serial.println("\nCan not find Wi-Fi");
  
}

void reconnectMQTT() {
  while (!client.connected()) {
    //checkFactoryDefault();reconnectMQTT
    Serial.println("Connecting to MQTT...");
    if (client.connect(dev.mac.c_str())) {
      Serial.println("Connected to MQTT");
      client.subscribe(wifi.inTopic);  // MQTT 토픽 구독
      wifi.isConnectedMqtt = true;
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      wifi.isConnectedMqtt = false;
      delay(5000);
    }
  }
  digitalWrite(ledPin, wifi.isConnectedMqtt);
}
/* 와이파이 MQTT 함수 ===============================================*/

/* Tools ===========================================================*/
void parseJSONPayload(byte *payload, unsigned int length) {
  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';  // Null-terminate the string
  Serial.println(payloadStr);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payloadStr);

  if (error) {
    Serial.println("JSON 파싱 실패!");
    return;
  }

  int order = doc["order"] | -1;
  if (order == 0) {
    writeToBle(order);
    const char *fileName = doc["fileName"] | "";
    download_program(fileName);
  } 
  else if (order == 1) {
    const char *ssid = doc["ssid"] | "";
    const char *password = doc["password"] | "";
    const char *email = doc["email"] | "";
    const char *mqttBroker = doc["mqttBroker"] | "";

    wifiSave.ssid = ssid;
    wifiSave.password = password;
    wifiSave.email = email;
    wifiSave.mqttBroker=mqttBroker;
    returnMsg = wifiSave.ssid + " 정보가 저장 되었습니다.";
    writeToBle(101);

    //Serial.print("wifi.ssid: "); Serial.println(wifiSave.ssid);
    //Serial.print("wifi.password: "); Serial.println(wifiSave.password);
    //Serial.print("wifi.email: "); Serial.println(wifiSave.email);
    saveConfigToSPIFFS();
  } 
  else if (order == 2) {
    // JSON 메시지에서 "no"와 "value" 값을 읽어옵니다.
    int no = doc["no"] | -1;  // 유효하지 않은 인덱스로 초기화
    bool value = doc["value"] | false;
    dev.out[no] = value;
    String State = "";
    // "no" 값이 유효한 범위 내에 있는지 확인하고, "out" 배열에 "value"를 설정합니다.
    if (no >= 0 && no < 8) {
      dev.out[no] = value ? 1 : 0;  // "true"이면 1로, "false"이면 0으로 설정
      Serial.print("out[");
      Serial.print(no);
      Serial.print("] 값이 ");
      Serial.print(value ? "true" : "false");
      Serial.println("로 설정되었습니다.");
      if(value) State = "ON";
      else State = "OFF";
      returnMsg=String(no)+"번 "+State;
      prepareDataForMqtt();  // MQTT를 통해 데이터 전송
    } 
    else {
      Serial.println("유효하지 않은 'no' 값입니다.");
    }
    //Serial.println(outputPins[no]);
    //Serial.println(dev.out[no]);
    digitalWrite(outputPins[no], dev.out[no]);
  } else if (order == 3) {
    bool value = doc["value"] | false;
    wifi.selectMqtt = value;
    if (value == true)
      returnMsg = "mqtt로 통신 합니다.";
    else
      returnMsg = "블루투스로 통신 합니다.";
  }
  else if (order == 4) {  // 커튼에서 knob의 상태를 읽어온다.
    int noSelect = doc["noSelect"] | 2;
    dev.noSelect = noSelect;
    actMachine(dev.noSelect);
  }
  returnMessage();
}
void prepareDataForMqtt() {
    String strIn,strOut;
    sensors_event_t humidity_event, temp_event;
    aht.getEvent(&humidity_event, &temp_event);

    dev.humi = humidity_event.relative_humidity;
    dev.temp = temp_event.temperature;

    strIn=String(dev.in[0])+String(dev.in[1])+String(dev.in[2])+String(dev.in[3]);
    strOut=String(dev.out[0])+String(dev.out[1])+String(dev.out[2])+String(dev.out[3]);
    String strHumidity = String(dev.humi, 1);
    String strTemp = String(dev.temp, 1);
    dev.strInPre=dev.strIn;
    // 데이터 변경 여부 확인
    bool dataChanged = !dev.strIn.equals(dev.strInPre) || dev.temp != lasttemperature || dev.humi != lasthumidity;

    if (dataChanged) {
      DynamicJsonDocument responseDoc(2048);
      responseDoc["order"] = 3;
      responseDoc["type"] = 13;
      responseDoc["email"] = wifi.email;
      responseDoc["mac"] = dev.mac;
      responseDoc["in"] = strIn;
      responseDoc["out"] = strOut;
      responseDoc["humi"] = strHumidity;
      responseDoc["temp"] = strTemp;
      responseDoc["noSelect"] = dev.noSelect;
      dev.sendData = "";
      serializeJson(responseDoc, dev.sendData);

      // MQTT에 연결되어 있을 경우 항상 MQTT로 데이터 전송
      if (client.connected()) {
        publishMqtt();
        Serial.println(dev.sendData);
      } 
      // MQTT에 연결되어 있지 않고 BLE에 연결되어 있을 경우 BLE로 데이터 전송
      else if (!client.connected() && ble.isConnected && pCharacteristic) {
        writeToBle(2);
        Serial.println("BLE");
        Serial.println(dev.sendData);
      }
      // 이전 bat와 adc 값을 업데이트
      lasttemperature = dev.temp;
      lasthumidity = dev.humi;  

    } // dataChanged
}

void returnMessage() {
  DynamicJsonDocument responseDoc(1024);
  responseDoc["order"] = 101;
  responseDoc["message"] = returnMsg;
  dev.sendData = "";
  serializeJson(responseDoc, dev.sendData);
  Serial.print("returnMessage: ");
  Serial.println(dev.sendData);
  if (wifi.selectMqtt == true)
    publishMqtt();
  else
    writeToBle(101);
}

void actMachine(int noSelect) {
  //Serial.println("noSelect: "+String(noSelect));
  dev.noSelect = noSelect;
  if(noSelect==0) {
    digitalWrite(outputPins[0], LOW);
    delay(100);
    digitalWrite(outputPins[1], LOW);
    digitalWrite(outputPins[0], HIGH);
    Serial.println("Up");
  }
  else if(noSelect==1) {
    digitalWrite(outputPins[0], LOW);
    delay(100);
    digitalWrite(outputPins[1], HIGH);
    Serial.println("Down");
  }
  else {
    digitalWrite(outputPins[0], LOW);
    digitalWrite(outputPins[1], LOW);
    Serial.println("Stop");
  }
  prepareDataForMqtt();
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentTime = millis();  // 현재 시간을 가져옵니다
  // 현재 시간이 마지막 폴링 시간 + 설정된 폴링 간격보다 클 때만 폴링 수행
  if (currentTime - lastPollTime >= POLLING_INTERVAL) {
    lastPollTime = currentTime; // 마지막 폴링 시간 업데이트
    bool stateChanged = false; // 상태 변경 여부를 추적하는 플래그
    // 모든 입력 핀에 대해 상태를 확인
    for (int i = 0; i < 4; i++) {
      int currentState = digitalRead(inputPins[i]); // 현재 핀 상태 읽기
      // 현재 핀 상태가 이전 상태와 다른 경우 상태 변경 플래그를 true로 설정
      if (currentState != dev.in[i]) {
        dev.in[i] = currentState; // 상태 업데이트
        stateChanged = true; // 상태 변경 플래그 설정
        actMachine(i);
      }
    }

    // 상태 변경이 감지된 경우에만 데이터 처리 수행
    if (stateChanged) {
      prepareDataForMqtt();
      }
    }


  String strIn,strOut;
  if ( currentTime - lastTime >= interval) {
    lastTime = currentTime;
    // for (int i = 0; i < 4; i++) {
    //   // Read the state of the current input pin
    //   dev.in[i] = digitalRead(inputPins[i]);
    // }
    prepareDataForMqtt();

  } // internal
} // dotick


// Config 파일을 SPIFFS에서 읽어오는 함수
void loadConfigFromSPIFFS() {
  Serial.println("파일 읽기");

  if (!initializeSPIFFS()) {
    Serial.println("Failed to initialize SPIFFS.");
    return;
  }

  if (!SPIFFS.exists("/config.txt")) {
    Serial.println("Config file does not exist.");
    return;
  }

  File configFile = SPIFFS.open("/config.txt", FILE_READ);
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());
  
  if (error) {
    Serial.println("Failed to parse config file");
    return;
  }

  wifi.ssid = doc["ssid"] | "";
  wifi.password = doc["password"] | "";
  wifi.mqttBroker = doc["mqttBroker"] | "";
  wifi.email = doc["email"] | "";

  Serial.print("wifi.ssid: "); Serial.println(wifi.ssid);
  Serial.print("wifi.password: "); Serial.println(wifi.password);
  Serial.print("wifi.mqttBroker: "); Serial.println(wifi.mqttBroker);
  Serial.print("wifi.email: "); Serial.println(wifi.email);
  configFile.close();
  // 유효성 검사: ssid와 password가 비어 있지 않은지 확인
  if (wifi.ssid.length() > 0 && wifi.password.length() > 0) {
    connectToWiFi(); // Wi-Fi 연결 시도
    client.setServer(wifi.mqttBroker.c_str(), 1883); // MQTT 브로커 주소와 포트 설정
    client.setCallback(callback); // 콜백 함수 설정
  } else {
    Serial.println("Skipping WiFi connection");
  }
}

void saveConfigToSPIFFS() {
  Serial.println("config.txt 저장");

  if (!initializeSPIFFS()) {
    Serial.println("SPIFFS 초기화 실패.");
    return;
  }

  // SPIFFS 초기화를 시도합니다.
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed to initialize. Formatting...");
    // 초기화 실패 시 포맷을 시도합니다.
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS format failed.");
      return;
    }
    // 포맷 후에 다시 초기화를 시도합니다.
    if (!SPIFFS.begin()) {
      Serial.println("SPIFFS failed to initialize after format.");
      return;
    }
  }

  File configFile = SPIFFS.open("/config.txt", FILE_WRITE);
  
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return;
  }

  DynamicJsonDocument doc(1024);

  // 데이터를 구조체에서 가져온다고 가정합니다.
  doc["ssid"] = wifiSave.ssid;
  doc["password"] = wifiSave.password;
  doc["mqttBroker"] = wifiSave.mqttBroker;
  doc["email"] = wifiSave.email;

  Serial.print("wifi.ssid: "); Serial.println(wifiSave.ssid);
  Serial.print("wifi.password: "); Serial.println(wifiSave.password);
  Serial.print("wifi.mqttBroker: "); Serial.println(wifiSave.mqttBroker);
  Serial.print("wifi.email: "); Serial.println(wifiSave.email);

  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write to file");
    configFile.close();
    return;
  }

  configFile.close();
  // 파일이 제대로 닫혔는지 확인합니다.
  if (configFile) {
    Serial.println("파일이 여전히 열려있습니다.");
  } else {
    Serial.println("파일이 성공적으로 닫혔습니다.");
  }
  Serial.println("파일 저장 끝");

  // 파일이 제대로 저장되었는지 확인합니다.
  if (SPIFFS.exists("/config.txt")) {
    Serial.println("Config file saved successfully.");
    // 저장이 확인된 후 재부팅을 진행합니다.
    Serial.println("Rebooting...");
    delay(1000); // 재부팅 전에 짧은 지연을 줍니다.
    ESP.restart();
  } else {
    Serial.println("Config file was not saved properly.");
  }
  
  // ESP32 재부팅
  delay(1000);
  ESP.restart();
}

// SPIFFS를 초기화하고 필요한 경우 포맷하는 함수를 정의합니다.
bool initializeSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS 초기화 실패!");
    if (!SPIFFS.format()) {
      Serial.println("SPIFFS 포맷 실패!");
      return false;
    }
    if (!SPIFFS.begin()) {
      Serial.println("포맷 후 SPIFFS 초기화 실패!");
      return false;
    }
  }
  return true;
}

void checkFactoryDefault() {
  if (digitalRead(TRIGGER_PIN) == LOW) {
    Serial.println("Please wait over 3 min");
    SPIFFS.format();
    delay(1000);
    ESP.restart();
    delay(1000);
  }
}

// httpupdate()
void download_program(String fileName) {
  Serial.println(fileName);
  if (WiFi.status() == WL_CONNECTED) {
  isFirmwareUpdating = true;  // 펌웨어 다운로드 시작
    WiFiClient client;
    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    httpUpdate.setLedPin(ledPin, LOW);

    // Add optional callback notifiers
    httpUpdate.onStart(update_started);
    httpUpdate.onEnd(update_finished);
    httpUpdate.onProgress(update_progress);
    httpUpdate.onError(update_error);

    String ss;
    //ss=(String)URL_fw_Bin+fileName;
    ss = "http://i2r.link/download/" + fileName;
    Serial.println(ss);
    yield(); // WDT 리셋
    t_httpUpdate_return ret = httpUpdate.update(client, ss);
    yield(); // WDT 리셋
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
    // Or:
    //t_httpUpdate_return ret = ESPhttpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  isFirmwareUpdating = false;  // 펌웨어 다운로드 종료
  }
}

void update_started() {
  Serial.println("Update Started");
}

void update_finished() {
  Serial.println("Update Finished");
  isFirmwareUpdating = false;
  if(WiFi.status() != WL_CONNECTED) {
    connectToWiFi(); // Wi-Fi 재연결
  }
  digitalWrite(ledPin, HIGH); // LED 다시 켜기
}

void update_progress(int cur, int total) {
  yield(); // WDT 리셋
  digitalWrite(ledPin, HIGH); // LED 켜기
  Serial.printf("Progress: %d%%\n", (cur * 100) / total);
}
void update_error(int error) {
  Serial.printf("Update Error: %d\n", error);
  isFirmwareUpdating = false;
}

/* Tools ===========================================================*/


void loop() {
  
  if (!isFirmwareUpdating) {
    doTick();
  }

  if (event != 0) {
    writeToBle(event);
    event = 0;
  }

  // Wi-Fi 및 MQTT 설정을 여기서 처리
  if (wifi.isConnected) {
    if (!client.connected()) {
      reconnectMQTT();
    }
    client.loop();
  }


  checkFactoryDefault();

}
