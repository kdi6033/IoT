#WiFi RS-485 아두이노 보드로 센서값 받기
---

## 회로도
### !KSM8900 어뎁터는 5V입니다. 5V이상 전류가 흐를시 센서가 고장납니다.!
![sensordata](https://user-images.githubusercontent.com/59568377/125215103-5708ee00-e2f5-11eb-8b56-010cafa7767d.jpg)

### KSM900 이아닌 다른 센서 일 경우

 코드 내용중 3가지를 변경 하셔야 합니다.
 
 1. 통신 프로토콜 변경
 ```
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
```
1,4,0,7,0,3,0,0 부분을 변경 (메뉴얼에는 01 04 07 00 03 97 C9 이런식으로 표현돼있는데 마지막 뒷 부분 97 C9은 CRC부분이라 0으로 놔둬도 상관없습니다.)
 
 2. 센서값 변경 함수
 ```
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
 ```
 40~63줄 받는 값 길이에 따라 `inputString.length() >= 11` 의 11의 숫자 변경(inputstring이 inchar의 값이 누적되어 생성 되므로 11이라는 조건이 없으면
 센서값이 다 들어오기 전에 값을 출력하게 되면 이상한 값이 나오게 됨) 을 해주셔야 합니다.
 
 프로토콜이 온도라고 쳐서 3884 가 나오고 이를 10진수 변환하여 100을 나누어야 한다는것이 매뉴얼에 나와있으면
 sT(sensorTemp)를 참고하여 보면  16진수를 10진수로 변환하여 둘이 더한뒤 100으로 나눈다는 표현을 한 부분이
 `float sT=((float)inputString.charAt(5)*256+(float)inputString.charAt(6))/100;` 이부분이 된다.
 3. BoadRate(보드레이트) 속도
 ```
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  ticker.attach(3, tick);
}
```
SensorDataRead.ino 스캐치의 19번째 줄에 보드레이트란이 있는데 KSM-8900은 115200
