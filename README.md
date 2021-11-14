# IoT 센서 모음    
[김동일교수 유튜브보기](http://i2r.link)

- 센서와 액튜에이터를 연결하기만 하면 프로그램 하지 않아도 자동으로 모니터링과 콘트롤 할 수 있는 방법을 소개하며 모든 소스 프로그램은 여기 사이트에 공개 합니다.       
- 센서 및 액투에이터는 아두이노로 프로그램 하였으며 와이파이 무선으로 연결 됩니다.    
- 서버는 3가지 방법으로 구성할 수 있습니다.     

### 1. 로컬서버 프로그램 
- 같은 AP 즉 무선공유기에 연결되어 있는 기기들을 모니터링/제어 합니다. 
- 인터넷은 연결되어 있지 않아도 되며 AP에서 DHCP로 할당되는 주소를 사용 함으로 192.168.*.* 을 사용 함으로 이론상으로 256 * 256 개의 기기를 연결할수 있습니다. 
- 시스템을 구성하기 위해서는 아래 3가지 프로그램을 설치해야 합니다.
- node-red
- 모스키토
- mongoDB



|Type|이름|<center>사진</center>|<center>파일이름</center>|<center>측정값</center>|<center>메뉴얼</center>|회사이름
|----|----|---------------:|:--------------------|:---------------------:|:---------------------:|:---------------------:
|2|PE-300|<img src = "https://user-images.githubusercontent.com/37902752/129737600-b4768e5a-3e48-475f-8af8-5935a2e66d8b.jpg" width="100" height="100">|down-local-02<br>down-aws-02|온도,PH,EC|[PE-300](https://github.com/kdi6033/IoT/blob/main/0%20manual/pe-300.pdf)|[FARMSCUBE](https://farmscube.kr/product.html)
|3|키우미|<img src = "https://user-images.githubusercontent.com/37902752/130728221-0a7e2b79-6d23-4f7c-b453-41bd6105d155.jpg" width="100" height="100">|down-local-03<br>down-aws-03|온도,습도,CO2|[키우미](https://github.com/kdi6033/IoT/blob/main/0%20manual/3.ECO-FARM_V1.pdf)|[Allsensing](https://allsensing.com/)
|4|KSM-8900|<img src = "https://user-images.githubusercontent.com/37902752/129312387-896a2530-ea5f-4e74-9941-f8335003f127.jpg" width="100" height="100">|down-local-04<br>down-aws-04|토양의 습도, 온도, EC|[ksm8900](https://github.com/kdi6033/IoT/tree/main/11-4%20%5Bsensecube%5D%20KSM-8900)|[FARMSCUBE](https://farmscube.kr/product.html)
|5|simpleTH|<img src = "https://user-images.githubusercontent.com/37902752/129312389-093260ec-f3b9-4373-a7c6-abfd97ce2179.png" width="100" height="100">|down-local-05<br> down-aws-05|습도, 온도|[simpleTH](https://github.com/kdi6033/IoT/tree/main/11-5%20%5Ballsensing%5D%20temperature%20humidity%20sensor)|[Allsensing](https://allsensing.com/)
|6|XEC-DR14E|<img src = "https://user-images.githubusercontent.com/37902752/129728737-3dff933d-2746-4986-aca9-dc25efcef0d2.jpg" width="100" height="100">|down-local-06<br> down-aws-06|PLC입출력|[메뉴얼](https://github.com/kdi6033/plc/blob/master/LS%EC%82%B0%EC%A0%84%EC%9E%90%EB%A3%8C/XEC_MANUAL.pdf)<br>[Cnet통신](https://github.com/kdi6033/plc/blob/master/LS%EC%82%B0%EC%A0%84%EC%9E%90%EB%A3%8C/%EC%82%AC%EC%9A%A9%EC%84%A4%EB%AA%85%EC%84%9C_XGB%20Cnet_V1.8.pdf)<br>[Fnet통신](https://github.com/kdi6033/plc/blob/master/LS%EC%82%B0%EC%A0%84%EC%9E%90%EB%A3%8C/%EC%82%AC%EC%9A%A9%EC%84%A4%EB%AA%85%EC%84%9C_XGB%20FEnet_%EA%B5%AD%EB%AC%B8_V1.5.pdf)<br>[XG500 사용법](https://github.com/kdi6033/plc/blob/master/LS%EC%82%B0%EC%A0%84%EC%9E%90%EB%A3%8C/XG5000_Manual_V2.8_202005_KR.pdf)|[LS산전](https://www.ls-electric.com/ko/product/category/CCC001)
|7|DHT11 Shield|<img src = "https://user-images.githubusercontent.com/37902752/129735447-3c72f093-cb0d-4991-904a-5bc682ee2532.jpg" width="100" height="100">|down-local-07<br>down-aws-07|습도, 온도|[메뉴얼](https://www.wemos.cc/en/latest/d1_mini_shield/dht.html#features)|[WEMOS](https://www.wemos.cc/en/latest/index.html)
|8|RY-WS301|<img src = "https://user-images.githubusercontent.com/37902752/129312381-0a46e6b7-17c5-4e57-bcbe-e59221eb55bd.jpg" width="100" height="100">|down-local-08<br> down-aws-08|습도, 온도|[RY-WS301](https://github.com/kdi6033/IoT/blob/main/0%20manual/RY-WS301.pdf)|효림솔루션<br>[Handan Yunnong](https://en.nong-iot.com/)
|9|MP-508E|<img src = "https://user-images.githubusercontent.com/37902752/129312385-053a8389-4ecc-497d-864c-6427dfcfae8e.png" width="100" height="100">|down-local-09<br> down-aws-09|토양의 온도, 습도, 염도|[MP-508E](https://github.com/kdi6033/IoT/blob/main/0%20manual/MP-508El.pdf)|효림솔루션<br>[Handan Yunnong](https://en.nong-iot.com/)
|10|sound|<img src = "https://user-images.githubusercontent.com/37902752/133548055-3d3c8029-809a-4185-ae31-194b7422e0dc.png" width="100" height="50">|down-local-10<br> down-aws-10|소음,소리크기|-|-
