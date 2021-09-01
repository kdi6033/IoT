1.온습도센서(SimpleTH_Probe485)
=============
1-1. SimpleTH_Probe485통신
-------------
###### SimpleTH_Probe485 온습도프로브는 RS485통신을 사용하여 여러 지점의 온도 및 습도를 동시에 감시할 수 있습니다.
1-2. SimpleTH_Probe485 배선도
-------------
![ㅁㄴㅇ](https://user-images.githubusercontent.com/86224836/123580716-582a1d80-d815-11eb-9329-7b23d0884ec7.png)


1-3. SimpleTH_Probe485 Manual
-------------
![SimpleTH_Probe_1](https://user-images.githubusercontent.com/86224836/122852917-5b706580-d34c-11eb-9ca5-96f1ab5b7323.png)
![SimpleTH_Probe_2](https://user-images.githubusercontent.com/86224836/122852921-5ca19280-d34c-11eb-8653-5028a0047946.png)
###### Address Setting/주소세팅 (:FF04800200017A)
###### Address change/주소변경 (:FFA0800200825D)
###### RH/Temp(both)/습도,온도 (:80040004000276)
###### Relative Humidity only/상대습도 (:80040004000177)
###### Temperature only/오직 온도만 (:80040005000176)
1-4. Node-Red source
-------------
<pre><code>[{"id":"2d791af1.11a096","type":"tab","label":"플로우 2","disabled":false,"info":""},{"id":"4053fcb0.8c9804","type":"serial out","z":"2d791af1.11a096","name":"","serial":"adc9d8bd.2b3c18","x":260,"y":40,"wires":[]},{"id":"4ac092bc.0fbb2c","type":"serial in","z":"2d791af1.11a096","name":"","serial":"adc9d8bd.2b3c18","x":290,"y":120,"wires":[["2af17914.bbc146","d91cb857.0667e8"]]},{"id":"2af17914.bbc146","type":"debug","z":"2d791af1.11a096","name":"","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":470,"y":40,"wires":[]},{"id":"63ee301a.847a8","type":"inject","z":"2d791af1.11a096","name":"온도 습도","props":[{"p":"payload"},{"p":"topic","vt":"str"}],"repeat":"","crontab":"","once":false,"onceDelay":0.1,"topic":"","payload":"::80040004000276","payloadType":"str","x":100,"y":40,"wires":[["4053fcb0.8c9804"]]},{"id":"d91cb857.0667e8","type":"function","z":"2d791af1.11a096","name":"","func":"var msg1={};\nvar msg2={};\nmsg1.payload = msg.payload.substr(7,4);\nmsg2.payload = msg.payload.substr(11,4);\nmsg1.payload = parseInt(\"0x\"+msg1.payload) / 100.0;\nmsg2.payload = parseInt(\"0x\"+msg2.payload) / 100.0;\nreturn [msg1,msg2];","outputs":2,"noerr":0,"initialize":"","finalize":"","libs":[],"x":460,"y":180,"wires":[["f2761e48.d1b91","11c950a0.e2bf5f"],["aa8e959c.43f2b8","c1a7349b.6ba7f8"]]},{"id":"c1a7349b.6ba7f8","type":"ui_gauge","z":"2d791af1.11a096","name":"","group":"6b9e289f.a9f4c8","order":0,"width":0,"height":0,"gtype":"gage","title":"온도","label":"ºC","format":"{{value}}","min":"-20","max":"50","colors":["#00b500","#e6e600","#ca3838"],"seg1":"20","seg2":"38","x":630,"y":140,"wires":[]},{"id":"11c950a0.e2bf5f","type":"ui_gauge","z":"2d791af1.11a096","name":"","group":"6b9e289f.a9f4c8","order":0,"width":0,"height":0,"gtype":"wave","title":"습도","label":"%","format":"{{value}}","min":0,"max":"100","colors":["#00b500","#e6e600","#ca3838"],"seg1":"","seg2":"","x":630,"y":80,"wires":[]},{"id":"f2761e48.d1b91","type":"debug","z":"2d791af1.11a096","name":"습도 디버그","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":650,"y":200,"wires":[]},{"id":"aa8e959c.43f2b8","type":"debug","z":"2d791af1.11a096","name":"온도 디버그","active":true,"tosidebar":true,"console":false,"tostatus":false,"complete":"payload","targetType":"msg","statusVal":"","statusType":"auto","x":650,"y":260,"wires":[]},{"id":"adc9d8bd.2b3c18","type":"serial-port","serialport":"COM15","serialbaud":"9600","databits":"8","parity":"none","stopbits":"1","waitfor":"","dtr":"none","rts":"none","cts":"none","dsr":"none","newline":"\\n","bin":"false","out":"char","addchar":"\\r\\n","responsetimeout":"10000"},{"id":"6b9e289f.a9f4c8","type":"ui_group","name":"온습도센서-1","tab":"d5dfc051.282ac","order":1,"disp":true,"width":"6","collapse":false},{"id":"d5dfc051.282ac","type":"ui_tab","name":"온습도센서","icon":"dashboard","order":5,"disabled":false,"hidden":false}]</code></pre>
