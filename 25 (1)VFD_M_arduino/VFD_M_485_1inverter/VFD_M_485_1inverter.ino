#include <SoftwareSerial.h>
#include "CRC.h"

SoftwareSerial mySerial(D7, D5); // RX, TX 
//송신은 Serial1 수신은 mySerial로 하기 때문에 mySerial의 TX는 필요없음(임의로 D5지정)

unsigned long previousMillis = 0;   
const long interval = 3000; 
String inputString = "";         // 받은 문자열
char str[24];
String s;
int si,sj,len=6;

// 0:정지 1:fwd 2:rev
void Direction(int dir) {
  str[0]=0x01; str[1]=0x06; str[2]=0x20; str[3]=0x00; str[4]=0x00; str[5]=0x12;
  //str[6]=0x02;str[7]=0x07;
  if(dir==0)
    str[5]=0x01;
  else if(dir==1)
    str[5]=0x12;
  else if(dir==2)
    str[5]=0x22;
  crd16Rtu();
}

void velocity(int vel) {
  str[0]=0x01; str[1]=0x06; str[2]=0x20; str[3]=0x01; str[4]=0x17; str[5]=0x00;
  //str[6]=0xD3; str[7]=0xCA;
  uint8_t * data = (uint8_t *) &str[0];
  str[5]=vel&0xff;
  int sj=vel>>8;
  str[4]=sj&0xff;
  crd16Rtu();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600,SERIAL_8N2);
  Serial1.begin(9600,SERIAL_8N2);
  mySerial.begin(9600);
  Serial.println("start");
}

void loop() {
  Direction(1);//정회전
  delay(1000);
  velocity(5000);
  delay(5000);

  Direction(0);//정지
  delay(5000);
  
  Direction(2);//역회전
  delay(1000);
  velocity(5000);
  delay(5000);
  
  Direction(0);//정지
  delay(5000);
}


// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  //len=6;
  inputString = "";
  uint8_t * data = (uint8_t *) &str[0];
  si=crc16(data, len, 0x8005, 0xFFFF, 0x0000, true,  true  );
  sj=si&0xff;
  str[len]=sj;
  sj=si>>8;
  str[len+1]=sj;
  
  for(int i=0;i<len+2;i++)
    Serial1.print(str[i]);
}
