# node red program
```
[
    {
        "id": "10a86fdc.efc16",
        "type": "subflow",
        "name": "Iterate",
        "in": [
            {
                "x": 220,
                "y": 219,
                "wires": [
                    {
                        "id": "8709b651.7ce108"
                    }
                ]
            }
        ],
        "out": [
            {
                "x": 454,
                "y": 174,
                "wires": [
                    {
                        "id": "8709b651.7ce108",
                        "port": 0
                    }
                ]
            },
            {
                "x": 455,
                "y": 259,
                "wires": [
                    {
                        "id": "8709b651.7ce108",
                        "port": 1
                    }
                ]
            }
        ]
    },
    {
        "id": "8709b651.7ce108",
        "type": "function",
        "z": "10a86fdc.efc16",
        "name": "Iterate",
        "func": "//Node has 2 outputs - 1 for itteration and 1 for completion\nvar nextObj, out;\nvar itt = msg.iterationInfo;\n\n\n//If the iterating has not yet begun set up the iteration metadata in the msg\nif (typeof itt === 'undefined') {\n    //Make sure payload is an array\n    if( Object.prototype.toString.call(msg.payload) !== '[object Array]' ) {\n       msg.payload = [msg.payload];\n    }\n\n    msg.iterationInfo = itt = {};\n    itt.index = -1;\n    itt.inArray = msg.payload;\n    itt.outArray = [];\n\n//Otherwise just push the input to the output array\n} else {\n    itt.outArray.push(msg.payload)\n}\n\n//Goto next object\nitt.index ++;\n\n//If there are stil objects left to iterate goto the next one in the original array\nif (itt.index < itt.inArray.length) {\n    nextObj = msg;\n    msg.payload = itt.inArray[itt.index];\n\n//otherwise pass the out array as the payload\n} else {\n    out = msg;\n    msg.payload = itt.outArray;\n    delete msg.iterationInfo;\n}\n\nreturn [nextObj, out];",
        "outputs": "2",
        "noerr": 0,
        "x": 347,
        "y": 220,
        "wires": [
            [],
            []
        ]
    },
    {
        "id": "10c62f07eb65f6ee",
        "type": "mqtt in",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "x": 270,
        "y": 540,
        "wires": [
            [
                "de4755088f7db9be"
            ]
        ]
    },
    {
        "id": "de20e47e3c579a03",
        "type": "subflow:10a86fdc.efc16",
        "z": "b2c1c0ed37e56059",
        "name": "Iterate",
        "env": [],
        "x": 730,
        "y": 460,
        "wires": [
            [
                "ff320c8a0bc00586",
                "0f37cb572b3ea60a"
            ],
            []
        ]
    },
    {
        "id": "0f37cb572b3ea60a",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 728,
        "y": 400,
        "wires": [
            [
                "de20e47e3c579a03"
            ]
        ]
    },
    {
        "id": "458b0b06e4869533",
        "type": "udp out",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "addr": "",
        "iface": "",
        "port": "",
        "ipv": "udp4",
        "outport": "",
        "base64": false,
        "multicast": "false",
        "x": 1194,
        "y": 456,
        "wires": []
    },
    {
        "id": "ff320c8a0bc00586",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "var server=msg.server;\nvar newMsg={};\nnewMsg.payload=\"{\\\"mqttIp\\\":\\\"\"+server+\"\\\"}\";\nnewMsg.ip=msg.payload;\nnewMsg.port=4210;\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 901,
        "y": 458,
        "wires": [
            [
                "21d319b77592cbea"
            ]
        ]
    },
    {
        "id": "21d319b77592cbea",
        "type": "change",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "rules": [
            {
                "t": "set",
                "p": "ip",
                "pt": "msg",
                "to": "ip",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 1055,
        "y": 457,
        "wires": [
            [
                "458b0b06e4869533"
            ]
        ]
    },
    {
        "id": "5d634aa31bf10710",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "var ipList=[];\nvar server=global.get(\"serverIp\");\nvar a=server.split('.');\nvar ip=a[0]+'.'+a[1]+'.'+a[2]+'.';\n\nmsg.server=server;\nfor(var i=2;i<255;i++)\n    ipList.push(ip+i);\nmsg.payload=ipList;\nreturn msg;\n",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 560,
        "y": 460,
        "wires": [
            [
                "de20e47e3c579a03"
            ]
        ]
    },
    {
        "id": "d28745cee5f9d734",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 560,
        "y": 540,
        "wires": [
            [
                "b1fe09f831c423cd"
            ]
        ]
    },
    {
        "id": "b1fe09f831c423cd",
        "type": "mongodb2 in",
        "z": "b2c1c0ed37e56059",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 700,
        "y": 540,
        "wires": [
            [
                "9ff68e6c54586769"
            ]
        ]
    },
    {
        "id": "9ff68e6c54586769",
        "type": "switch",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "null"
            },
            {
                "t": "nnull"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 2,
        "x": 830,
        "y": 540,
        "wires": [
            [
                "627ea8138fc99cdc"
            ],
            [
                "94a4576d61ec8ce5"
            ]
        ]
    },
    {
        "id": "627ea8138fc99cdc",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "insert one",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nnewMsg.payload    = {'type':msg.payload.type,'mac':msg.payload.mac,'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 972,
        "y": 527,
        "wires": [
            [
                "6b2559f0bd666e78"
            ]
        ]
    },
    {
        "id": "6b2559f0bd666e78",
        "type": "mongodb2 in",
        "z": "b2c1c0ed37e56059",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1150,
        "y": 528,
        "wires": [
            []
        ]
    },
    {
        "id": "94a4576d61ec8ce5",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "findOneUpdate",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\n\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 980,
        "y": 580,
        "wires": [
            [
                "6b2559f0bd666e78"
            ]
        ]
    },
    {
        "id": "de4755088f7db9be",
        "type": "json",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 417,
        "y": 540,
        "wires": [
            [
                "d28745cee5f9d734"
            ]
        ]
    },
    {
        "id": "f89569ec89cdccac",
        "type": "http in",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 100,
        "y": 60,
        "wires": [
            [
                "f03543339147c399"
            ]
        ]
    },
    {
        "id": "2aca8b94f9e901ff",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "style",
        "field": "payload.style",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "body {\n    background: #eab0dc;\n    font-family: Arial, Helvetica, sans-serif;\n}\ntable, th, td {\n    padding: 2px;\n    border-collapse: collapse;\n    border: 1px solid #dddddd;\n}\n\n\n/* Full-width input fields */\ninput[type=text], input[type=password] {\n  width: 150px;\n  padding: 5px 10px;\n  margin: 8px 0;\n  display: inline-block;\n  border: 1px solid #ccc;\n  box-sizing: border-box;\n}\n\n/* Set a style for all buttons */\nbutton {\n  background-color: #4CAF50; /*Green*/\n  color: white;\n  padding: 14px 20px;\n  margin: 8px 0;\n  border: none;\n  cursor: pointer;\n}\n\n.buttonMenu {\n          padding: 5px 24px;\n          margin-left:20%;\n          background-color:black;\n          border: none;\n          border-color:black;\n          color:white;\n          text-align: left;\n          text-decoration: none;\n          display: inline-block;\n          font-size: 16px;\n          margin: 4px 2px;\n          cursor: pointer;\n        }\n        .sidenav {\n          height: 100%;\n          width: 0;\n          position: fixed;\n          z-index: 1;\n          top: 0;\n          left: 0;\n          background-color: #111;\n          overflow-x: hidden;\n          transition: 0.5s;\n          padding-top: 60px;\n        }\n        .sidenav a {\n          padding: 8px 8px 8px 32px;\n          text-decoration: none;\n          font-size: 18px;\n          color: #818181;\n          display: block;\n                transition: 0.3s;\n        }\n        .sidenav a:hover {\n          color: #f1f1f1;\n        }\n        .sidenav .closebtn {\n          position: absolute;\n          top: 0;\n          right: 25px;\n          font-size: 36px;\n          margin-left: 50px;\n        }\n\n.buttonM {background-color: #ff66cc;color:white; width:100px; height:20px; padding: 0px 0px; font-size: 16px} /* 기기선택 */  \n.buttonL {background-color: #ff66cc;color:white; width:100px; height:25px; padding: 0px 0px; font-size: 16px} /* 선택 */  \n.buttonMenu {background-color: #000000;} \n.button2 {background-color: #008CBA;} /* Blue */\n.button3 {background-color: #f44336;} /* Red */ \n.button4 {background-color: #e7e7e7; color: black;} /* Gray */ \n.button5 {background-color: #555555;} /* Black */\n.button20 {width: 20%;} \n.button-on {background-color:Lime; color:blue; width:20%;} \n.button-off {background-color:LightGrey; color:white; width:20%;} \n.button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}\n.button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}\n\nbutton:hover {\n  opacity: 0.8;\n}\n\n/* Extra styles for the cancel button */\n.cancelbtn {\n  width: auto;\n  padding: 10px 18px;\n  background-color: #f44336;\n}\n\n/* Center the image and position the close button */\n.imgcontainer {\n  text-align: center;\n  margin: 24px 0 12px 0;\n  position: relative;\n}\n\nimg.avatar {\n  width: 40%;\n  border-radius: 50%;\n}\n\n.container {\n  padding: 16px;\n}\n\nspan.psw {\n  float: right;\n  padding-top: 16px;\n}\n\n/* The Modal (background) */\n.modal {\n  display: none; /* Hidden by default */\n  position: fixed; /* Stay in place */\n  z-index: 1; /* Sit on top */\n  left: 0;\n  top: 0;\n  width: 100%; /* Full width */\n  height: 100%; /* Full height */\n  overflow: auto; /* Enable scroll if needed */\n  background-color: rgb(0,0,0); /* Fallback color */\n  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */\n  padding-top: 60px;\n}\n\n/* Modal Content/Box */\n.modal-content {\n  background-color: #fefefe;\n  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */\n  border: 1px solid #888;\n  width: 80%; /* Could be more or less, depending on screen size */\n}\n\n/* The Close Button (x) */\n.close {\n  position: absolute;\n  right: 25px;\n  top: 0;\n  color: #000;\n  font-size: 35px;\n  font-weight: bold;\n}\n\n.close:hover,\n.close:focus {\n  color: red;\n  cursor: pointer;\n}\n\n/* Add Zoom Animation */\n.animate {\n  -webkit-animation: animatezoom 0.6s;\n  animation: animatezoom 0.6s\n}\n\n@-webkit-keyframes animatezoom {\n  from {-webkit-transform: scale(0)} \n  to {-webkit-transform: scale(1)}\n}\n  \n@keyframes animatezoom {\n  from {transform: scale(0)} \n  to {transform: scale(1)}\n}\n\n/* Change styles for span and cancel button on extra small screens */\n@media screen and (max-width: 300px) {\n  span.psw {\n     display: block;\n     float: none;\n  }\n  .cancelbtn {\n     width: 100%;\n  }\n}",
        "x": 670,
        "y": 60,
        "wires": [
            [
                "1d5f3c1cec08a3b4"
            ]
        ]
    },
    {
        "id": "1b0c9ccc0a81df87",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "\n        \n// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}",
        "output": "str",
        "x": 545,
        "y": 60,
        "wires": [
            [
                "2aca8b94f9e901ff"
            ]
        ]
    },
    {
        "id": "d6e3d2c78544feea",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\n/*\nmsg.collection = 'localRecord';\nmsg.operation  = 'find.toArray';\nmsg.payload    = {  };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\n*/\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 941,
        "y": 60,
        "wires": [
            [
                "b5faec73c2c2b14f"
            ]
        ]
    },
    {
        "id": "1d5f3c1cec08a3b4",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='/display?act=2'>환경설정</a>\n  <a href='/display?act=4'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 790,
        "y": 60,
        "wires": [
            [
                "d6e3d2c78544feea"
            ]
        ]
    },
    {
        "id": "bd6fa092d2581221",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "find.toArray",
        "func": "global.set(\"msg_main\",msg);\nmsg.collection = 'localRecord';\nmsg.operation  =  'find.toArray';\nmsg.payload    = { };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 750,
        "y": 220,
        "wires": [
            [
                "d48740b9385e3dd4"
            ]
        ]
    },
    {
        "id": "d48740b9385e3dd4",
        "type": "mongodb2 in",
        "z": "b2c1c0ed37e56059",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 890,
        "y": 220,
        "wires": [
            [
                "9f0be253736b0ed8"
            ]
        ]
    },
    {
        "id": "bb6f974666d4d202",
        "type": "http response",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1510,
        "y": 220,
        "wires": []
    },
    {
        "id": "fa702425b53810ba",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "0,1,2,3 List",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    if(msg.payload[i].type==2) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>PE-300</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>키우미</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>KSM-8900</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>simpleTH</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n       s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    else if(msg.payload[i].type==6) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>XEX-DR14E</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>\"\n            for(var j=0;j<8;j++) {\n                if(msg.payload[i].in[j]==1) \n                    s+=\"<button class='button button-ledon'>\"+j+\"</button>&nbsp;\";\n                else\n                    s+=\"<button class='button button-ledoff'>\"+j+\"</button>&nbsp;\";\n            }\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>DHT11</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>RY-WS301</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n     else if(msg.payload[i].type==9) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>MP-508E</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    \n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1190,
        "y": 220,
        "wires": [
            [
                "ad8c917f7d74b388"
            ]
        ]
    },
    {
        "id": "ad8c917f7d74b388",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<meta http-equiv='refresh' content='10'/>\n<style> {{{payload.style}}} </style>\n<script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>",
        "output": "str",
        "x": 1360,
        "y": 220,
        "wires": [
            [
                "bb6f974666d4d202"
            ]
        ]
    },
    {
        "id": "9f0be253736b0ed8",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_main\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1020,
        "y": 220,
        "wires": [
            [
                "fa702425b53810ba"
            ]
        ]
    },
    {
        "id": "76c76f3d8712f147",
        "type": "http in",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 270,
        "y": 140,
        "wires": [
            [
                "13e13a70ed3e6af7"
            ]
        ]
    },
    {
        "id": "9aa1eb0953b2d599",
        "type": "comment",
        "z": "b2c1c0ed37e56059",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 94,
        "y": 140,
        "wires": []
    },
    {
        "id": "dbaf151f6118ef35",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "get global",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nmsg.payload.act=global.get(\"act\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1200,
        "y": 120,
        "wires": [
            [
                "a2887b949c63d31b"
            ]
        ]
    },
    {
        "id": "a2887b949c63d31b",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "html 기기 name",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n   <br> &emsp;\n   <table>\n         <th>mac</th> <th>{{{payload.mac}}}</th>\n      </tr>\n      <tr>\n        <th>접속IP</th> <th>{{{payload.ip}}}</th>\n      </tr>\n      <tr>\n        <th>이름</th> \n            <form action=\"/save\" method=\"post\">\n                <input type='hidden' name='mac' value='{{{payload.mac}}}'>\n                <input type='hidden' name='act' value='1'>\n                <th><input type=\"text\" id=\"name\" name=\"name\" value=\"{{{payload.name}}}\"></th>\n                <th><button class='button buttonL' type=\"submit\" style=\"width:50px;\" >저장</button></th>\n            </form> \n      </tr>\n    </table>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 1360,
        "y": 120,
        "wires": [
            [
                "8b047cd9223a3253"
            ]
        ]
    },
    {
        "id": "a7e8d90a60038940",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 120,
        "wires": [
            [
                "ee56c1503c6c35e5"
            ]
        ]
    },
    {
        "id": "ee56c1503c6c35e5",
        "type": "mongodb2 in",
        "z": "b2c1c0ed37e56059",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 910,
        "y": 120,
        "wires": [
            [
                "72b1d06dc76a7116"
            ]
        ]
    },
    {
        "id": "8b047cd9223a3253",
        "type": "http response",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1530,
        "y": 120,
        "wires": []
    },
    {
        "id": "13e13a70ed3e6af7",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "global.set(\"msg_display\",msg);\nglobal.set(\"act\",msg.payload.act);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 440,
        "y": 140,
        "wires": [
            [
                "8ff53e362df843dc"
            ]
        ]
    },
    {
        "id": "72b1d06dc76a7116",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_display\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1060,
        "y": 120,
        "wires": [
            [
                "dbaf151f6118ef35"
            ]
        ]
    },
    {
        "id": "8ff53e362df843dc",
        "type": "switch",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "property": "payload.act",
        "propertyType": "msg",
        "rules": [
            {
                "t": "eq",
                "v": "1",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "2",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "3",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "4",
                "vt": "str"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 4,
        "x": 590,
        "y": 140,
        "wires": [
            [
                "a7e8d90a60038940"
            ],
            [
                "3a490dcf97e83de0"
            ],
            [
                "bd6fa092d2581221"
            ],
            [
                "f6bce435d09a13a6"
            ]
        ]
    },
    {
        "id": "02c84740db893892",
        "type": "http in",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 270,
        "y": 340,
        "wires": [
            [
                "1ee479c2ed78ce72"
            ]
        ]
    },
    {
        "id": "7c3f3d22c3156080",
        "type": "comment",
        "z": "b2c1c0ed37e56059",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 94,
        "y": 340,
        "wires": []
    },
    {
        "id": "1eb631566c367ee2",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 580,
        "y": 340,
        "wires": [
            [
                "e6ab248a928dcd44"
            ]
        ]
    },
    {
        "id": "e6ab248a928dcd44",
        "type": "mongodb2 in",
        "z": "b2c1c0ed37e56059",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 750,
        "y": 340,
        "wires": [
            [
                "fb8e29510f8887d8"
            ]
        ]
    },
    {
        "id": "c461ed1fa21871a8",
        "type": "link out",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "links": [
            "714cc4f175d76795",
            "2044220.61e99de"
        ],
        "x": 995,
        "y": 340,
        "wires": []
    },
    {
        "id": "fb8e29510f8887d8",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 900,
        "y": 340,
        "wires": [
            [
                "c461ed1fa21871a8"
            ]
        ]
    },
    {
        "id": "1ee479c2ed78ce72",
        "type": "switch",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "property": "payload.act",
        "propertyType": "msg",
        "rules": [
            {
                "t": "eq",
                "v": "1",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "2",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "3",
                "vt": "str"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 3,
        "x": 410,
        "y": 340,
        "wires": [
            [
                "1eb631566c367ee2"
            ],
            [],
            []
        ]
    },
    {
        "id": "a23d4521137b3969",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.internalIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1470,
        "y": 60,
        "wires": [
            [
                "52fe2ac47a20ec82"
            ]
        ]
    },
    {
        "id": "52fe2ac47a20ec82",
        "type": "http response",
        "z": "b2c1c0ed37e56059",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1622,
        "y": 60,
        "wires": []
    },
    {
        "id": "714cc4f175d76795",
        "type": "link in",
        "z": "b2c1c0ed37e56059",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "b5faec73c2c2b14f",
            "c461ed1fa21871a8"
        ],
        "x": 1239,
        "y": 60,
        "wires": [
            [
                "c65224e92ed8bad2"
            ]
        ]
    },
    {
        "id": "b5faec73c2c2b14f",
        "type": "link out",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "links": [
            "714cc4f175d76795"
        ],
        "x": 1055,
        "y": 60,
        "wires": []
    },
    {
        "id": "c3fc380ce55a1da2",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "html 환경설정",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    환경설정 <br>\n    센서와 기기를 서버에 연결하려면 아래 버튼을 누르세요\n    <form action=\"/act\" method=\"post\">\n        <input type='hidden' name='act' value='1'>\n        <div class=\"container\">\n            <br> &emsp;\n            <button class='button buttonL' type=\"submit\" style=\"width:auto;\">- mqtt 통신연결 -</button>\n        </div>\n    </form>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 940,
        "y": 180,
        "wires": [
            [
                "2471af8f947856cc"
            ]
        ]
    },
    {
        "id": "3a490dcf97e83de0",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 180,
        "wires": [
            [
                "c3fc380ce55a1da2"
            ]
        ]
    },
    {
        "id": "2471af8f947856cc",
        "type": "http response",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1107,
        "y": 180,
        "wires": []
    },
    {
        "id": "3e3814f7c3b9623f",
        "type": "template",
        "z": "b2c1c0ed37e56059",
        "name": "html 메뉴얼",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    메뉴얼\n\n</body>\n</html>\n",
        "output": "str",
        "x": 890,
        "y": 280,
        "wires": [
            [
                "67e737e62d3e9101"
            ]
        ]
    },
    {
        "id": "f6bce435d09a13a6",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 730,
        "y": 280,
        "wires": [
            [
                "3e3814f7c3b9623f"
            ]
        ]
    },
    {
        "id": "67e737e62d3e9101",
        "type": "http response",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1067,
        "y": 280,
        "wires": []
    },
    {
        "id": "19db6d642a4d4b93",
        "type": "comment",
        "z": "b2c1c0ed37e56059",
        "name": "Act",
        "info": "1: \n",
        "x": 90,
        "y": 460,
        "wires": []
    },
    {
        "id": "719d2db6867eed65",
        "type": "http in",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 260,
        "y": 460,
        "wires": [
            [
                "e94e63b69a7aae3f"
            ]
        ]
    },
    {
        "id": "e94e63b69a7aae3f",
        "type": "switch",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "property": "payload.act",
        "propertyType": "msg",
        "rules": [
            {
                "t": "eq",
                "v": "1",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "2",
                "vt": "str"
            },
            {
                "t": "eq",
                "v": "3",
                "vt": "str"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 3,
        "x": 410,
        "y": 460,
        "wires": [
            [
                "5d634aa31bf10710"
            ],
            [],
            []
        ]
    },
    {
        "id": "fa8062dce9db4fa3",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "var server=msg.payload[1].address;\nglobal.set(\"serverIp\",server);\nmsg.server=server;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 400,
        "y": 60,
        "wires": [
            [
                "1b0c9ccc0a81df87"
            ]
        ]
    },
    {
        "id": "c65224e92ed8bad2",
        "type": "function",
        "z": "b2c1c0ed37e56059",
        "name": "",
        "func": "var serverIp=global.get(\"serverIp\");\nmsg.payload.internalIPv4=serverIp;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1340,
        "y": 60,
        "wires": [
            [
                "a23d4521137b3969"
            ]
        ]
    },
    {
        "id": "f03543339147c399",
        "type": "hostip",
        "z": "b2c1c0ed37e56059",
        "name": "Host IP",
        "x": 240,
        "y": 60,
        "wires": [
            [
                "fa8062dce9db4fa3"
            ]
        ]
    },
    {
        "id": "f15d66b3.c7edf8",
        "type": "mqtt-broker",
        "name": "",
        "broker": "localhost",
        "port": "1883",
        "tls": "509a45a9.089bbc",
        "clientid": "",
        "usetls": false,
        "protocolVersion": "4",
        "keepalive": "60",
        "cleansession": true,
        "birthTopic": "",
        "birthQos": "0",
        "birthPayload": "",
        "birthMsg": {},
        "closeTopic": "",
        "closeQos": "0",
        "closePayload": "",
        "closeMsg": {},
        "willTopic": "",
        "willQos": "0",
        "willPayload": "",
        "willMsg": {},
        "sessionExpiry": ""
    },
    {
        "id": "ff55d734.a9cd28",
        "type": "mongodb2",
        "uri": "mongodb://localhost:27000/local",
        "name": "local",
        "options": "",
        "parallelism": "-1"
    },
    {
        "id": "8364b0aa.ebd7e",
        "type": "mongodb2",
        "uri": "mongodb://localhost:27000/local",
        "name": "local",
        "options": "",
        "parallelism": "-1"
    },
    {
        "id": "509a45a9.089bbc",
        "type": "tls-config",
        "name": "",
        "cert": "",
        "key": "",
        "ca": "",
        "certname": "ea1ef80b73-certificate.pem.crt",
        "keyname": "ea1ef80b73-private.pem.key",
        "caname": "MtesCA.pem",
        "servername": "",
        "verifyservercert": true
    }
]
```
