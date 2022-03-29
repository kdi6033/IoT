
#include <ArduinoJson.h>
#include <Ticker.h>
#include "CRC.h"
#include <SoftwareSerial.h>
Ticker ticker;
SoftwareSerial mySerial(D7, D4); // RX, TX


String inputString = "";     
int counter=0;
int type=1;

void tick()
{
  counter++;
  crd16Rtu();
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  ticker.attach(3, tick);
}

void loop() {
  serialEvent();
}

void serialEvent() {
  if(Serial.available() == false)
    return;
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    Serial.print(inChar,HEX);
    Serial.print(" ");
    // add it to the inputString:
    inputString += inChar;
  }

  int rs;
  int cLength;
  if(type==1)
    cLength=7;
  else if(type==2)
    cLength=7;
  else if(type==3)
    cLength=7;
    
  if(inputString.length() >= cLength) {
    Serial.println("");
    uint16_t myVar = 0;

    for(int i=0; i<str[5]; i++) {
      myVar=inputString.charAt(4);
      myVar <<= 8;
      myVar = myVar | inputString.charAt(4);
      rs=String(myVar,DEC).toInt();
      val[i]=(float)rs;
    }

    // 계측된 센서 소숫점 자리수 보정
    if(type==1) {
      val[0];
      val[1];
      val[2];
    }
    else if(type==2) {
      val[0]/=1;
    }
    else if(type==3) {
      val[0];
      val[1];
    }
    mqtt_send();
    inputString="";
    Serial.println("");
  }
}
/*void serialEvent() {
  if(mySerial.available() == false)
    return;
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    Serial.print(inChar,HEX);
    // add it to the inputString:
    inputString += inChar;
  }
  Serial.println("");
  if(inputString.length() >= 7) {
    
      
   //for(int i=0;i<9;i++){
   // Serial.println(inputString.charAt(i));
   //}
    String mo="비/눈 : ";

    float sM=((float)inputString.charAt(4));
    Serial.println(mo);
    if(sM==1){
      Serial.println("온다");
    }
    else{
      Serial.println("안온다");
    }
  
    inputString="";
    Serial.println("");

  } 
}*/

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0xff,0x03,0x00,0x00,0x00,0x01,0x91,0xd4};
  String s;
  int si,sj,len;

  if(type==1) {
    str[0]=0xff;str[1]=0x06;str[2]=0x00;str[3]=0x02;str[4]=0x01;str[5]=0xc2;str[6]=0xbd;str[7]=0xba; //heating 45
  }
  else if(type==2) {
    str[0]=0xff;str[1]=0x06;str[2]=0x00;str[3]=0x02;str[4]=0x00;str[5]=0x00;str[6]=0xbd;str[7]=0xba; //heating 0
  }
  //char str[24] =  {0x1f,0x04,0x00,0x01,0x00,0x03,0x00,0x00};  //pe300
  //char str[24] =  {0x1f,0x04,0x00,0x04,0x00,0x01,0x00,0x00};  //co2
  //char str[24] =  {0x1f,0x04,0x00,0x40,0x00,0x02,0x00,0x00}; // Te Hu 115200
  
  len=6;
  
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;

  for(int i=0;i<len+2;i++) {
    Serial.print(str[i],HEX);
    Serial.print(" ");
  }
  Serial.println(" ");
  for(int i=0;i<len+2;i++)
    Serial1.print(str[i]);
}
