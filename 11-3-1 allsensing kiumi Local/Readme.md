키우미센서
======================
![016](https://user-images.githubusercontent.com/83691399/124417807-f4639f80-dd94-11eb-9a02-b59e1b55a9aa.png)

## 1.1 키우미센서란?
키우미센서는 에코나레에서 개발한 온도, 습도, CO2를 측정해주는 센서입니다.

## 1.2 키우미센서 프로토콜
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

## 1.3 배선도
![018](https://user-images.githubusercontent.com/83691399/124417589-68517800-dd94-11eb-8e46-ad3d20f0020b.png)

## 1.4 Node-Red소스
```
[{"id":"ace6987b.29b6f8","type":"tab","label":"플로우 1","disabled":false,"info":""},{"id":"d9990130.6b6","type":"mqtt out","z":"ace6987b.29b6f8","name":"Kium-Local mqtt","topic":"/doowon-Kium/inTopic","qos":"0","retain":"","broker":"c84ecd4a.7ca0e","x":620,"y":420,"wires":[]},{"id":"26812c2.15605d4","type":"mqtt in","z":"ace6987b.29b6f8","name":"Kium-Local mqtt","topic":"/Kium-Local mqtt/outTopic","qos":"0","datatype":"auto","broker":"c84ecd4a.7ca0e","x":430,"y":480,"wires":[["c634888f.244468","e083594b.5ba838"]]},{"id":"8329e6b2.2e0788","type":"inject","z":"ace6987b.29b6f8","name":"","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":"테스트 메세지","payloadType":"str","x":430,"y":420,"wires":[["d9990130.6b6"]]},{"id":"c634888f.244468","type":"debug","z":"ace6987b.29b6f8","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":730,"y":480,"wires":[]},{"id":"e083594b.5ba838","type":"json","z":"ace6987b.29b6f8","name":"","property":"payload","action":"","pretty":false,"x":670,"y":540,"wires":[["bef522ec.df231"]]},{"id":"bef522ec.df231","type":"function","z":"ace6987b.29b6f8","name":"변환","func":"var TEMMsg={};\nvar HUMMsg={};\nvar COMsg={};\n\nTEMMsg.payload=msg.payload.TEM;\nHUMMsg.payload=msg.payload.HUM;\nCOMsg.payload=msg.payload.CO;\n\nreturn [TEMMsg,HUMMsg,COMsg];\n","outputs":3,"noerr":0,"initialize":"","finalize":"","x":810,"y":540,"wires":[["82fa0a25.e5e418"],["76ddea7a.91cfc4"],["c9239348.a37e1"]]},{"id":"c9239348.a37e1","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":2,"width":"3","height":"3","gtype":"wave","title":"CO2","label":"ph","format":"{{value}}","min":"400","max":"3000","colors":["#00b500","#e6e600","#ca3838"],"seg1":"600","seg2":"800","x":1030,"y":580,"wires":[]},{"id":"82fa0a25.e5e418","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":0,"width":"3","height":"3","gtype":"donut","title":"온도","label":"℃","format":"{{value}}","min":"-20","max":"70","colors":["#50e2e0","#4bf00f","#ca3838"],"seg1":"20","seg2":"35","x":1030,"y":460,"wires":[]},{"id":"76ddea7a.91cfc4","type":"ui_gauge","z":"ace6987b.29b6f8","name":"","group":"199b0928.389a87","order":1,"width":"3","height":"3","gtype":"wave","title":"습도","label":"%","format":"{{value}}","min":0,"max":"100","colors":["#00b500","#e6e600","#ca3838"],"seg1":"40","seg2":"80","x":1030,"y":520,"wires":[]},{"id":"c84ecd4a.7ca0e","type":"mqtt-broker","name":"","broker":"192.168.0.67","port":"1883","clientid":"","usetls":false,"compatmode":false,"keepalive":"60","cleansession":true,"birthTopic":"","birthQos":"0","birthPayload":"","closeTopic":"","closeQos":"0","closePayload":"","willTopic":"","willQos":"0","willPayload":""},{"id":"199b0928.389a87","type":"ui_group","name":"키우미","tab":"5eec05a4.959bac","order":1,"disp":true,"width":"6","collapse":false},{"id":"5eec05a4.959bac","type":"ui_tab","name":"키우미","icon":"dashboard","order":3,"disabled":false,"hidden":false}]
```
