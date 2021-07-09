# KSM-8900
![KakaoTalk_20210625_104303022](https://user-images.githubusercontent.com/37902752/123356182-28271400-d5a2-11eb-8a0c-576a4994f4bb.png)

## 온습도 토양 센서
---



* node-red 
```
[{"id":"a2fdc9ef.f3a7c8","type":"tab","label":"local mqtt","disabled":true,"info":""},{"id":"d9990130.6b6","type":"mqtt out","z":"a2fdc9ef.f3a7c8","name":"KSM8900-Local mqtt","topic":"/doowon-ksm8900/inTopic","qos":"0","retain":"","respTopic":"","contentType":"","userProps":"","correl":"","expiry":"","broker":"c84ecd4a.7ca0e","x":380,"y":60,"wires":[]},{"id":"26812c2.15605d4","type":"mqtt in","z":"a2fdc9ef.f3a7c8","name":"KSM8900-Local mqtt","topic":"/doowon-ksm8900/outTopic","qos":"0","datatype":"auto","broker":"c84ecd4a.7ca0e","nl":false,"rap":true,"rh":0,"x":150,"y":140,"wires":[["c634888f.244468","e083594b.5ba838"]]},{"id":"8329e6b2.2e0788","type":"inject","z":"a2fdc9ef.f3a7c8","name":"","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":"테스트 메세지","payloadType":"str","x":150,"y":60,"wires":[["d9990130.6b6"]]},{"id":"c634888f.244468","type":"debug","z":"a2fdc9ef.f3a7c8","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":430,"y":140,"wires":[]},{"id":"e083594b.5ba838","type":"json","z":"a2fdc9ef.f3a7c8","name":"","property":"payload","action":"","pretty":false,"x":410,"y":220,"wires":[["bef522ec.df231"]]},{"id":"bef522ec.df231","type":"function","z":"a2fdc9ef.f3a7c8","name":"변환","func":"var moMsg={};\nvar temMsg={};\nvar ecMsg3={};\n\nmoMsg.payload=msg.payload.mo;\ntemMsg.payload=msg.payload.tem;\necMsg3.payload=msg.payload.ec;\n\nreturn [moMsg,temMsg,ecMsg3];\n","outputs":3,"noerr":0,"initialize":"","finalize":"","libs":[],"x":590,"y":220,"wires":[["5427c459.fa5e2c"],["f5882ccf.beeaf"],["c6759839.7c3428"]]},{"id":"c6759839.7c3428","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"EC","group":"30584558.b1941a","order":2,"width":"3","height":"3","gtype":"compass","title":"토양EC","label":"dS/m","format":"{{value}}","min":"0","max":"10","colors":["#00b500","#e6e600","#ca3838"],"seg1":"600","seg2":"800","x":770,"y":260,"wires":[]},{"id":"5427c459.fa5e2c","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"","group":"30584558.b1941a","order":0,"width":"3","height":"3","gtype":"donut","title":"수분","label":"%VWC","format":"{{value}}","min":"0","max":"50","colors":["#50e2e0","#4bf00f","#ca3838"],"seg1":"20","seg2":"35","x":770,"y":180,"wires":[]},{"id":"f5882ccf.beeaf","type":"ui_gauge","z":"a2fdc9ef.f3a7c8","name":"","group":"30584558.b1941a","order":1,"width":"3","height":"3","gtype":"donut","title":"온도","label":"℃","format":"{{value}}","min":"-40","max":"60","colors":["#00b500","#e6e600","#ca3838"],"seg1":"40","seg2":"80","x":770,"y":220,"wires":[]},{"id":"c84ecd4a.7ca0e","type":"mqtt-broker","name":"","broker":"192.168.0.63","port":"1883","clientid":"","usetls":false,"protocolVersion":"4","keepalive":"60","cleansession":true,"birthTopic":"","birthQos":"0","birthPayload":"","birthMsg":{},"closeTopic":"","closeQos":"0","closePayload":"","closeMsg":{},"willTopic":"","willQos":"0","willPayload":"","willMsg":{},"sessionExpiry":""},{"id":"30584558.b1941a","type":"ui_group","name":"KSM-8900","tab":"1bf78d5b.106443","order":1,"disp":true,"width":"6","collapse":false},{"id":"1bf78d5b.106443","type":"ui_tab","name":"센서","icon":"dashboard","order":3,"disabled":false,"hidden":false}]
```
센서를 두 가지 방법으로 사용하는 방법입니다.

순서

![1](https://user-images.githubusercontent.com/59568377/122491263-9ad24580-d01e-11eb-92cf-f61cb3d44cd0.jpg)

### 전용모듈 SR100 배선도                      
![KakaoTalk_20210625_103957707](https://user-images.githubusercontent.com/37902752/123356078-f4e48500-d5a1-11eb-8765-5bf7ec2ad367.png)

### 전용모듈 SR100으로 사용하기
1. 센서 연결 후 (보드레이트 이하 다른 설정하고)
2. 센서값 추출

### Node-red로 사용하기
1. cmd를 통해 node-red on
2. localhost:1880 접속
3. github txt가져오기 (영상에서는 node-red 파일을 들어갔지만 위에 나와있는거 맨 오른쪽에 복사 버튼을 눌러도됨( 사진설명 맨위에)
4. 팔레트 관리에서 node-red-node-serialport 설치
5. 본인이 맞는 com 불러오기
6. 배포하기를 누른다음 localhost:1880/ui를 입력
7. 센서값 추출
