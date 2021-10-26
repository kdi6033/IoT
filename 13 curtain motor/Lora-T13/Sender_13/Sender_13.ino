#include <ArduinoJson.h>
#include "CRC.h"
#include <SoftwareSerial.h>
#include "heltec.h"

#define BAND    923E6 
#define TRIGGER_PIN 13 // trigger pin 13
SoftwareSerial mySerial(23,17);

String inputString = "";     

// 정리
int Out[8]={0},In[10]={0};  // plc 입력과 출력 저장 
int noOut=0,valueOut=0;
int orderCurtain=0,orderCurtainPre=0; //0=stop, 1=up, 2=down  plc입력

String orderCurtainWeb=""; //0=stop, 1=up, 2=down  cpu의 web

int outPlc=0;
String orderCurtainNow,orderCurtainWebPre;
int OutLed[3]={0};

unsigned int countTick=0;
unsigned long previousMillis = 0;     

String outgoing;              // outgoing message
long lastSendTime = 0;        // last send time
const long interval = 2000;  

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
  actCurtain();
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

// 정리
void sendData() {
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(chipidS);
  LoRa.print("|");
  LoRa.print(orderCurtainNow);
  LoRa.print("|"); 
  for(int i=0;i<=7;i++){ 
  LoRa.print(In[i]);
  LoRa.print("|");
}
  LoRa.endPacket();
}

// 선택
void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 0, "Mac: "+chipidS);
    Heltec.display->drawString(0, 8, "=================");
    Heltec.display->drawString(0, 15, "0:stop | 1:up | 2:down");
    Heltec.display->drawString(0, 25, "State: "+orderCurtainWeb);
    String sIn="";
    for(int i=0; i<7; i++) sIn +=String(In[i])+String(" ");
    Heltec.display->drawString(0, 32, "=================");
    Heltec.display->drawString(0, 40, "In: "+String(sIn)); // 정지 올림 내림 표시 up... down....

    Heltec.display->display();
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
}

void loop() {
  serialEvent();
  doTick();
  if ( digitalRead(TRIGGER_PIN) == LOW ) 
   factoryDefault();  
  //패킷을 구문 분석하고 onReceive를 호출합니다.
  onReceive(LoRa.parsePacket());  
}

// 받아온 데이터결과 출력
void onReceive(int packetSize)
{
  if (packetSize == 0) return;          // 받은값이 없다면 아무것도 X
  if (packetSize) {     
      //read packet
      while (LoRa.available()) {
      orderCurtainWeb = LoRa.readStringUntil('|');
    } 
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
  if(outPlc!=1 && inputString.length() >= 6 && inputString.charAt(1)==0x02) {
    int b=1;
    for(int i=1;i<=8;i++) {
        int c=inputString.charAt(3)&b;
        if(c!=0)
          c=0x01; //불킨다
        In[i-1]=c;
        // 1 1 1 1 1 0 1 이런 느낌
        Serial.print(c,HEX);
        Serial.print(" ");
        b*=2;
    }
    switchCurtain();
    inputString="";
    Serial.println("");
  }
}
// 릴레이 코드 : 스위치 In[0]==1 인경우 
void switchCurtain() {
  if(In[0]==1)
    orderCurtain=1;
  else if(In[1]==1) 
    orderCurtain=2;
  else 
    orderCurtain=0;
}

//릴레이 코드 : 실제 동작
void actCurtain() {
  if(orderCurtainWeb != orderCurtainWebPre) {
    orderCurtainWebPre = orderCurtainWeb;
    //Serial.println("web======================");
    //Serial.println(orderCurtainWeb);
    if(orderCurtainWeb=="1") {
      Out[0]=1;
      Out[1]=0;
    }
    else if(orderCurtainWeb=="2") {
      Out[0]=0;
      Out[1]=1;
    }
    else {
      Out[0]=0;
      Out[1]=0;
    }
    outPlc = 1;
    for(int i=0;i<2;i++) {
      outPlc = 1;
      noOut=i;
      crd16Rtu();
      delay(200);
    };
    if(orderCurtainWeb!="0")
      orderCurtainNow=orderCurtainWeb;
  }
  // 릴레이 코드 : 스위치 기동시, 입력 접점시 들어오는 값 
  if(orderCurtain != orderCurtainPre) {
    orderCurtainPre = orderCurtain;
    if(orderCurtain==1) {
      Out[0]=1;
      Out[1]=0;
    }
    else if(orderCurtain==2) {
      Out[0]=0;
      Out[1]=1;
    }
    else {
      Out[0]=0;
      Out[1]=0;
    }
    outPlc = 1; // 상태 확인 프로토콜
    // 3개 버튼의 상태를 확인하고 in led를 점등한다.
    for(int i=0;i<2;i++) {
      outPlc = 1; // 상태 확인 프로토콜 실행
      noOut=i;
      crd16Rtu();
      delay(200);
    }
    if(orderCurtain!=0)
      orderCurtainNow=orderCurtain;
  }
}
// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  String s;
  int si,sj,len;
  char str[24];

  if(outPlc == 1) {  //출력
    //LS str[24] =  {0x00,0x0f,0x00,0x00,0x00,0x0a,0x02,0xff,0x00,0x00,0x00};  //비트연속출력 len=9
    //str[24]=[0xff,0x05,0x00,0x02,0xff,0x00,0x00,0x00]; //비트출력 len=6
    str[0]=0xff; str[1]=0x05; str[2]=0x00; str[3]=0x02; str[4]=0xff;
    str[5]=0x00; str[6]=0x00; str[7]=0x00; 
    len=6;
    str[3]=noOut; 
    if(Out[noOut]==1)
      str[4]=0xff;
    else
      str[4]=0x0;
    outPlc=0;
  }
  else {    //입력
    //str[24]=[0xff,0x02,0x00,0x00,0x00,0x08,0x00,0x00]; // 비트 입력영역 읽기 len=6
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
