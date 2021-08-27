#include "CRC.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D7, D4); // RX,FX 
unsigned int countTick=0;
unsigned int countMqtt=0;
unsigned int countMeasure=0;
unsigned long previousMillis = 0;     
const long interval = 1000; 
String inputString = "";         // 받은 문자열
//float TEM=0,HUM=0,CO=0;
float temp=0,humi=0,co2=0;

void tick()
{
  countTick++;
  if(countTick > 10000)
    countTick=0;

  if((countTick%3)==0)
    tickMeasure();
  if((countTick%5)==0) {
    countMqtt++;
    //tickMqtt();
  }
}

void tickMeasure()
{
  crd16Rtu();
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  doTick();
  serialEvent();
}

void serialEvent() {
  if(mySerial.available() == false)
    return;
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    Serial.print(inChar,HEX);
    // add it to the inputString:
    inputString += inChar;
  }
  Serial.print("");
  if(inputString.length() >= 23) {     
    String A = "0x" + inputString.substring(7,11);
    String B = "0x" + inputString.substring(11,15);
    String C = "0x" + inputString.substring(15,19);
    temp = (A.toFloat()-200)/10;
    humi = (B.toFloat())/10;
    co2 = C.toFloat();

    Serial.println("");
    Serial.println("온도 : "+(String)temp);
    Serial.println("습도 : "+(String)humi);
    Serial.println("CO2 : "+(String)co2);
    inputString="";
    Serial.println("");
  } 
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    tick();
  }  
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0x3A,0x30,0x31,0x30,0x34,0x30,0x30,0x30,0x31,0x30,0x30,0x30,0x33,0x66,0x37,0x0D,0x0A};   // :010400010003F7\r\n

  for(int i=0;i<23;i++) {
    mySerial.print(str[i]);
    //Serial.println((int)str[i]);
  }
}
