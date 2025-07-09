# 모터 및 자동 개폐기 프로그램 개발 가이드   
## 제품 개요   
WSM-4035는 전동 스크류 드라이버로 스마트팜 농가에서 사용하기 적합한 자동 개폐기 장치입니다. 이 장치는 효율적인 작업 수행을 위해 정밀한 토크 제어와 빠른 반응 속도를 제공합니다.   
[우성하이텍 WSM-4035 제품링크](http://www.wsh.co.kr/ventcontrol2.html)  

## 제품 사양
- 모델명: WSM-4035  
- 구동방식: 유압식  
- 토크 제어: 알루미늄 합금  
- 출력 회전수: 4.2rpm  
- 전류: 2.6A  
- 토크: 최대 4kg·m  
- 무게: 2.4kg  
- 크기: 218 x 143 x 104mm   
![13 회로도](https://github.com/kdi6033/IoT/assets/37902752/23d57982-7c7e-4109-8350-dbc4409fbb20)

![13 회로도-i2r-04](https://github.com/user-attachments/assets/fcd6b202-b735-4339-99d3-9d58d5c9fae1)


## 배선연결  
- WSM-4035는 24V로 동작 합니다. 24V + - 를 기기의 배선에 연결하면 정회전 하고 반대로 연결하면 역회전 합니다. 배선에 두개 모두 0V를 연결하면 정지 합니다. 그러므로 i2r-03 보드의 릴레이 두개를 가지고 이러한 조합을 만들어 갑니다. 그림과 같이 배선을 연결합니다.

## 통신 프로토콜
i2r-03,04 보드의 프로토콜을 그대로 사용하고 내부 프로그램에서 동시에 on 이 되면 합선이 됨으로 두개의 릴레이를 off 시키고 선택된 릴레이를 on 시킴으로 합선을 방지하고 좌회전 우회전 제어를 합니다.

## 프로그램 설명
- 프로그램은 WSM-4035 모델을 이용하여 스마트팜 농가의 자동 개폐 시스템을 원격 제어합니다.
- WSM-4035 작동원리를 설명하고 프로그램 합니다.  
- 스마트폰에서 블루투스나 mqtt 통신을 이용하여 원격으로 제어하는 프로그램을 합니다.  
- [i2r.link AI&IoT](https://i2r.link) 사이트를 접속하면 자동으로 홈페이지가 만들어지고 여러개의 기기를 원격으로 제어 할 수 있습니다.
- 프로그램은 i2r-03,04 보드의 프로그램을 그대로 사용하고 제어보드의 아두이노 프로그램에서 동시에 on을 방지하는 프로그램만 추가 했습니다.
- 현장에서 동작을 하기 위해서 rp2040 을 사용하여 터치판넬을 부착하여 현장에서 바로 동작을 시킬 수 있습니다.

```
for (int i = 0; i < 4; i++) {
      int currentState = digitalRead(inputPins[i]); // 현재 핀 상태 읽기
      // 현재 핀 상태가 이전 상태와 다른 경우 상태 변경 플래그를 true로 설정
      if (currentState != dev.in[i]) {
        dev.in[i] = currentState; // 상태 업데이트
        stateChanged = true; // 상태 변경 플래그 설정
        actMachine(i);
      }
    }
```
모트를 정회전, 역회전, 정지를 실행 한다.
```
void actMachine(int noSelect) {
  dev.noSelect = noSelect;
  if(noSelect==0) {
    digitalWrite(outputPins[0], LOW);
    delay(100);
    digitalWrite(outputPins[1], LOW);
    digitalWrite(outputPins[0], HIGH);
    Serial.println("Up");
  }
  else if(noSelect==1) {
    digitalWrite(outputPins[0], LOW);
    delay(100);
    digitalWrite(outputPins[1], HIGH);
    Serial.println("Down");
  }
  else {
    digitalWrite(outputPins[0], LOW);
    digitalWrite(outputPins[1], LOW);
    Serial.println("Stop");
  }
  publishMqtt();
}
```

[i2r-13보드 아두이노 소스프로그램](https://github.com/kdi6033/IoT/blob/main/13%20curtain%20motor/0%20arduino%20app%20program/board-i2r-13/board-i2r-13.ino)    
[i2r-13 App 프로그램](https://github.com/kdi6033/IoT/tree/main/13%20curtain%20motor/0%20arduino%20app%20program/app)

---------------------------------  
다은은 이전에 ESP D1mini로 개발했던 내용으로 지우지 않고 있으니 필요하신 분은 참조하도록 하세요.  
본릴레이 보드는 처음 나올때 국번(address)가 01로 나와있어 기존에 작성했던 프로그램을 그대로 사용하기 위해 
국번 변경 프로토콜을 이용하여 국번을 255로 변경하여 기존에 프로그램 업로드

9th (FF)에 있는 숫자에 따라 국번 변경 프로토콜
```
//00 10 00 00 00 01 02 00 _FF_ EB 80
//밑은 node-red function code
var buf = Buffer.alloc(9);
buf=[0x00,0x10,0x00,0x00,0x00,0x01,0x02,0x00,0xff,0x00,0x00];
msg.payload=buf;
return msg;
```
배선도
* 주의 TRX+ TRX- 선을 A+ B-랑 잘못 연결하면 릴레이의 485보드가 고장납니다.
![T13Motor](https://user-images.githubusercontent.com/37902752/137422739-71364d93-68f6-4799-9f55-e9dc4b9ff8a6.jpg)
