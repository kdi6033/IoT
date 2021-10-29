#include <SoftwareSerial.h>
#include "CRC.h"

SoftwareSerial mySerial(D7, D4); // RX, TX

unsigned long previousMillis = 0;   
const long interval = 3000; 
String inputString = "";         // 받은 문자열
int iout=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  mySerial.begin(19200);
  Serial.println("start");
}


void loop() {
  // put your main code here, to run repeatedly:
  crd16Rtu();
  delay(1000);
}


// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  String s;
  int si,sj,len;
  char str[24];
  //출력
  //str[24] =  {0x01,0x0f,0x00,0x40,0x00,0x0a,0x02,0xff,0x00,0x00,0x00};  //비트연속출력 len=9
  str[0]=0x01; str[1]=0x0f; str[2]=0x00; str[3]=0x40; str[4]=0x00;
  str[5]=0x0a; str[6]=0x02; str[7]=0xff; str[8]=0x00; str[9]=0x00; str[10]=0x00;
  len=9;
  Serial.println(iout);
  str[7]=iout++;
  if(iout>15)
    iout=0;
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
