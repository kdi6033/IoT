#include <SoftwareSerial.h>
#include "CRC.h"

SoftwareSerial mySerial(D7, D4); // RX, TX

int outPlc=0;
unsigned long previousMillis = 0;   
const long interval = 3000; 
String inputString = "";         // 받은 문자열
int Out[8]={0,1,0,0},In[10]={0};  // plc 입력과 출력 저장 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("start");
}

void tick()
{
  crd16Rtu();
}

void loop() {
  // put your main code here, to run repeatedly:
  doTick();
  serialEvent();
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    tick();
  }  
}

void serialEvent() {
  if(mySerial.available() == false)
    return;
  while (mySerial.available()) {
    // get the new byte:
    char inChar = (char)mySerial.read();
    //Serial.print(inChar,HEX);
    // add it to the inputString:
    inputString += inChar;
  }
  //Serial.println("");

  if(outPlc!=1 && inputString.length() >= 6) {
    int b=1;
    for(int i=1;i<=8;i++) {
        int c=inputString.charAt(3)&b;
        if(c!=0)
          c=0x01;
        In[i-1]=c;
        Serial.print(c,HEX);
        Serial.print(" ");
        b*=2;
      }
    inputString="";
    Serial.println("");
  }
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  String s;
  int si,sj,len;
  char str[24];

  if(outPlc == 1) {  //출력
    str[0]=0xff; str[1]=0x05; str[2]=0x00; str[3]=0x03; str[4]=0xff;
    str[5]=0x00; str[6]=0x00; str[7]=0x00; 
    len=6;
    //outPlc=0;
  }
  else {    //입력상태읽기
    str[0]=0xff; str[1]=0x02; str[2]=0x00; str[3]=0x00; str[4]=0x00;
    str[5]=0x08; str[6]=0x00; str[7]=0x00; 
    len=6;
  }

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
