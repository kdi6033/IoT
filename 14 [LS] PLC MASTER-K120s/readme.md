## MASTER-K120s
### 1. 프로토콜 확인 
PLC 번호는 0x01로 프로그램, 출력에서 0은 0V 1은 24V 입니다.   
[참조메뉴얼] (https://github.com/kdi6033/plc/blob/master/LS%EC%82%B0%EC%A0%84%EC%9E%90%EB%A3%8C/%EC%82%AC%EC%9A%A9%EC%84%A4%EB%AA%85%EC%84%9C_XGB%20Cnet_V1.8.pdf,"sss")    
|기능|보냄|설명|
|----|----|-------|
|비트연속출력|01 0F 00 40 00 0A 02 01 * *| out 1000 으로 출력 |
|비트연속출력|01 0F 00 40 00 0A 02 02 * *| out 0100 으로 출력 |
|비트연속출력|01 0F 00 40 00 0A 0F 02 * *| out 1111 으로 출력 |
|비트 입력영역 읽기|01 02 00 00 00 08 * *| 입력값이 리턴문자열의 4번째|

[Google](https://google.com, "google link")
