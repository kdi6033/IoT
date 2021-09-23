#include "heltec.h" 
#include "images.h"
#include <WiFi.h>
#include <PubSubClient.h>
#define BAND    923E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

char ssid[32] = "mecha1203";
char password[32] = "mecha1203";
unsigned long lastConnectTry = 0;
unsigned int sWifi;


// mqtt를 위한 설정
const char* mqtt_server = "192.168.0.63";
const char* outTopic = "/test/outTopic";
const char* inTopic = "/test/inTopic";
const char* clientName = "4181"; // AP 이름과 같이 사용
char msg[100];

WiFiClient espClient;
PubSubClient client(espClient);

int connectWifi(){
  WiFi.begin(ssid, password);
  delay(3000);

  sWifi = WiFi.status();
  Serial.print ( "wifi status: " );
  Serial.println ( sWifi );
  if(sWifi == WL_CONNECTED){
    Serial.println("well connected");
    Serial.println(WiFi.localIP());
  }
  else
    WiFi.disconnect();
  drawFontFaceDemo();
  return sWifi;
}

void drawFontFaceDemo() {
    Heltec.display->clear();
    String s;
    if(sWifi!=WL_DISCONNECTED)
      s="ip "+toStringIp(WiFi.localIP());
    else
      s="wifi disconnected";
    Heltec.display->drawString(0, 0, s);
    Heltec.display->drawString(0, 20, packet);
    Heltec.display->display();
}

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet);
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  Serial.println(rssi);
  Serial.println(packet);
  LoRaData();
  packet.toCharArray(msg, packet.length()+1);
  client.publish(outTopic, msg);
}

void setup() { 
  DisplaySetup();
  Serial.begin(9600);
  connectWifi();
  LoRa.receive();
    // mqtt 설정
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Serial2.print((char)payload[i]);
  }
  Serial.println();
}

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Reconnected");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);

  long now = millis();
  //6초에 한번 와이파이 끊기면 다시 연결
  sWifi = WiFi.status();
  //Serial.println(sWifi);
  if (sWifi == WL_CONNECTED ) {
    if (!client.connected() && sWifi==WL_CONNECTED) {
      reconnect();
    }
    client.loop();
  }
  else {
    if(now > (lastConnectTry + 6000)) {
      lastConnectTry=now;
      Serial.println ( "Connect requested" );
      connectWifi();
    }
  }
}

void DisplaySetup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
}


/** IP to String? */
String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
