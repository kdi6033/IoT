# KSM-8900 Local mqtt 

![sumnail](https://user-images.githubusercontent.com/37902752/125016607-942e6f80-e0ac-11eb-9369-05cafbe776f3.jpg)

## Local mqtt 통신하는 방법
### 
---



* Local mqtt 통신 node-red flow 
```
[{"id":"d9990130.6b6","type":"mqtt out","z":"a2fdc9ef.f3a7c8","name":"KSM8900-Local mqtt","topic":"/doowon-ksm8900/inTopic","qos":"0","retain":"","respTopic":"","contentType":"","userProps":"","correl":"","expiry":"","broker":"c84ecd4a.7ca0e","x":340,"y":80,"wires":[]},{"id":"26812c2.15605d4","type":"mqtt in","z":"a2fdc9ef.f3a7c8","name":"KSM8900-Local mqtt","topic":"/doowon-ksm8900/outTopic","qos":"0","datatype":"auto","broker":"c84ecd4a.7ca0e","nl":false,"rap":true,"rh":0,"x":150,"y":140,"wires":[["c634888f.244468","e083594b.5ba838"]]},{"id":"8329e6b2.2e0788","type":"inject","z":"a2fdc9ef.f3a7c8","name":"","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":"테스트 메세지","payloadType":"str","x":130,"y":80,"wires":[["d9990130.6b6"]]},{"id":"c634888f.244468","type":"debug","z":"a2fdc9ef.f3a7c8","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":430,"y":140,"wires":[]},{"id":"e083594b.5ba838","type":"json","z":"a2fdc9ef.f3a7c8","name":"","property":"payload","action":"","pretty":false,"x":370,"y":200,"wires":[["bef522ec.df231"]]},{"id":"bef522ec.df231","type":"function","z":"a2fdc9ef.f3a7c8","name":"변환","func":"var moMsg={};\nvar temMsg={};\nvar ecMsg3={};\n\nmoMsg.payload=msg.payload.mo;\ntemMsg.payload=msg.payload.tem;\necMsg3.payload=msg.payload.ec;\n\nreturn [moMsg,temMsg,ecMsg3];\n","outputs":3,"noerr":0,"initialize":"","finalize":"","libs":[],"x":510,"y":200,"wires":[["5427c459.fa5e2c"],["f5882ccf.beeaf"],["c6759839.7c3428"]]},{"id":"c6759839.7c3428","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"EC","group":"30584558.b1941a","order":2,"width":"3","height":"3","gtype":"compass","title":"토양EC","label":"dS/m","format":"{{value}}","min":"0","max":"10","colors":["#00b500","#e6e600","#ca3838"],"seg1":"600","seg2":"800","x":650,"y":240,"wires":[]},{"id":"5427c459.fa5e2c","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"","group":"30584558.b1941a","order":0,"width":"3","height":"3","gtype":"donut","title":"수분","label":"%VWC","format":"{{value}}","min":"0","max":"50","colors":["#50e2e0","#4bf00f","#ca3838"],"seg1":"20","seg2":"35","x":650,"y":160,"wires":[]},{"id":"f5882ccf.beeaf","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"","group":"30584558.b1941a","order":1,"width":"3","height":"3","gtype":"donut","title":"온도","label":"℃","format":"{{value}}","min":"-40","max":"60","colors":["#00b500","#e6e600","#ca3838"],"seg1":"40","seg2":"80","x":650,"y":200,"wires":[]},{"id":"c84ecd4a.7ca0e","type":"mqtt-broker","name":"","broker":"192.168.0.5","port":"1883","clientid":"","usetls":false,"protocolVersion":"4","keepalive":"60","cleansession":true,"birthTopic":"","birthQos":"0","birthPayload":"","birthMsg":{},"closeTopic":"","closeQos":"0","closePayload":"","closeMsg":{},"willTopic":"","willQos":"0","willPayload":"","willMsg":{},"sessionExpiry":""},{"id":"30584558.b1941a","type":"ui_group","name":"KSM-8900","tab":"1bf78d5b.106443","order":1,"disp":true,"width":"6","collapse":false},{"id":"1bf78d5b.106443","type":"ui_tab","name":"센서","icon":"dashboard","order":3,"disabled":false,"hidden":false}]
```
### 1. WiFi 연결
![mqtt 1](https://user-images.githubusercontent.com/37902752/126635732-45a57837-f648-462f-aa99-325dddd8b12a.JPG)

구매한 RS-485 보드를 컴퓨터에 연결하고 WiFi칸을 보면 i2r-"영어 숫자 랜덤"이 나타나는데 클릭
### 2. 주소 접속
![mqtt 2](https://user-images.githubusercontent.com/37902752/126635736-32761210-ce8e-4798-b597-c047a19ba356.JPG)
i2r-%% wifi를 연결해주면 OLED에 '192.168.4.1'이 나타나는데 i2r wifi가 `연결된 상태`에서 주소창에 입력
### 3. 홈페이지 진입
![mqtt 3](https://user-images.githubusercontent.com/37902752/126635740-df27ab89-8d43-439d-a98a-e1f06035fd82.JPG)

홈페이지에 들어온 후에 좌측 상단 햄버거 메뉴를 선택(빨간색 동그라미)
### 4. 와이파이 설정-1 단계
![mqtt 4](https://user-images.githubusercontent.com/37902752/126635742-fbc5fd0c-9c59-4ea4-9e41-8254f63a2e60.JPG)

메뉴에 들어와 와이파이 설정 클릭
### 5. 와이파이 설정-2 단계
![mqtt 5](https://user-images.githubusercontent.com/37902752/126635743-76e510b3-9329-47f5-aac0-eb6448c600ae.JPG)

와이파이 이름과 비밀번호에 주변에서 사용하고 있는 와이파이를 입력
### 6. 와이파이 설정-3 단계
![mqtt 6](https://user-images.githubusercontent.com/37902752/126635746-1390f1db-d594-4352-86c8-6420861d871d.JPG)

와이파이를 설정해주면 홈페이지 연결이 끊기는데 당연한거라 괜찮습니다. 그 뒤 컴퓨터 wifi를 칸에 입력했던 와이파이를 연결
### 7. 홈페이지 연결
![mqtt 7](https://user-images.githubusercontent.com/37902752/126635748-43d8cf3c-4791-4c46-b373-00e1e3e9f9ac.JPG)

입력한 와이파이로 연결 후 OLED에 주소가 나와있는데 와이파이마다 다릅니다. 나온 ip주소를 인터넷창에 입력
### 8. 다운로드-1 단계
![mqtt 8](https://user-images.githubusercontent.com/37902752/126635751-f1cb5855-64db-4857-a95b-b76d84dd75fe.JPG)

햄버거 메뉴에 다운로드 클릭
### 9. 다운로드-2 단계
![mqtt 9](https://user-images.githubusercontent.com/37902752/126635753-97589095-b46e-4cb9-8efc-ccdb6e3f4194.JPG)

ksm-8900 현장제어 프로그램을 누르고 다운로드 받습니다.(OLED에 Download...가 뜨고 30초 정도 뒤에 재부팅)
### 10. Node-Red-1 단계
![mqtt 10](https://user-images.githubusercontent.com/37902752/126635754-37bc7e29-82d9-4fbc-b5f5-ed914ebfa371.JPG)

위에 나와있는 node-red flow를 복사 후 (오른쪽 저장버튼 누르면 자동복사)
### 11. Node-Red-2 단계
![mqtt 11](https://user-images.githubusercontent.com/37902752/126635757-e2a43fa9-8b47-4827-84e6-41895d831f05.JPG)

cmd에서 node-red를 실행한 후 localhost:1880에 접속해 우측 상단 햄버거 메뉴에 가져오기를 클릭 후 flow를 붙여넣기 
### 12. Node-Red-3 단계
![mqtt 12](https://user-images.githubusercontent.com/37902752/126635758-08ac0a84-2297-4fc3-b40a-a2101f286073.JPG)

window키+R을 누른후 cmd를 검색 후 ipconfig를 검색하여 IPv4 주소값을 복사
### 13. Node-Red-4 단계 
![mqtt 13](https://user-images.githubusercontent.com/37902752/126635715-cd906603-7aec-4c01-ac2b-a52a66f420cc.JPG)
복사한 ipv4주소를 mqtt노드 서버  주소를 변경
### 14. Node-Red-5 단계
![mqtt 14](https://user-images.githubusercontent.com/37902752/126635723-bfd4e645-67be-446f-bc99-83c06f53d2a8.JPG)
보라색 mqtt노드 하단에 초록색으로 접속됨이 떳음을 확인(이 작업 이전에 mosquitto를 통해서 1883 포트를 열어줘야합니다.)
### 15. Mqtt 연결
![mqtt 15](https://user-images.githubusercontent.com/37902752/126635726-ce34f76a-63e1-4ee3-b1d9-d89c7e22f02c.JPG)

다시 홈페이로 돌아와서 방금 복사한  IPv4값을 붙여넣기
### 16. 디버그 확인
![mqtt 16](https://user-images.githubusercontent.com/37902752/126635727-c799cae1-69b7-4eab-b643-dd04105ee9fb.JPG)

Node-red(localhost:1880)에 다시 들어가서 디버그(무당벌레 아이콘)에 들어가서 빨간색 글씨로 센서값이 홈페이지 센서값과 OLED에 나온 센서값이 동일한지 확인
### 17. 결과 확인
![mqtt 17](https://user-images.githubusercontent.com/37902752/126635728-f9a5c634-2440-49f2-a039-ff78047f6f3e.JPG)

컴퓨터에서 설정이 끝났고 나중에 현장에서 재확인할 때 테블릿이나 휴대폰으로 설정한 와이파이에 접속후 `localhost:1880/ui`에 접속하면 확인가능
