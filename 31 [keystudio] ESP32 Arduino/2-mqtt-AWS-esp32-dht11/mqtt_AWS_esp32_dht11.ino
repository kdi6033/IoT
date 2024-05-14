//아마존자료 https://aws.amazon.com/ko/blogs/compute/building-an-aws-iot-core-device-using-aws-serverless-and-an-esp32/
//IoT Core 설치 유튜브 https://youtu.be/5ojpKiQ00qk
//MQTTClient.h 라이브러리 다운로드
//   https://github.com/kdi6033/cloud/blob/master/6-2-0%20aws%20ESP32%20Mqtt/arduino-mqtt-master.zip
//참조 유튜브 https://www.youtube.com/watch?v=kgJMwPFN30M
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> 
#include <MQTTClient.h>
#include "secrets.h"
#include "DHT.h"

#define DHTPIN 17     // 센서를 연결할 디지털 핀
#define DHTTYPE DHT11   // DHT 11 센서를 사용
DHT dht(DHTPIN, DHTTYPE);
// LED 핀 설정
const int ledPin = 19;


/*
const char* mqtt_server = "*****.iot.us-west-2.amazonaws.com";
// AWS IoT 인증서 (PEM 형식)
const char* device_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAJR0itZEjCxLTep1jDFxoQQP4wLyMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
-----END CERTIFICATE-----
)EOF";
// AWS IoT 사물 프라이빗 키 (PEM 형식)
const char* device_private_key = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEArEzMNur+G67NI8CT+x2Bj+HOoD1wqv4njaOiZw1kHp7Jws4v
kUQyOH2RmFm1HoCar7V0HHFJH9wH22shRnXM/0P7SHf+CPArhjbe1n3n4la6Kzs1
-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Root CA 1
const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
-----END CERTIFICATE-----
)EOF";
*/


WiFiClientSecure net;
PubSubClient client(net);
void callback(char* topic, byte* payload, unsigned int length);

struct WifiMqtt {
public:
  String ssid="i2r";
  String password="00000000";
  //String mqtt_server = "ats.iot.us-west-2.amazonaws.com"; // 브로커 
  int mqtt_port = 8883;
  char outTopic[50]="i2r/out"; 
  char inTopic[50]="i2r/in";  
  String mac="";
  void loop();
  void connectToWiFi();
  void publishMqtt();
  void reconnectMQTT();
  void setup() {
    this->connectToWiFi();
    Serial.print("Connecting to AWS IoT Core...");
    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);
    client.setServer(mqtt_server, this->mqtt_port);
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

    this->temp = t;
    this->humi = round(h);

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
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
  char temp_str[8];
  dtostrf(sensor.temp, 4, 1, temp_str); // 전체 너비 4자리, 소수점 아래 1자리
  //sensor.temp = atof(temp_str);
  //Serial.println(this->temp);

  DynamicJsonDocument responseDoc(256);
  responseDoc["mac"] = this->mac;
  responseDoc["temp"] = temp_str;  // 문자열로 변환된 온도 데이터
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

