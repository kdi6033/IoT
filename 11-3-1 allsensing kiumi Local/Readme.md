# KIUMI Local mqtt 

![016](https://user-images.githubusercontent.com/83691399/124417807-f4639f80-dd94-11eb-9a02-b59e1b55a9aa.png)

# Local mqtt 통신하는 방법

## 키우미센서 프로토콜
>
보드레이트 : 9600   
문자데이타로 입력하세요   
주소확인 :FFFB0000000006\r\n   
온도요청 :010400010001F9\r\n   
습도요청 :010400020001F8\r\n   
CO2요청 :010400030001F7\r\n   
온도,습도요청 :010400010002F8\r\n   
습도,CO2요청 :010400020002F7\r\n   
온도,습도,CO2요청 :010400010003F7\r\n   

## 배선도
![1111](https://user-images.githubusercontent.com/37902752/130716248-827ba40e-9641-435c-b018-999aec72e095.png)


* Local mqtt 통신 node-red flow 
```
[{"id":"ace6987b.29b6f8","type":"tab","label":"플로우 1","disabled":false,"info":""},{"id":"d9990130.6b6","type":"mqtt out","z":"ace6987b.29b6f8","name":"Kium-Local mqtt","topic":"/doowon-Kium/inTopic","qos":"0","retain":"","broker":"c84ecd4a.7ca0e","x":620,"y":420,"wires":[]},{"id":"26812c2.15605d4","type":"mqtt in","z":"ace6987b.29b6f8","name":"Kium-Local mqtt","topic":"/Kium-Local mqtt/outTopic","qos":"0","datatype":"auto","broker":"c84ecd4a.7ca0e","x":430,"y":480,"wires":[["c634888f.244468","e083594b.5ba838"]]},{"id":"8329e6b2.2e0788","type":"inject","z":"ace6987b.29b6f8","name":"","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":"테스트 메세지","payloadType":"str","x":430,"y":420,"wires":[["d9990130.6b6"]]},{"id":"c634888f.244468","type":"debug","z":"ace6987b.29b6f8","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":730,"y":480,"wires":[]},{"id":"e083594b.5ba838","type":"json","z":"ace6987b.29b6f8","name":"","property":"payload","action":"","pretty":false,"x":670,"y":540,"wires":[["bef522ec.df231"]]},{"id":"bef522ec.df231","type":"function","z":"ace6987b.29b6f8","name":"변환","func":"var TEMMsg={};\nvar HUMMsg={};\nvar COMsg={};\n\nTEMMsg.payload=msg.payload.TEM;\nHUMMsg.payload=msg.payload.HUM;\nCOMsg.payload=msg.payload.CO;\n\nreturn [TEMMsg,HUMMsg,COMsg];\n","outputs":3,"noerr":0,"initialize":"","finalize":"","x":810,"y":540,"wires":[["82fa0a25.e5e418"],["76ddea7a.91cfc4"],["c9239348.a37e1"]]},{"id":"c9239348.a37e1","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":2,"width":"3","height":"3","gtype":"wave","title":"CO2","label":"ph","format":"{{value}}","min":"400","max":"3000","colors":["#00b500","#e6e600","#ca3838"],"seg1":"600","seg2":"800","x":1030,"y":580,"wires":[]},{"id":"82fa0a25.e5e418","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":0,"width":"3","height":"3","gtype":"donut","title":"온도","label":"℃","format":"{{value}}","min":"-20","max":"70","colors":["#50e2e0","#4bf00f","#ca3838"],"seg1":"20","seg2":"35","x":1030,"y":460,"wires":[]},{"id":"76ddea7a.91cfc4","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":1,"width":"3","height":"3","gtype":"wave","title":"습도","label":"%","format":"{{value}}","min":0,"max":"100","colors":["#00b500","#e6e600","#ca3838"],"seg1":"40","seg2":"80","x":1030,"y":520,"wires":[]},{"id":"c84ecd4a.7ca0e","type":"mqtt-broker","name":"","broker":"192.168.0.67","port":"1883","clientid":"","usetls":false,"compatmode":false,"keepalive":"60","cleansession":true,"birthTopic":"","birthQos":"0","birthPayload":"","closeTopic":"","closeQos":"0","closePayload":"","willTopic":"","willQos":"0","willPayload":""},{"id":"199b0928.389a87","type":"ui_group","name":"키우미","tab":"5eec05a4.959bac","order":1,"disp":true,"width":"6","collapse":false},{"id":"5eec05a4.959bac","type":"ui_tab","name":"키우미","icon":"dashboard","order":3,"disabled":false,"hidden":false}]
```
### 1. WiFi 연결
![image](https://user-images.githubusercontent.com/83691399/126777406-46c9ff77-3d1c-4bc0-aa98-ae60ed82e6ae.png)
구매한 RS-485 보드를 컴퓨터에 연결하고 WiFi칸을 보면 i2r-"영어 숫자 랜덤이 나타나는데 클릭
### 2. 주소 접속
![KakaoTalk_20210723_2041494150](https://user-images.githubusercontent.com/83691399/126777070-fc1cf060-c942-4574-a215-7b38ad262e33.jpg)

i2r-%% wifi를 연결해주면 OLED에 '192.168.4.1'이 나타나는데 i2r wifi가 `연결된 상태`에서 주소창에 입력
### 3. 홈페이지 진입
![image](https://user-images.githubusercontent.com/83691399/126778330-63100784-5e85-4be9-8489-14bb139bf9d1.png)

홈페이지에 들어온 후에 좌측 상단 三를 선택
### 4. 와이파이 설정-1 단계
![KakaoTalk_20210723_205310010_01](https://user-images.githubusercontent.com/83691399/126780079-07840cfc-1a93-4bbf-b13b-2e3110ca2b15.jpg)
메뉴에 들어와 와이파이 설정 클릭

### 5. 와이파이 설정-2 단계
![KakaoTalk_20210723_205310010_02](https://user-images.githubusercontent.com/83691399/126780123-add4d5f1-66d0-4bf1-bbca-1416d6a26fd2.jpg)

와이파이 이름과 비밀번호에 주변에서 사용하고 있는 와이파이를 입력
### 6. 와이파이 설정-3 단계
![KakaoTalk_20210723_205310010_03](https://user-images.githubusercontent.com/83691399/126780154-b60a4038-f13b-4ac4-8d3a-ca85a8699053.jpg)
와이파이를 설정해주면 홈페이지 연결이 끊기는데 당연한거라 괜찮습니다. 그 뒤 컴퓨터 wifi를 칸에 입력했던 와이파이를 연결

### 7. 홈페이지 연결
![mqtt 7](https://user-images.githubusercontent.com/37902752/126635748-43d8cf3c-4791-4c46-b373-00e1e3e9f9ac.JPG)

입력한 와이파이로 연결 후 OLED에 주소가 나와있는데 와이파이마다 다릅니다. 나온 ip주소를 인터넷창에 입력
### 8. 다운로드-1 단계
![mqtt 8](https://user-images.githubusercontent.com/37902752/126635751-f1cb5855-64db-4857-a95b-b76d84dd75fe.JPG)

햄버거 메뉴에 다운로드 클릭
### 9. 다운로드-2 단계
![mqtt 9](https://user-images.githubusercontent.com/37902752/126635753-97589095-b46e-4cb9-8efc-ccdb6e3f4194.JPG)

kiumi 현장제어 프로그램을 누르고 다운로드 받습니다.(OLED에 Download...가 뜨고 30초 정도 뒤에 재부팅)
### 10. Node-Red-1 단계
![image](https://user-images.githubusercontent.com/83691399/126778751-bfb16b20-a78f-47b4-bf53-8d92fb5c063e.png)

위에 나와있는 node-red flow를 복사 후 (오른쪽 저장버튼 누르면 자동복사)
### 11. Node-Red-2 단계
![image](https://user-images.githubusercontent.com/83691399/126778843-d36fe0c9-5653-439d-b990-a53972a81874.png)

cmd에서 node-red를 실행한 후 localhost:1880에 접속해 우측 상단 햄버거 메뉴에 가져오기를 클릭 후 flow를 붙여넣기 
### 12. Node-Red-3 단계
![mqtt 12](https://user-images.githubusercontent.com/37902752/126635758-08ac0a84-2297-4fc3-b40a-a2101f286073.JPG)

window메뉴를 누른뒤 cmd를 검색 후 ipconfig를 검색하여 IPv4 주소값을 복사
### 13. Node-Red-4 단계 
![image](https://user-images.githubusercontent.com/83691399/126779048-6c6dae19-4670-4cb1-98fa-f1338967e056.png)
복사한 ipv4주소를 mqtt노드 서버  주소를 변경
### 14. Mqtt 연결
![mqtt 15](https://user-images.githubusercontent.com/37902752/126635726-ce34f76a-63e1-4ee3-b1d9-d89c7e22f02c.JPG)

다시 홈페이로 돌아와서 방금 복사한  IPv4값을 붙여넣기
### 16. 디버그 확인
첨부예정

Node-red(localhost:1880)에 다시 들어가서 디버그(무당벌레 아이콘)에 들어가서 빨간색 글씨로 센서값이 홈페이지 센서값과 OLED에 나온 센서값이 동일한지 확인
### 17. 결과 확인
첨부 예정

컴퓨터에서 설정이 끝났고 나중에 현장에서 재확인할 때 테블릿이나 휴대폰으로 설정한 와이파이에 접속후 `localhost:1880/ui`에 접속하면 확인가능
