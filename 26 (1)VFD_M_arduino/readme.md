# [Delta] VFD_M 인버터 제어
![Arduino+VFD인버터-001](https://user-images.githubusercontent.com/98401825/169675229-c581c743-e5a9-4889-a1e8-ef32160bacc6.png)

### -----VFD_M 인버터 파라미터 설정-----   
![VFD_M인버터 메뉴얼캡쳐](https://user-images.githubusercontent.com/98401825/169675482-ae5ca6de-cd8d-4f58-bd19-8c665a3e0840.jpg)

  
* 메뉴얼 참고해서 RS485관련 파라미터 설정 필요
* 메뉴얼 링크 : <http://dycop.com/pdf/delta-vfd-m-manual.pdf>  

### VFD_M 인버터 프로토콜 분석
![VFD_M인버터 메뉴얼캡쳐2](https://user-images.githubusercontent.com/98401825/169675388-3ef54554-b947-47bd-85b6-9bf567046d9a.jpg)
  
* 정회전 : FWD RUN 이므로 2000번지의 0001 0010 = 18(DEC) = 0x12(HEX)  
* 역회전 : REV RUN 이므로 2000번지의 0010 0010 = 34(DEC) = 0x22(HEX)    
* 정지 : 기능없음 STOP 이므로 2000번지의 0000 0001 = 1(DEC) = 0x01(HEX)  
