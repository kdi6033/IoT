#include <ArduinoJson.h>
#include "CRC.h"
#include <SoftwareSerial.h>
#include "heltec.h"

#define BAND    923E6 
#define TRIGGER_PIN 13 // trigger pin 13
SoftwareSerial mySerial(23,17);

String inputString = "";     
float temp=0.,humi=0.;
unsigned int countTick=0;
unsigned long previousMillis = 0;     
const long interval = 1000;  
String rssi = "RSSI --";
String packSize = "--";
String packet ;
unsigned int count = 0;
long reading;
uint64_t chipid;
String chipidS;

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    tick();
  }  
}
void tick()
{
  countTick++;
  if(countTick > 10000)
    countTick=0;

  if((countTick%3)==0)
    tickMeasure();
}

void tickMeasure()
{
  crd16Rtu();
  drawFontFaceDemo();
  sendData();
  Serial.println(chipidS);
}

void factoryDefault() {
    Serial.println("AP mode as Factory Deafault");
    Serial.println("Please wait over 3 min");
    //displayOled(5);
    //SPIFFS.begin();
    //SPIFFS.format();
    //SPIFFS.end();
    delay(1000);
    //ESP.restart();
    delay(1000);
}

void sendData() {
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("{\"chip\":\"");
  LoRa.print(chipidS);
  LoRa.print("\",\"temp\":");
  LoRa.print(temp);
  LoRa.print(",\"humi\":");
  LoRa.print(humi);
  LoRa.print("}");
  LoRa.endPacket();
}
void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 10, "Temp: "+String(temp));
    Heltec.display->drawString(0, 20, "Humi: "+String(humi));
    Heltec.display->display();
    Heltec.display->clear();
}

void setup() {
  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  chipidS=String(((uint16_t)(chipid>>32)),HEX)+String(((uint32_t)chipid),HEX);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  mySerial.begin(9600); //s2
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  //logo();
  //delay(1500);
}

void loop() {
  serialEvent();
  doTick();
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
   factoryDefault();  
}


void serialEvent() {
  if(mySerial.available() == false) //s2
    return;
  while (mySerial.available()) { //s2
    // get the new byte:
    char inChar = (char)mySerial.read(); //s2
    Serial.print(inChar,HEX);
    // add it to the inputString:
    inputString += inChar;
  }
  
  if(inputString.length() >=9) {
    Serial.println("");
    String ss="";
    ss=((float)inputString.charAt(3)*256+(float)inputString.charAt(4))/100-40;
    temp=ss.toFloat();
    Serial.println("온도 : "+ss+" °C"); 
    ss=((float)inputString.charAt(5)*256+(float)inputString.charAt(6))/100;
    humi=ss.toFloat();
    Serial.println("습도 : "+ss+" %");
    inputString="";
    Serial.println("");
  }
}
// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  //char str[24] =  {0x00,0x03,0x00,0x01,0x00,0x01,0x00,0x00,0x00};  //Read station number
  char str[24] =  {0xff,0x03,0x00,0x00,0x00,0x02,0x00,0x00};  //Read Date

  String s;
  int si,sj,len;

  len=6;
  
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;

  //Serial.println("");
  for(int i=0;i<len+2;i++) {
    mySerial.print(str[i]); //S2
    //Serial.print(" ");
    //Serial.print((int)str[i],HEX);
  }
}
