# ✅ Physical AI & Device Control
산업 현장에서 사용되는 ESP32 기반 Edge Controller 계층으로, Wi-Fi·Bluetooth·RS-485·MQTT 통신을 통해 각종 엣지 컴퓨터, 컨트롤러, 외부 시스템과 연결되며 On-Device AI 판단 결과를 현장 제어로 전달

## 🔷i2r 제품

| i2r-01<br>IoT 통신 | i2r-05<br>AI IoT 아두이노 |  |  |
| :---: | :---: | :---: | :---: |
| <img src="https://github.com/kdi6033/i2r-01/raw/main/images/i2r01-kc.png?raw=true" height="140"> | <img src="https://github.com/kdi6033/i2r-05/raw/main/images/i2r-05-kc.png?raw=true?raw=true" height="140"> |  |  |
| <img src="https://github.com/kdi6033/i2r-01/raw/main/images/case-i2r01-iso.png?raw=true" height="120"> | <img src="https://github.com/kdi6033/i2r-05/raw/main/images/i2r05-2.png?raw=true?raw=true" height="120"> |  | |
| <img src="https://github.com/kdi6033/i2r-01/raw/main/images/case-i2r01-kc.png?raw=true" height="140"> | <img src="https://github.com/kdi6033/i2r-05/raw/main/images/i2r05-1.png?raw=true?raw=true" height="140"> |  |  |

## 🔷회로도
| i2r-01 | i2r-05 |  |  |
| :---: | :---: | :---: | :---: |
| <img src="https://github.com/kdi6033/i2r-01/raw/main/images/i2r-01%20pin.png?raw=true?raw=true?raw=true" height="140"> | <img src="https://github.com/kdi6033/i2r-05/raw/main/images/i2r-05-circuit.png?raw=true?raw=true" height="140"> |  |  |
| <img src="https://github.com/kdi6033/i2r-01/raw/main/images/ESP32-wroom-32.png?raw=true?raw=true?raw=true?raw=true" height="140"> | <img src="https://github.com/kdi6033/i2r-05/raw/main/images/i2r-05-circuit.png?raw=true?raw=true" height="140"> |  |  |

---

[김동일교수 유튜브보기](http://i2r.link)

- 센서와 액튜에이터를 연결하기만 하면 프로그램 하지 않아도 자동으로 모니터링과 콘트롤 할 수 있는 방법을 소개하며 모든 소스 프로그램은 여기 사이트에 공개 합니다.       
- 센서 및 액투에이터는 아두이노로 프로그램 하였으며 와이파이 무선으로 연결 됩니다.    
- 서버는 3가지 방법으로 구성할 수 있습니다.     

### 1. 로컬서버 프로그램 
- 같은 AP 즉 무선공유기에 연결되어 있는 기기들을 모니터링/제어 합니다. 
- 인터넷은 연결되어 있지 않아도 되며 AP에서 DHCP로 할당되는 주소를 사용 함으로 192.168.*.* 을 사용 함으로 이론상으로 256 * 256 개의 기기를 연결할수 있습니다. 
- 시스템을 구성하기 위해서는 아래 3가지 프로그램을 설치해야 합니다.
- 아두이노 프로그램은 down-local-* 을 다운로드 받는다. [유튜브보기](https://www.youtube.com/watch?v=8n9ETHVoJKM)
- [node-red](https://youtu.be/nL3qdDtZC98)
- [모스키토](https://youtu.be/27mBMakaPuY)
- [mongoDB](https://youtu.be/ltLJx28tDOs)
- [node red 로컬서버 소스프로그램설치](https://github.com/kdi6033/IoT/tree/main/1-2%20Node-red%20Local%20Server)    

### 2. 아마존크라우드(AWS) 서버프로그램 (다수의 사용자)    
- 아마존 크라우드에서 동작하여 인터넷을 이용하여 세계 어느 곳에서든지 사용할 수 있다.    
- 이메일 ID로 등록하여 여러 사람이 이용 할 수 있으므로 상업용으로 운용 할 수 있다.
- 프로그램이 사용자 ID와 데이터베이스가 함계 운용되어 복잡하다.
- 아마존 크라우드 https://i2r.link/ 에 접속하여 상단 메뉴에서 "아마존크라우드 IoT"를 선택하면 프로그램 하지 않아도 시스템을 구성할 수 있다.
- [아마존크라우드 IoT 접속](http://18.237.189.188:1880/login)    
- 아두이노 프로그램은 down-aws-* 을 다운로드 받는다. [유튜브보기](https://www.youtube.com/watch?v=8n9ETHVoJKM)

### 3. 아마존크라우드(AWS) 서버프로그램 (개인 사용자) 
- 개인이 아마존크라우드에서 node-red 불러오기를 이용해 서버를 구성해 프로그램 하지 않아도 시스템을 만들 수 있다.
- 아두이노 프로그램은 down-awsLight-* 을 다운로드 받는다. [유튜브보기](https://www.youtube.com/watch?v=8n9ETHVoJKM)
- 아래 순서로 아마존 크라우드 서버를 만든다.
- [EC2 인스턴스 설치](https://youtu.be/voWEBY1nbb8)
- [인바운드 설정](https://youtu.be/GPHIZjJIsFA)
- [IoT Core 설치](https://youtu.be/0PnrpBVUjJQ)
- [Node-RED 설치](https://youtu.be/7CwXjkSA6HE)
- [MongoDB 설치](https://youtu.be/jbly92YECBw)
- [node red 아마존서버 소스프로그램설치](https://github.com/kdi6033/IoT/tree/main/1-2-3%20Node-red%20AWS%20Light%20Server)

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
|11|진동|<img src = "https://user-images.githubusercontent.com/37902752/145346513-43006c07-8ecb-42e2-a3f4-14ee53bafe0d.png" width="100" height="100">|down-local-11<br> down-aws-11|진동|-|-
|12|PLC-RS485|<img src = "https://user-images.githubusercontent.com/37902752/145347419-4aade2de-7b22-446d-bc12-e0cf0fa038c3.png" width="100" height="100">|down-local-12<br> down-aws-12|PLC 무선제어|[메뉴얼](https://github.com/kdi6033/IoT/tree/main/12%20PLC-RS485)|- 
|13|curtain motor|<img src = "https://user-images.githubusercontent.com/37902752/145349384-d39e215e-3fc9-4232-bf5d-cf07ce92ed65.png" width="100" height="100">|down-local-13<br> down-aws-13|모터 무선제어|-|[우성하이텍](http://www.wsh.co.kr/ventcontrol2.html) 
|14|MASTER-K120s|<img src = "https://user-images.githubusercontent.com/37902752/145350474-417c2ed5-93d7-4398-a3b6-badb45772f5a.png" width="100" height="100">|down-local-14<br> down-aws-14|PLC입출력|-|[LS산전](https://www.ls-electric.com/ko/product/category/CCC001)

