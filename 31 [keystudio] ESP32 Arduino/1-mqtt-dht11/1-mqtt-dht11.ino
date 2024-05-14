#include "WiFi.h"
#include "PubSubClient.h"
#include "DHT.h"
#include <ArduinoJson.h> 

#define DHTPIN 17     // 센서를 연결할 디지털 핀
#define DHTTYPE DHT11   // DHT 11 센서를 사용
DHT dht(DHTPIN, DHTTYPE);
// LED 핀 설정
const int ledPin = 19;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length);


struct WifiMqtt {
public:
  String ssid="";
  String password="";
  String mqtt_server = "ai.doowon.ac.kr"; // 브로커 
  int mqtt_port = 1883;
  char outTopic[50]="i2r/out"; 
  char inTopic[50]="i2r/in";  
  String mac="";
  void loop();
  void connectToWiFi();
  void publishMqtt();
  void reconnectMQTT();
  void setup() {
    this->connectToWiFi();
    // MQTT 설정
    client.setServer(this->mqtt_server.c_str(), this->mqtt_port);
    client.setCallback(callback);
    // MQTT 연결 시도
    this->reconnectMQTT();
  }
} mqtt;

struct Sensor {
public:
  float temp;
  int humi;
  unsigned long lastcurrentMillis = 0;

  void setup() {
    dht.begin();
  }

  void measure() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    humi = round(h);
    char temp_str[8];
    dtostrf(t, 4, 1, temp_str);
    temp = atof(temp_str);
    mqtt.publishMqtt();
  }

  void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastcurrentMillis >= 3000) {
      lastcurrentMillis = currentMillis;
      measure();
    }
  }
} sensor;  // 세미콜론을 추가


/* 와이파이 MQTT 함수 ===============================================*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // JSON 파싱
  //parseJSONPayload(payload, length);
}

void WifiMqtt::publishMqtt()
{ 
  DynamicJsonDocument responseDoc(256);
  responseDoc["mac"] = this->mac;
  responseDoc["temp"] = sensor.temp;  // 문자열로 변환된 온도 데이터
  responseDoc["humi"] = sensor.humi;  // 반올림된 습도 데이터
  String sendData;
  serializeJson(responseDoc, sendData);
  Serial.println(sendData);
  client.publish(this->outTopic, sendData.c_str());
}

void WifiMqtt::connectToWiFi() {
  // ESP32의 MAC 주소를 읽어서 String 객체 mac에 저장
  byte macArr[6];
  WiFi.macAddress(macArr);
  char macStr[18]; // MAC 주소를 저장할 char 배열 (17 문자 + 널 종료 문자)

  // MAC 주소를 문자열로 포맷팅
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           macArr[0], macArr[1], macArr[2], macArr[3], macArr[4], macArr[5]);

  this->mac = String(macStr); // char 배열을 String 객체로 변환
  Serial.print("MAC 주소: ");
  Serial.println(mac);

  // Wi-Fi 연결
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi 연결됨!");
  Serial.print("IP 주소: ");
  Serial.println(WiFi.localIP());
}

void WifiMqtt::loop() {
  if (!client.connected()) {
    this->reconnectMQTT();
  }
  client.loop();
}

void WifiMqtt::reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("MQTT 서버에 연결 시도...");
    if (client.connect(this->mac.c_str())) {
      Serial.println("연결됨");

      // MQTT 구독 시작
      client.subscribe(mqtt.inTopic);
    } else {
      Serial.print("실패, 다시 시도...");
      Serial.print(client.state());
      delay(5000);
    }
  }
}
/* 와이파이 MQTT 함수 ===============================================*/


void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  sensor.setup();
  mqtt.setup();
}

void loop() {
  sensor.loop();
  mqtt.loop();
}

