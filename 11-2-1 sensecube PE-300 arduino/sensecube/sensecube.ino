// 참조유튜브 : https://youtu.be/JHngW6Gf4sU
// crc16 rtu를 사용하기 위해 다음 라이브러리 설치하세요.
// crc16 modbus
//  https://github.com/RobTillaart/CRC
//  AUTHOR: Rob Tillaart

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "CRC.h"
#include <SoftwareSerial.h>
Ticker ticker;
SoftwareSerial mySerial(D7, D4); // RX, TX

const char* ssid     = "i2r";
const char* password = "00000000";
String inputString = "";         // 받은 문자열
int counter=0;

void tick()
{
  Serial.println ( WiFi.localIP() );
  counter++;
  crd16Rtu();
}

void setup() {
  Serial.begin(38400);
  mySerial.begin(38400);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ticker.attach(3, tick);  //0.1 초도 가능
  //ticker.detach();
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
    String ss="";
    ss=((float)inputString.charAt(3)*255+(float)inputString.charAt(4));
    Serial.println("EC : "+ss);
    ss=((float)inputString.charAt(5)*255+(float)inputString.charAt(6))/100;
    Serial.println("PH : "+ss);
    ss=((float)inputString.charAt(7)*255+(float)inputString.charAt(8))/10;
    Serial.println("온도 : "+ss);
    inputString="";
    Serial.println("");

  }
}

// 아두이노에서 RS485 출력을 내보낸다.
void crd16Rtu() {
  char str[24] =  {0x1f,0x04,0x00,0x01,0x00,0x03,0x00,0x00,0x00};  //[31,4,0,1,0,3,0,0]
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
