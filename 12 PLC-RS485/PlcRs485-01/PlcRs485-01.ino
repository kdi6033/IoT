#include <SoftwareSerial.h>
#include "CRC.h"

SoftwareSerial mySerial(D7, D4); // RX, TX

unsigned long previousMillis = 0;   
const long interval = 3000; 
String inputString = "";         // 받은 문자열

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("start");
}


void loop() {
  // put your main code here, to run repeatedly:
  crd16Rtu();
  delay(3000);
}


// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  String s;
  int si,sj,len;
  char str[24];
  //출력
  str[0]=0xff; str[1]=0x05; str[2]=0x00; str[3]=0x03; str[4]=0xff;
  str[5]=0x00; str[6]=0x00; str[7]=0x00; 
  len=6;

  inputString = "";
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;

  for(int i=0;i<len+2;i++)
    mySerial.print(str[i]);
}
