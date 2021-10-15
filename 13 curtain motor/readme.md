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
