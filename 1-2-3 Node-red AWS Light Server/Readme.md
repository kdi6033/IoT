# node red program
IoT core 설치     
node-red 설치     
mongoDB 설치
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
        "id": "997ba372c49185d6",
        "type": "tab",
        "label": "플로우 1",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "9f6dfb540e3039a1",
        "type": "mqtt in",
        "z": "997ba372c49185d6",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 170,
        "y": 460,
        "wires": [
            [
                "03cb2200063973b3"
            ]
        ]
    },
    {
        "id": "21ea7cd1f9606606",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 460,
        "y": 460,
        "wires": [
            [
                "89461b3acde25e3a"
            ]
        ]
    },
    {
        "id": "6db97a988ae1f91a",
        "type": "switch",
        "z": "997ba372c49185d6",
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
        "x": 730,
        "y": 460,
        "wires": [
            [
                "6862ef41f689f78b"
            ],
            [
                "af74294792de9263"
            ]
        ]
    },
    {
        "id": "6862ef41f689f78b",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "insert one (수정)",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar model=[];\nmodel[2]='PE-300';\nmodel[3]='키우미';\nmodel[4]='KSM-8900';\nmodel[5]='simpleTH';\nmodel[6]='XEX-DR14E';\nmodel[7]='DHT11';\nmodel[8]='RY-WS301';\nmodel[9]='MP-508E';\nmodel[10]='사운드';\nmodel[11]='충격센서';\nmodel[12]='PLC-RS485';\nmodel[13]='차양모터';\nmodel[14]='MASTER-K120s';\nmodel[15]='smoke';\nmodel[16]='methane';\nmodel[18]='PLC-RS485-8ch';\n\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nif(msg.payload.type==6)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0] };\nelse if(msg.payload.type==12 || msg.payload.type==14)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0] };\nelse if(msg.payload.type==18)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0,0,0] };\nelse\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 892,
        "y": 447,
        "wires": [
            [
                "57cb9bac99aef8ea"
            ]
        ]
    },
    {
        "id": "af74294792de9263",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOneUpdate (수정)",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\nelse if(msg.payload.type==10)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'sound':msg.payload.sound\n    , 'time':time }} ];\nelse if(msg.payload.type==11)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'knock':msg.payload.knock\n    , 'time':time }} ];\nelse if(msg.payload.type==12)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==13)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time\n     }} ];\nelse if(msg.payload.type==14)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{'ip':msg.payload.ip, 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5), 'time':time }} ];\nelse if(msg.payload.type==15)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==16)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==18)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), \n    'out.0':msg.payload.out.charAt(0), 'out.1':msg.payload.out.charAt(1),'out.2':msg.payload.out.charAt(2), 'out.3':msg.payload.out.charAt(3),\n    'out.4':msg.payload.out.charAt(4), 'out.5':msg.payload.out.charAt(5),'out.6':msg.payload.out.charAt(6), 'out.7':msg.payload.out.charAt(7),\n    'time':time\n     }} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 900,
        "y": 500,
        "wires": [
            [
                "b234e3f0a9e924d5"
            ]
        ]
    },
    {
        "id": "03cb2200063973b3",
        "type": "json",
        "z": "997ba372c49185d6",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 317,
        "y": 460,
        "wires": [
            [
                "21ea7cd1f9606606"
            ]
        ]
    },
    {
        "id": "f21f294ed36ce9ea",
        "type": "http in",
        "z": "997ba372c49185d6",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 220,
        "y": 89,
        "wires": [
            [
                "d9de814f35c3b714"
            ]
        ]
    },
    {
        "id": "fa3b0915bb313113",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "style",
        "field": "payload.style",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "body {\n    background: #eab0dc;\n    font-family: Arial, Helvetica, sans-serif;\n}\ntable, th, td {\n    padding: 2px;\n    border-collapse: collapse;\n    border: 1px solid #dddddd;\n    text-align: center;\n    vertical-align: middle;\n}\n\n\n/* Full-width input fields */\ninput[type=text], input[type=password] {\n  width: 150px;\n  padding: 5px 10px;\n  margin: 8px 0;\n  display: inline-block;\n  border: 1px solid #ccc;\n  box-sizing: border-box;\n}\n\n/* Set a style for all buttons */\nbutton {\n  background-color: #4CAF50; /*Green*/\n  color: white;\n  padding: 14px 20px;\n  margin: 8px 0;\n  border: none;\n  cursor: pointer;\n}\n\n.buttonMenu {\n          padding: 5px 24px;\n          margin-left:20%;\n          background-color:black;\n          border: none;\n          border-color:black;\n          color:white;\n          text-align: left;\n          text-decoration: none;\n          display: inline-block;\n          font-size: 16px;\n          margin: 4px 2px;\n          cursor: pointer;\n        }\n        .sidenav {\n          height: 100%;\n          width: 0;\n          position: fixed;\n          z-index: 1;\n          top: 0;\n          left: 0;\n          background-color: #111;\n          overflow-x: hidden;\n          transition: 0.5s;\n          padding-top: 60px;\n        }\n        .sidenav a {\n          padding: 8px 8px 8px 32px;\n          text-decoration: none;\n          font-size: 18px;\n          color: #818181;\n          display: block;\n                transition: 0.3s;\n        }\n        .sidenav a:hover {\n          color: #f1f1f1;\n        }\n        .sidenav .closebtn {\n          position: absolute;\n          top: 0;\n          right: 25px;\n          font-size: 36px;\n          margin-left: 50px;\n        }\n\n.buttonM {background-color: #ff66cc;color:white; width:100px; height:20px; padding: 0px 0px; font-size: 16px} /* 기기선택 */  \n.buttonL {background-color: #ff66cc;color:white; width:100px; height:25px; padding: 0px 0px; font-size: 16px} /* 선택 */  \n.buttonMenu {background-color: #000000;} \n.button2 {background-color: #008CBA;} /* Blue */\n.button3 {background-color: #f44336;} /* Red */ \n.button4 {background-color: #e7e7e7; color: black;} /* Gray */ \n.button5 {background-color: #555555;} /* Black */\n.button20 {width: 20%;} \n.button-on {border-radius: 100%; padding: 20px; font-size: 18px; margin: 0px 0px; background-color: #4CAF50;}\n.button-off {border-radius: 100%; padding: 20px; font-size: 18px; background-color: #707070;}\n.button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}\n.button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}\n\nbutton:hover {\n  opacity: 0.8;\n}\n\n/* Extra styles for the cancel button */\n.cancelbtn {\n  width: auto;\n  padding: 10px 18px;\n  background-color: #f44336;\n}\n\n/* Center the image and position the close button */\n.imgcontainer {\n  text-align: center;\n  margin: 24px 0 12px 0;\n  position: relative;\n}\n\nimg.avatar {\n  width: 40%;\n  border-radius: 50%;\n}\n\n.container {\n  padding: 16px;\n}\n\nspan.psw {\n  float: right;\n  padding-top: 16px;\n}\n\n/* The Modal (background) */\n.modal {\n  display: none; /* Hidden by default */\n  position: fixed; /* Stay in place */\n  z-index: 1; /* Sit on top */\n  left: 0;\n  top: 0;\n  width: 100%; /* Full width */\n  height: 100%; /* Full height */\n  overflow: auto; /* Enable scroll if needed */\n  background-color: rgb(0,0,0); /* Fallback color */\n  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */\n  padding-top: 60px;\n}\n\n/* Modal Content/Box */\n.modal-content {\n  background-color: #fefefe;\n  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */\n  border: 1px solid #888;\n  width: 80%; /* Could be more or less, depending on screen size */\n}\n\n/* The Close Button (x) */\n.close {\n  position: absolute;\n  right: 25px;\n  top: 0;\n  color: #000;\n  font-size: 35px;\n  font-weight: bold;\n}\n\n.close:hover,\n.close:focus {\n  color: red;\n  cursor: pointer;\n}\n\n/* Add Zoom Animation */\n.animate {\n  -webkit-animation: animatezoom 0.6s;\n  animation: animatezoom 0.6s\n}\n\n@-webkit-keyframes animatezoom {\n  from {-webkit-transform: scale(0)} \n  to {-webkit-transform: scale(1)}\n}\n  \n@keyframes animatezoom {\n  from {transform: scale(0)} \n  to {transform: scale(1)}\n}\n\n/* Change styles for span and cancel button on extra small screens */\n@media screen and (max-width: 300px) {\n  span.psw {\n     display: block;\n     float: none;\n  }\n  .cancelbtn {\n     width: 100%;\n  }\n}",
        "x": 630,
        "y": 90,
        "wires": [
            [
                "baf10810816b488a"
            ]
        ]
    },
    {
        "id": "6138dcad4d1a8d29",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}\n",
        "output": "str",
        "x": 505,
        "y": 90,
        "wires": [
            [
                "fa3b0915bb313113"
            ]
        ]
    },
    {
        "id": "8e75e7292e1db1ac",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 901,
        "y": 90,
        "wires": [
            [
                "3ed0637ce70adaaf"
            ]
        ]
    },
    {
        "id": "baf10810816b488a",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='https://github.com/kdi6033/IoT' target='_blank'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 750,
        "y": 90,
        "wires": [
            [
                "8e75e7292e1db1ac"
            ]
        ]
    },
    {
        "id": "920de5c8df3168de",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "find.toArray",
        "func": "global.set(\"msg_main\",msg);\nmsg.collection = 'localRecord';\nmsg.operation  =  'find.toArray';\nmsg.payload    = { };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 710,
        "y": 200,
        "wires": [
            [
                "baea73383e3da365"
            ]
        ]
    },
    {
        "id": "a3f929cc75f58062",
        "type": "http response",
        "z": "997ba372c49185d6",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1750,
        "y": 200,
        "wires": []
    },
    {
        "id": "40fad226e01359e3",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "0,1,2,3 List (수정)",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i,j;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    s+=\"<tr><th>\";\n    s+=\"<form action='/display'>\";\n        s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n        s+=\"<input type='hidden' name='act' value='1'>\";\n        s+=\"<button type='submit' name='value' value='0' class='button buttonM'>\"+msg.payload[i].model+\"</button></a>\";\n    s+=\"</form>\";\n    s+=\"</th>\";\n    if(msg.payload[i].type==2) {\n        //s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        //s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    else if(msg.payload[i].type==6) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        //s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==9) {\n        //s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==10) {\n        //s+=\"<td>사운드:\"+msg.payload[i].sound+\" \"+msg.payload[i].time+\"</td> \";\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==11) {\n        //s+=\"<td>충격발생:\"+msg.payload[i].time+\"</td> \"\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==12) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==13) {\n        s+=\"<td>\"\n            s+=\"<table align='center' style='margin: 0px auto;'>\";\n            s+=\"<tr>\";\n            s+=\"<td>정지</td>\";\n            s+=\"<td>올림</td>\";\n            s+=\"<td>내림</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==14) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==15) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==16) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==18) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1170,
        "y": 200,
        "wires": [
            [
                "e28043214a1bc439",
                "a25ce25e88426845"
            ]
        ]
    },
    {
        "id": "20fd2565c8925546",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_main\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 980,
        "y": 200,
        "wires": [
            [
                "40fad226e01359e3"
            ]
        ]
    },
    {
        "id": "5cf850dcafa350c0",
        "type": "http in",
        "z": "997ba372c49185d6",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 230,
        "y": 170,
        "wires": [
            [
                "a9a7db96f17ca874"
            ]
        ]
    },
    {
        "id": "4799f2e8c1d549e1",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 54,
        "y": 170,
        "wires": []
    },
    {
        "id": "b71a92267b50e2e7",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "get global",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nmsg.payload.act=global.get(\"act\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1132,
        "y": 150,
        "wires": [
            [
                "1f4e20ed4a9f6230"
            ]
        ]
    },
    {
        "id": "1f4e20ed4a9f6230",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "html 기기 name",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n   <br> &emsp;\n   <table>\n         <th>mac</th> <th>{{{payload.mac}}}</th>\n      </tr>\n      <tr>\n        <th>접속IP</th> <th>{{{payload.ip}}}</th>\n      </tr>\n      <tr>\n        <th>이름</th> \n            <form action=\"/save\" method=\"post\">\n                <input type='hidden' name='mac' value='{{{payload.mac}}}'>\n                <input type='hidden' name='act' value='1'>\n                <th><input type=\"text\" id=\"name\" name=\"name\" value=\"{{{payload.name}}}\"></th>\n                <th><button class='button buttonL' type=\"submit\" style=\"width:50px;\" >저장</button></th>\n            </form> \n      </tr>\n    </table>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 1292,
        "y": 150,
        "wires": [
            [
                "677a45b050b9c605"
            ]
        ]
    },
    {
        "id": "130ae72c189ddb0e",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 702,
        "y": 150,
        "wires": [
            [
                "8959d58187cabc5a"
            ]
        ]
    },
    {
        "id": "677a45b050b9c605",
        "type": "http response",
        "z": "997ba372c49185d6",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1462,
        "y": 150,
        "wires": []
    },
    {
        "id": "a9a7db96f17ca874",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "",
        "func": "global.set(\"msg_display\",msg);\nglobal.set(\"act\",msg.payload.act);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 400,
        "y": 170,
        "wires": [
            [
                "c875a41bdd502173"
            ]
        ]
    },
    {
        "id": "e988e266b8f0bf16",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_display\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 992,
        "y": 150,
        "wires": [
            [
                "b71a92267b50e2e7"
            ]
        ]
    },
    {
        "id": "c875a41bdd502173",
        "type": "switch",
        "z": "997ba372c49185d6",
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
        "x": 550,
        "y": 170,
        "wires": [
            [
                "130ae72c189ddb0e"
            ],
            [],
            [
                "920de5c8df3168de"
            ],
            []
        ]
    },
    {
        "id": "8caa8ae8309251b9",
        "type": "http in",
        "z": "997ba372c49185d6",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 230,
        "y": 280,
        "wires": [
            [
                "17fc6dee1d103a6d"
            ]
        ]
    },
    {
        "id": "cba4b043b125bd4e",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 54,
        "y": 280,
        "wires": []
    },
    {
        "id": "c8c344a966a8bce1",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 540,
        "y": 280,
        "wires": [
            [
                "8392419c11a75c7c"
            ]
        ]
    },
    {
        "id": "7efb92a2f7de0cca",
        "type": "link out",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "448b2a1e4c36b5d7",
            "2044220.61e99de"
        ],
        "x": 955,
        "y": 280,
        "wires": []
    },
    {
        "id": "47530edf2a071d30",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 860,
        "y": 280,
        "wires": [
            [
                "7efb92a2f7de0cca"
            ]
        ]
    },
    {
        "id": "17fc6dee1d103a6d",
        "type": "switch",
        "z": "997ba372c49185d6",
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
        "x": 370,
        "y": 280,
        "wires": [
            [
                "c8c344a966a8bce1"
            ],
            [],
            []
        ]
    },
    {
        "id": "db8a6ad47b172528",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.publicIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1457,
        "y": 80,
        "wires": [
            [
                "0cb72bfbc4c4e20e"
            ]
        ]
    },
    {
        "id": "0cb72bfbc4c4e20e",
        "type": "http response",
        "z": "997ba372c49185d6",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1609,
        "y": 80,
        "wires": []
    },
    {
        "id": "448b2a1e4c36b5d7",
        "type": "link in",
        "z": "997ba372c49185d6",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "3ed0637ce70adaaf",
            "7efb92a2f7de0cca"
        ],
        "x": 1155,
        "y": 80,
        "wires": [
            [
                "0fa01ef7ea61400d"
            ]
        ]
    },
    {
        "id": "3ed0637ce70adaaf",
        "type": "link out",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "448b2a1e4c36b5d7"
        ],
        "x": 1015,
        "y": 90,
        "wires": []
    },
    {
        "id": "dc454fed52302134",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "Act",
        "info": "1: \n",
        "x": 50,
        "y": 340,
        "wires": []
    },
    {
        "id": "4195585ad45a41cf",
        "type": "http in",
        "z": "997ba372c49185d6",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 220,
        "y": 340,
        "wires": [
            []
        ]
    },
    {
        "id": "d9de814f35c3b714",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "",
        "func": "global.set(\"serverIP\",msg.payload.publicIPv4);\n//msg.payload=msg.payload.publicIPv4;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 380,
        "y": 90,
        "wires": [
            [
                "6138dcad4d1a8d29"
            ]
        ]
    },
    {
        "id": "a6aff96b07a8c667",
        "type": "websocket out",
        "z": "997ba372c49185d6",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 1700,
        "y": 440,
        "wires": []
    },
    {
        "id": "e00773f6e3731b55",
        "type": "websocket in",
        "z": "997ba372c49185d6",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 180,
        "y": 560,
        "wires": [
            [
                "c3d28114a2e06192"
            ]
        ]
    },
    {
        "id": "9f72fe9a370b5e1e",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "출력버튼 웹소켙",
        "info": "",
        "x": 180,
        "y": 520,
        "wires": []
    },
    {
        "id": "32121652174dbeca",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "<!DOCTYPE HTML>\n<html>\n<head>\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n    <style> {{{payload.style}}} </style>\n    <script> {{{payload.script}}} </script>\n    <script type=\"text/javascript\"> {{{payload.scriptSocket}}} </script>\n</head>\n<body onload=\"wsConnect();\" onunload=\"ws.disconnect();\">\n    {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>\n\n",
        "x": 1620,
        "y": 200,
        "wires": [
            [
                "a3f929cc75f58062"
            ]
        ]
    },
    {
        "id": "e28043214a1bc439",
        "type": "template",
        "z": "997ba372c49185d6",
        "name": "Web Socket scriptSocket (수정)",
        "field": "payload.scriptSocket",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "        var ws;\n        var wsUri = \"ws:\";\n        var loc = window.location;\n        console.log(loc);\n        if (loc.protocol === \"https:\") { wsUri = \"wss:\"; }\n        // This needs to point to the web socket in the Node-RED flow\n        // ... in this case it's ws/simple\n        wsUri += \"//\" + loc.host + loc.pathname.replace(\"display\",\"ws/simple\");\n\n        function wsConnect() {\n            console.log(\"connect\",wsUri);\n            ws = new WebSocket(wsUri);\n            //var line = \"\";    // either uncomment this for a building list of messages\n            ws.onmessage = function(msg) {\n                var line = \"\";  // or uncomment this to overwrite the existing message\n                // parse the incoming message as a JSON object\n                var data = JSON.parse(msg.data);\n                //var data = msg.data;\n                console.log(data);\n                if(data.type==2) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"EC:\"+data.ec;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"PH:\"+data.ph;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                if(data.type==3) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"CO2:\"+data.co2;\n                }\n                if(data.type==4) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.mo+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.tem+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                if(data.type==5) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==6) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:1}\\\");'></button>\";\n                }\n                else if(data.type==7) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                }\n                else if(data.type==8) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"대기압:\"+data.pres;\n                }\n                else if(data.type==9) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                else if(data.type==10) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"사운드:\"+data.sound;\n                }\n                else if(data.type==11) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"충격발생:\"+data.knock;\n                }\n                else if(data.type==12) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:1}\\\");'></button>\";\n                }\n                if(data.type==13) {\n                    if(data.state==0) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==1) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==2) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n\n                 }\n                 else if(data.type==14) {\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:1}\\\");'></button>\";\n                    \n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                }\n                else if(data.type==15) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"연기농도:\"+data.gas;\n                }\n                else if(data.type==16) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"메탄농도:\"+data.gas;\n                }\n                else if(data.type==18) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:1}\\\");'></button>\";\n                    if(data.out[6]=='1')\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:1}\\\");'></button>\";\n                    if(data.out[7]=='1')\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:1}\\\");'></button>\";\n                }\n            }\n            ws.onopen = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"connected\";\n                //ws.send(\"Open for data\");\n                console.log(\"connected\");\n            }\n            ws.onclose = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"not connected\";\n                // in case of lost connection tries to reconnect every 3 secs\n                setTimeout(wsConnect,3000);\n            }\n        }\n        \n        function doit(m) {\n            if (ws) { ws.send(m); }\n        }\n",
        "x": 1410,
        "y": 200,
        "wires": [
            [
                "32121652174dbeca"
            ]
        ]
    },
    {
        "id": "c3d28114a2e06192",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "",
        "func": "var msg = {payload:msg.payload.replace(/['^']/g, \"\\\"\")};\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 360,
        "y": 560,
        "wires": [
            [
                "da894aec890f8ab3",
                "fd15ce2db5ed3498"
            ]
        ]
    },
    {
        "id": "da894aec890f8ab3",
        "type": "json",
        "z": "997ba372c49185d6",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 510,
        "y": 560,
        "wires": [
            [
                "4ca180eeb0d6c96a"
            ]
        ]
    },
    {
        "id": "4ca180eeb0d6c96a",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOneUpdate (출력있는 경우만 수정)",
        "func": "global.set(\"msg_in\",msg);\nvar time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\n\nif(msg.payload.type==6 ||msg.payload.type==12 || msg.payload.type==14 || msg.payload.type==18) {\n    if(msg.payload.outNo == 0)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.0':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 1)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.1':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 2)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.2':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 3)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.3':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 4)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.4':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 5)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.5':msg.payload.value, 'time':time }} ];\n}\nelse if(msg.payload.type==13) \n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time }} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 750,
        "y": 560,
        "wires": [
            [
                "a40deae8342285ce"
            ]
        ]
    },
    {
        "id": "a2660f0a80bad9ff",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "입력버튼 웹소켙",
        "info": "",
        "x": 180,
        "y": 420,
        "wires": []
    },
    {
        "id": "fd15ce2db5ed3498",
        "type": "mqtt out",
        "z": "997ba372c49185d6",
        "name": "",
        "topic": "/i2r/inTopic",
        "qos": "0",
        "retain": "false",
        "respTopic": "",
        "contentType": "",
        "userProps": "",
        "correl": "",
        "expiry": "",
        "broker": "f15d66b3.c7edf8",
        "x": 530,
        "y": 620,
        "wires": []
    },
    {
        "id": "57cb9bac99aef8ea",
        "type": "link out",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "1730d76738b6f4de"
        ],
        "x": 1008,
        "y": 447,
        "wires": []
    },
    {
        "id": "b234e3f0a9e924d5",
        "type": "link out",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "1730d76738b6f4de"
        ],
        "x": 1035,
        "y": 500,
        "wires": []
    },
    {
        "id": "a40deae8342285ce",
        "type": "link out",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "1730d76738b6f4de"
        ],
        "x": 935,
        "y": 560,
        "wires": []
    },
    {
        "id": "1730d76738b6f4de",
        "type": "link in",
        "z": "997ba372c49185d6",
        "name": "",
        "links": [
            "57cb9bac99aef8ea",
            "b234e3f0a9e924d5",
            "a40deae8342285ce"
        ],
        "x": 1115,
        "y": 440,
        "wires": [
            [
                "f93ae4da5a88d697"
            ]
        ]
    },
    {
        "id": "1f376d33aada9066",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOne",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1360,
        "y": 440,
        "wires": [
            [
                "d79f5e81b0f46ddc"
            ]
        ]
    },
    {
        "id": "345e52cb42ca1abd",
        "type": "subflow:10a86fdc.efc16",
        "z": "997ba372c49185d6",
        "name": "Iterate",
        "env": [],
        "x": 1490,
        "y": 340,
        "wires": [
            [
                "adac4dda7cbddddc",
                "c107a54c6b5c5157"
            ],
            []
        ]
    },
    {
        "id": "c107a54c6b5c5157",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1480,
        "y": 280,
        "wires": [
            [
                "345e52cb42ca1abd"
            ]
        ]
    },
    {
        "id": "adac4dda7cbddddc",
        "type": "function",
        "z": "997ba372c49185d6",
        "name": "findOne",
        "func": "var newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1660,
        "y": 340,
        "wires": [
            [
                "d79f5e81b0f46ddc"
            ]
        ]
    },
    {
        "id": "a25ce25e88426845",
        "type": "delay",
        "z": "997ba372c49185d6",
        "name": "",
        "pauseType": "delay",
        "timeout": "1",
        "timeoutUnits": "seconds",
        "rate": "1",
        "nbRateUnits": "1",
        "rateUnits": "second",
        "randomFirst": "1",
        "randomLast": "5",
        "randomUnits": "seconds",
        "drop": false,
        "allowrate": false,
        "outputs": 1,
        "x": 1340,
        "y": 340,
        "wires": [
            [
                "345e52cb42ca1abd"
            ]
        ]
    },
    {
        "id": "9cc202b62d352e2a",
        "type": "comment",
        "z": "997ba372c49185d6",
        "name": "웹페이지 초기화",
        "info": "",
        "x": 1300,
        "y": 280,
        "wires": []
    },
    {
        "id": "89461b3acde25e3a",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 600,
        "y": 460,
        "wires": [
            [
                "6db97a988ae1f91a"
            ]
        ]
    },
    {
        "id": "f93ae4da5a88d697",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1210,
        "y": 440,
        "wires": [
            [
                "1f376d33aada9066"
            ]
        ]
    },
    {
        "id": "baea73383e3da365",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 850,
        "y": 200,
        "wires": [
            [
                "20fd2565c8925546"
            ]
        ]
    },
    {
        "id": "8959d58187cabc5a",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 842,
        "y": 150,
        "wires": [
            [
                "e988e266b8f0bf16"
            ]
        ]
    },
    {
        "id": "8392419c11a75c7c",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 710,
        "y": 280,
        "wires": [
            [
                "47530edf2a071d30"
            ]
        ]
    },
    {
        "id": "d79f5e81b0f46ddc",
        "type": "mongodb2 in",
        "z": "997ba372c49185d6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 1520,
        "y": 440,
        "wires": [
            [
                "a6aff96b07a8c667"
            ]
        ]
    },
    {
        "id": "0fa01ef7ea61400d",
        "type": "ip",
        "z": "997ba372c49185d6",
        "name": "node-red-contrib-ip",
        "https": false,
        "timeout": "5000",
        "internalIPv4": true,
        "internalIPv6": true,
        "publicIPv4": true,
        "publicIPv6": false,
        "x": 1290,
        "y": 80,
        "wires": [
            [
                "db8a6ad47b172528"
            ]
        ]
    },
    {
        "id": "f15d66b3.c7edf8",
        "type": "mqtt-broker",
        "name": "",
        "broker": "",
        "port": "8883",
        "tls": "509a45a9.089bbc",
        "clientid": "",
        "autoConnect": true,
        "usetls": true,
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
        "id": "bf4882ef33e69fa3",
        "type": "websocket-listener",
        "path": "/ws/simple",
        "wholemsg": "false"
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
        "uri": "mongodb://3.143.254.163:27000/local",
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
        "certname": "",
        "keyname": "",
        "caname": "",
        "servername": "",
        "verifyservercert": true,
        "alpnprotocol": ""
    }
]
```
