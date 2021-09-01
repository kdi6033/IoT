
#include <ArduinoJson.h>
#include <Ticker.h>
#include "CRC.h"
#include <SoftwareSerial.h>
Ticker ticker;
SoftwareSerial mySerial(D7, D4); // RX, TX


String inputString = "";     
int counter=0;

void tick()
{
  counter++;
  crd16Rtu();
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  ticker.attach(3, tick);
}

void loop() {
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
  Serial.println("");
  if(inputString.length() >= 11) {
    
      
  // for(int i=0;i<9;i++){
  //  Serial.println(inputString.charAt(i));
  // }
    String mo="습도 : ";
    String tem="온도 : ";
    String ec="EC : ";

    float sM=((float)inputString.charAt(3)*256+(float)inputString.charAt(4))/100;
    float sT=((float)inputString.charAt(5)*256+(float)inputString.charAt(6))/100;
    float sE=((float)inputString.charAt(7)*256+(float)inputString.charAt(8))/100;
    
    String MO = mo+sM+"%VWC";
    String TEM = tem+sT+"°C";
    String EC = ec+sE+"dS/m";
  
    Serial.println(MO);
    Serial.println(TEM);
    Serial.println(EC);
    inputString="";
    Serial.println("");

  } 
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0x01,0x04,0x00,0x07,0x00,0x03,0x00,0x00};  //[1,4,0,7,0,3,0,0]
  String s;
  int si,sj,len;

  len=6;
  
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;

  for(int i=0;i<len+2;i++) {
    mySerial.print(str[i]);
    //Serial.println((int)str[i]);
  }
}
