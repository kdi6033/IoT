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
        "id": "25c4c21006073cf6",
        "type": "tab",
        "label": "Light Server",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "013e217d8fa3cf4b",
        "type": "mqtt in",
        "z": "25c4c21006073cf6",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 190,
        "y": 500,
        "wires": [
            [
                "6e1eaa52dcc22487",
                "3e391e66171329ef"
            ]
        ]
    },
    {
        "id": "078dc1e5215e4f91",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 480,
        "y": 500,
        "wires": [
            [
                "a9831b15bd87dca5"
            ]
        ]
    },
    {
        "id": "6ad709dc75bb6bf0",
        "type": "switch",
        "z": "25c4c21006073cf6",
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
        "x": 750,
        "y": 500,
        "wires": [
            [
                "2f2481e8fbb05bf6"
            ],
            [
                "123f5c0902679df6"
            ]
        ]
    },
    {
        "id": "2f2481e8fbb05bf6",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "insert one (수정)",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar model=[];\nmodel[2]='PE-300';\nmodel[3]='키우미';\nmodel[4]='KSM-8900';\nmodel[5]='simpleTH';\nmodel[6]='XEX-DR14E';\nmodel[7]='DHT11';\nmodel[8]='RY-WS301';\nmodel[9]='MP-508E';\nmodel[10]='사운드';\nmodel[11]='충격센서';\nmodel[12]='PLC-RS485';\nmodel[13]='차양모터';\nmodel[14]='MASTER-K120s';\nmodel[15]='smoke';\nmodel[16]='methane';\nmodel[17]='Light sensor';\nmodel[18]='PLC-RS485-8ch';\nmodel[20]='Leaf Humidity';\nmodel[21]='RK520-01';\nmodel[27]='SWWS-7320';\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nif(msg.payload.type==6)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0] };\nelse if(msg.payload.type==12 || msg.payload.type==14)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0] };\nelse if(msg.payload.type==18)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0,0,0] };\nelse\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 920,
        "y": 480,
        "wires": [
            [
                "119b9653808d890a"
            ]
        ]
    },
    {
        "id": "123f5c0902679df6",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOneUpdate (수정)",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\nelse if(msg.payload.type==10)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'sound':msg.payload.sound\n    , 'time':time }} ];\nelse if(msg.payload.type==11)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'knock':msg.payload.knock\n    , 'time':time }} ];\nelse if(msg.payload.type==12)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==13)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time\n     }} ];\nelse if(msg.payload.type==14)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{'ip':msg.payload.ip, 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5), 'time':time }} ];\nelse if(msg.payload.type==15)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==16)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\n    else if(msg.payload.type==17)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'lux':msg.payload.lux, 'time':time}} ];\nelse if(msg.payload.type==18)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), \n    'out.0':msg.payload.out.charAt(0), 'out.1':msg.payload.out.charAt(1),'out.2':msg.payload.out.charAt(2), 'out.3':msg.payload.out.charAt(3),\n    'out.4':msg.payload.out.charAt(4), 'out.5':msg.payload.out.charAt(5),'out.6':msg.payload.out.charAt(6), 'out.7':msg.payload.out.charAt(7),\n    'time':time\n     }} ];\n      else if(msg.payload.type==20)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'hum':msg.payload.hum, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==21)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==27)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'windspeed':msg.payload.windspeed,'time':time}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 920,
        "y": 540,
        "wires": [
            [
                "4bf85b0e23c9ab84"
            ]
        ]
    },
    {
        "id": "6e1eaa52dcc22487",
        "type": "json",
        "z": "25c4c21006073cf6",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 350,
        "y": 500,
        "wires": [
            [
                "078dc1e5215e4f91"
            ]
        ]
    },
    {
        "id": "a7f7a21c40b31f1d",
        "type": "http in",
        "z": "25c4c21006073cf6",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 240,
        "y": 129,
        "wires": [
            [
                "4621b03cecc1c247"
            ]
        ]
    },
    {
        "id": "63495cab021e8e69",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "style",
        "field": "payload.style",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "body {\n    background: #eab0dc;\n    font-family: Arial, Helvetica, sans-serif;\n}\ntable, th, td {\n    padding: 2px;\n    border-collapse: collapse;\n    border: 1px solid #dddddd;\n    text-align: center;\n    vertical-align: middle;\n}\n\n\n/* Full-width input fields */\ninput[type=text], input[type=password] {\n  width: 150px;\n  padding: 5px 10px;\n  margin: 8px 0;\n  display: inline-block;\n  border: 1px solid #ccc;\n  box-sizing: border-box;\n}\n\n/* Set a style for all buttons */\nbutton {\n  background-color: #4CAF50; /*Green*/\n  color: white;\n  padding: 14px 20px;\n  margin: 8px 0;\n  border: none;\n  cursor: pointer;\n}\n\n.buttonMenu {\n          padding: 5px 24px;\n          margin-left:20%;\n          background-color:black;\n          border: none;\n          border-color:black;\n          color:white;\n          text-align: left;\n          text-decoration: none;\n          display: inline-block;\n          font-size: 16px;\n          margin: 4px 2px;\n          cursor: pointer;\n        }\n        .sidenav {\n          height: 100%;\n          width: 0;\n          position: fixed;\n          z-index: 1;\n          top: 0;\n          left: 0;\n          background-color: #111;\n          overflow-x: hidden;\n          transition: 0.5s;\n          padding-top: 60px;\n        }\n        .sidenav a {\n          padding: 8px 8px 8px 32px;\n          text-decoration: none;\n          font-size: 18px;\n          color: #818181;\n          display: block;\n                transition: 0.3s;\n        }\n        .sidenav a:hover {\n          color: #f1f1f1;\n        }\n        .sidenav .closebtn {\n          position: absolute;\n          top: 0;\n          right: 25px;\n          font-size: 36px;\n          margin-left: 50px;\n        }\n\n.buttonM {background-color: #ff66cc;color:white; width:100px; height:20px; padding: 0px 0px; font-size: 16px} /* 기기선택 */  \n.buttonL {background-color: #ff66cc;color:white; width:100px; height:25px; padding: 0px 0px; font-size: 16px} /* 선택 */  \n.buttonMenu {background-color: #000000;} \n.button2 {background-color: #008CBA;} /* Blue */\n.button3 {background-color: #f44336;} /* Red */ \n.button4 {background-color: #e7e7e7; color: black;} /* Gray */ \n.button5 {background-color: #555555;} /* Black */\n.button20 {width: 20%;} \n.button-on {border-radius: 100%; padding: 20px; font-size: 18px; margin: 0px 0px; background-color: #4CAF50;}\n.button-off {border-radius: 100%; padding: 20px; font-size: 18px; background-color: #707070;}\n.button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}\n.button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}\n\nbutton:hover {\n  opacity: 0.8;\n}\n\n/* Extra styles for the cancel button */\n.cancelbtn {\n  width: auto;\n  padding: 10px 18px;\n  background-color: #f44336;\n}\n\n/* Center the image and position the close button */\n.imgcontainer {\n  text-align: center;\n  margin: 24px 0 12px 0;\n  position: relative;\n}\n\nimg.avatar {\n  width: 40%;\n  border-radius: 50%;\n}\n\n.container {\n  padding: 16px;\n}\n\nspan.psw {\n  float: right;\n  padding-top: 16px;\n}\n\n/* The Modal (background) */\n.modal {\n  display: none; /* Hidden by default */\n  position: fixed; /* Stay in place */\n  z-index: 1; /* Sit on top */\n  left: 0;\n  top: 0;\n  width: 100%; /* Full width */\n  height: 100%; /* Full height */\n  overflow: auto; /* Enable scroll if needed */\n  background-color: rgb(0,0,0); /* Fallback color */\n  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */\n  padding-top: 60px;\n}\n\n/* Modal Content/Box */\n.modal-content {\n  background-color: #fefefe;\n  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */\n  border: 1px solid #888;\n  width: 80%; /* Could be more or less, depending on screen size */\n}\n\n/* The Close Button (x) */\n.close {\n  position: absolute;\n  right: 25px;\n  top: 0;\n  color: #000;\n  font-size: 35px;\n  font-weight: bold;\n}\n\n.close:hover,\n.close:focus {\n  color: red;\n  cursor: pointer;\n}\n\n/* Add Zoom Animation */\n.animate {\n  -webkit-animation: animatezoom 0.6s;\n  animation: animatezoom 0.6s\n}\n\n@-webkit-keyframes animatezoom {\n  from {-webkit-transform: scale(0)} \n  to {-webkit-transform: scale(1)}\n}\n  \n@keyframes animatezoom {\n  from {transform: scale(0)} \n  to {transform: scale(1)}\n}\n\n/* Change styles for span and cancel button on extra small screens */\n@media screen and (max-width: 300px) {\n  span.psw {\n     display: block;\n     float: none;\n  }\n  .cancelbtn {\n     width: 100%;\n  }\n}",
        "x": 650,
        "y": 130,
        "wires": [
            [
                "e1599a1fac6f4e13"
            ]
        ]
    },
    {
        "id": "8e434affbb0adb0c",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}\n",
        "output": "str",
        "x": 525,
        "y": 130,
        "wires": [
            [
                "63495cab021e8e69"
            ]
        ]
    },
    {
        "id": "2cf4d5c6b3720cef",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 921,
        "y": 130,
        "wires": [
            [
                "74852b1609853e02"
            ]
        ]
    },
    {
        "id": "e1599a1fac6f4e13",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='https://github.com/kdi6033/IoT' target='_blank'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 770,
        "y": 130,
        "wires": [
            [
                "2cf4d5c6b3720cef"
            ]
        ]
    },
    {
        "id": "f38f4a8628892f4d",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "find.toArray",
        "func": "global.set(\"msg_main\",msg);\nmsg.collection = 'localRecord';\nmsg.operation  =  'find.toArray';\nmsg.payload    = { };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 730,
        "y": 240,
        "wires": [
            [
                "e561da8b1c50f7f8"
            ]
        ]
    },
    {
        "id": "0ff4940d490bf99d",
        "type": "http response",
        "z": "25c4c21006073cf6",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1770,
        "y": 240,
        "wires": []
    },
    {
        "id": "5115bdac1b093bc5",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "0,1,2,3 List (수정)",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i,j;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    s+=\"<tr><th>\";\n    s+=\"<form action='/display'>\";\n        s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n        s+=\"<input type='hidden' name='act' value='1'>\";\n        s+=\"<button type='submit' name='value' value='0' class='button buttonM'>\"+msg.payload[i].model+\"</button></a>\";\n    s+=\"</form>\";\n    s+=\"</th>\";\n    if(msg.payload[i].type==2) {\n        //s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        //s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    else if(msg.payload[i].type==6) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        //s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==9) {\n        //s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==10) {\n        //s+=\"<td>사운드:\"+msg.payload[i].sound+\" \"+msg.payload[i].time+\"</td> \";\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==11) {\n        //s+=\"<td>충격발생:\"+msg.payload[i].time+\"</td> \"\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==12) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==13) {\n        s+=\"<td>\"\n            s+=\"<table align='center' style='margin: 0px auto;'>\";\n            s+=\"<tr>\";\n            s+=\"<td>정지</td>\";\n            s+=\"<td>올림</td>\";\n            s+=\"<td>내림</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==14) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==15) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==16) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n     else if(msg.payload[i].type==17) {\n        //s+=\"<td>lux:\"+msg.payload[i].lux+\" </td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==18) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==20) {\n        //s+=\"<td>hum:\"+msg.payload[i].hum+\"   온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==21) {\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==27) {\n        //s+=\"<td>windspeed:\"+msg.payload[i].windspeed+\"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n     }\n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1190,
        "y": 240,
        "wires": [
            [
                "7bad2ada94887015",
                "32196219e341e50f"
            ]
        ]
    },
    {
        "id": "a4ec770b26d8d842",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_main\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1000,
        "y": 240,
        "wires": [
            [
                "5115bdac1b093bc5"
            ]
        ]
    },
    {
        "id": "b9b7264d699cd1d7",
        "type": "http in",
        "z": "25c4c21006073cf6",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 250,
        "y": 210,
        "wires": [
            [
                "f8245f1f7b9b246f"
            ]
        ]
    },
    {
        "id": "226574474470a925",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 74,
        "y": 210,
        "wires": []
    },
    {
        "id": "828bbbf026621f78",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "get global",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nmsg.payload.act=global.get(\"act\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1152,
        "y": 190,
        "wires": [
            [
                "f1344771bc08860b"
            ]
        ]
    },
    {
        "id": "f1344771bc08860b",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "html 기기 name",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n   <br> &emsp;\n   <table>\n         <th>mac</th> <th>{{{payload.mac}}}</th>\n      </tr>\n      <tr>\n        <th>접속IP</th> <th>{{{payload.ip}}}</th>\n      </tr>\n      <tr>\n        <th>이름</th> \n            <form action=\"/save\" method=\"post\">\n                <input type='hidden' name='mac' value='{{{payload.mac}}}'>\n                <input type='hidden' name='act' value='1'>\n                <th><input type=\"text\" id=\"name\" name=\"name\" value=\"{{{payload.name}}}\"></th>\n                <th><button class='button buttonL' type=\"submit\" style=\"width:50px;\" >저장</button></th>\n            </form> \n      </tr>\n    </table>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 1312,
        "y": 190,
        "wires": [
            [
                "e50fdd976ec4c235"
            ]
        ]
    },
    {
        "id": "8a3838dc9012621a",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 722,
        "y": 190,
        "wires": [
            [
                "72858b0be67b3a6b"
            ]
        ]
    },
    {
        "id": "e50fdd976ec4c235",
        "type": "http response",
        "z": "25c4c21006073cf6",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1482,
        "y": 190,
        "wires": []
    },
    {
        "id": "f8245f1f7b9b246f",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "",
        "func": "global.set(\"msg_display\",msg);\nglobal.set(\"act\",msg.payload.act);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 420,
        "y": 210,
        "wires": [
            [
                "d23d3af8a5d3d35d"
            ]
        ]
    },
    {
        "id": "0cce6bd63465f7ef",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_display\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1012,
        "y": 190,
        "wires": [
            [
                "828bbbf026621f78"
            ]
        ]
    },
    {
        "id": "d23d3af8a5d3d35d",
        "type": "switch",
        "z": "25c4c21006073cf6",
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
        "x": 570,
        "y": 210,
        "wires": [
            [
                "8a3838dc9012621a"
            ],
            [],
            [
                "f38f4a8628892f4d"
            ],
            []
        ]
    },
    {
        "id": "a7de9665b2f7a140",
        "type": "http in",
        "z": "25c4c21006073cf6",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 250,
        "y": 320,
        "wires": [
            [
                "18d9046bec3f0654"
            ]
        ]
    },
    {
        "id": "1d896f0cdac0f008",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 74,
        "y": 320,
        "wires": []
    },
    {
        "id": "ad5ffb5343840a32",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 560,
        "y": 320,
        "wires": [
            [
                "694eb15c86ac6e21"
            ]
        ]
    },
    {
        "id": "ad92f351109259cc",
        "type": "link out",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "c6e28cf8d11682b1",
            "2044220.61e99de"
        ],
        "x": 975,
        "y": 320,
        "wires": []
    },
    {
        "id": "b56a1302e3cef9f7",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 880,
        "y": 320,
        "wires": [
            [
                "ad92f351109259cc"
            ]
        ]
    },
    {
        "id": "18d9046bec3f0654",
        "type": "switch",
        "z": "25c4c21006073cf6",
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
        "x": 390,
        "y": 320,
        "wires": [
            [
                "ad5ffb5343840a32"
            ],
            [],
            []
        ]
    },
    {
        "id": "41262f1a92dd1f66",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.publicIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1477,
        "y": 120,
        "wires": [
            [
                "d515dfe1a534e896"
            ]
        ]
    },
    {
        "id": "d515dfe1a534e896",
        "type": "http response",
        "z": "25c4c21006073cf6",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1629,
        "y": 120,
        "wires": []
    },
    {
        "id": "c6e28cf8d11682b1",
        "type": "link in",
        "z": "25c4c21006073cf6",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "74852b1609853e02",
            "ad92f351109259cc"
        ],
        "x": 1175,
        "y": 120,
        "wires": [
            [
                "6d613dc1fc11602a"
            ]
        ]
    },
    {
        "id": "74852b1609853e02",
        "type": "link out",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "c6e28cf8d11682b1"
        ],
        "x": 1035,
        "y": 130,
        "wires": []
    },
    {
        "id": "b5b0be17391cadf0",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "Act",
        "info": "1: \n",
        "x": 70,
        "y": 380,
        "wires": []
    },
    {
        "id": "793d7fdcfa95adf2",
        "type": "http in",
        "z": "25c4c21006073cf6",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 240,
        "y": 380,
        "wires": [
            []
        ]
    },
    {
        "id": "4621b03cecc1c247",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "",
        "func": "global.set(\"serverIP\",msg.payload.publicIPv4);\n//msg.payload=msg.payload.publicIPv4;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 400,
        "y": 130,
        "wires": [
            [
                "8e434affbb0adb0c"
            ]
        ]
    },
    {
        "id": "3ef4b0ed6679a58e",
        "type": "websocket out",
        "z": "25c4c21006073cf6",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 1720,
        "y": 480,
        "wires": []
    },
    {
        "id": "c2b9ecb374521bd8",
        "type": "websocket in",
        "z": "25c4c21006073cf6",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 200,
        "y": 600,
        "wires": [
            [
                "3be37501686bad31"
            ]
        ]
    },
    {
        "id": "ba996a576172ecee",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "출력버튼 웹소켙",
        "info": "",
        "x": 200,
        "y": 560,
        "wires": []
    },
    {
        "id": "71e7c7261d8a5576",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "<!DOCTYPE HTML>\n<html>\n<head>\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n    <style> {{{payload.style}}} </style>\n    <script> {{{payload.script}}} </script>\n    <script type=\"text/javascript\"> {{{payload.scriptSocket}}} </script>\n</head>\n<body onload=\"wsConnect();\" onunload=\"ws.disconnect();\">\n    {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>\n\n",
        "x": 1640,
        "y": 240,
        "wires": [
            [
                "0ff4940d490bf99d"
            ]
        ]
    },
    {
        "id": "7bad2ada94887015",
        "type": "template",
        "z": "25c4c21006073cf6",
        "name": "Web Socket scriptSocket (수정)",
        "field": "payload.scriptSocket",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "        var ws;\n        var wsUri = \"ws:\";\n        var loc = window.location;\n        console.log(loc);\n        if (loc.protocol === \"https:\") { wsUri = \"wss:\"; }\n        // This needs to point to the web socket in the Node-RED flow\n        // ... in this case it's ws/simple\n        wsUri += \"//\" + loc.host + loc.pathname.replace(\"display\",\"ws/simple\");\n\n        function wsConnect() {\n            console.log(\"connect\",wsUri);\n            ws = new WebSocket(wsUri);\n            //var line = \"\";    // either uncomment this for a building list of messages\n            ws.onmessage = function(msg) {\n                var line = \"\";  // or uncomment this to overwrite the existing message\n                // parse the incoming message as a JSON object\n                var data = JSON.parse(msg.data);\n                //var data = msg.data;\n                console.log(data);\n                if(data.type==2) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"EC:\"+data.ec;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"PH:\"+data.ph;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                if(data.type==3) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"CO2:\"+data.co2;\n                }\n                if(data.type==4) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.mo+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.tem+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                if(data.type==5) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==6) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:1}\\\");'></button>\";\n                }\n                else if(data.type==7) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                }\n                else if(data.type==8) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"대기압:\"+data.pres;\n                }\n                else if(data.type==9) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                else if(data.type==10) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"사운드:\"+data.sound;\n                }\n                else if(data.type==11) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"충격발생:\"+data.knock;\n                }\n                else if(data.type==12) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:1}\\\");'></button>\";\n                }\n                if(data.type==13) {\n                    if(data.state==0) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==1) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==2) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n\n                 }\n                 else if(data.type==14) {\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:1}\\\");'></button>\";\n                    \n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                }\n                else if(data.type==15) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"연기농도:\"+data.gas;\n                }\n                else if(data.type==16) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"메탄농도:\"+data.gas;\n                }\n                 else if(data.type==17) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"Lux:\"+data.lux;\n                }\n                else if(data.type==18) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:1}\\\");'></button>\";\n                    if(data.out[6]=='1')\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:1}\\\");'></button>\";\n                    if(data.out[7]=='1')\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:1}\\\");'></button>\";\n                }\n                else if(data.type==20) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"HUM:\"+data.hum;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==21) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                 else if(data.type==27) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"풍속:\"+data.windspeed;\n            }\n            }\n            ws.onopen = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"connected\";\n                //ws.send(\"Open for data\");\n                console.log(\"connected\");\n            }\n            ws.onclose = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"not connected\";\n                // in case of lost connection tries to reconnect every 3 secs\n                setTimeout(wsConnect,3000);\n            }\n        }\n        \n        function doit(m) {\n            if (ws) { ws.send(m); }\n        }\n",
        "x": 1430,
        "y": 240,
        "wires": [
            [
                "71e7c7261d8a5576"
            ]
        ]
    },
    {
        "id": "3be37501686bad31",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "",
        "func": "var msg = {payload:msg.payload.replace(/['^']/g, \"\\\"\")};\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 380,
        "y": 600,
        "wires": [
            [
                "0403876b2a5ed5cf",
                "51de87bb84e92dde"
            ]
        ]
    },
    {
        "id": "0403876b2a5ed5cf",
        "type": "json",
        "z": "25c4c21006073cf6",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 530,
        "y": 600,
        "wires": [
            [
                "a043992a81ecbfdb"
            ]
        ]
    },
    {
        "id": "a043992a81ecbfdb",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOneUpdate (출력있는 경우만 수정)",
        "func": "global.set(\"msg_in\",msg);\nvar time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\n\nif(msg.payload.type==6 ||msg.payload.type==12 || msg.payload.type==14 || msg.payload.type==18) {\n    if(msg.payload.outNo == 0)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.0':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 1)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.1':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 2)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.2':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 3)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.3':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 4)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.4':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 5)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.5':msg.payload.value, 'time':time }} ];\n\n    else if(msg.payload.outNo == 17)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.17':msg.payload.value, 'time':time }} ];\n}\nelse if(msg.payload.type==13) \n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time }} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 600,
        "wires": [
            [
                "cb7d331b47c7630d"
            ]
        ]
    },
    {
        "id": "e381dceacecdde77",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "입력버튼 웹소켙",
        "info": "",
        "x": 200,
        "y": 460,
        "wires": []
    },
    {
        "id": "51de87bb84e92dde",
        "type": "mqtt out",
        "z": "25c4c21006073cf6",
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
        "x": 550,
        "y": 660,
        "wires": []
    },
    {
        "id": "119b9653808d890a",
        "type": "link out",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "76cc99f1f25484d1"
        ],
        "x": 1055,
        "y": 480,
        "wires": []
    },
    {
        "id": "4bf85b0e23c9ab84",
        "type": "link out",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "76cc99f1f25484d1"
        ],
        "x": 1055,
        "y": 540,
        "wires": []
    },
    {
        "id": "cb7d331b47c7630d",
        "type": "link out",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "76cc99f1f25484d1"
        ],
        "x": 955,
        "y": 600,
        "wires": []
    },
    {
        "id": "76cc99f1f25484d1",
        "type": "link in",
        "z": "25c4c21006073cf6",
        "name": "",
        "links": [
            "119b9653808d890a",
            "4bf85b0e23c9ab84",
            "cb7d331b47c7630d"
        ],
        "x": 1135,
        "y": 480,
        "wires": [
            [
                "5a135241644a33c2"
            ]
        ]
    },
    {
        "id": "dd593862be8d4d79",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOne",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1380,
        "y": 480,
        "wires": [
            [
                "bdca3da9180806c9"
            ]
        ]
    },
    {
        "id": "89e5ce0200800e37",
        "type": "subflow:10a86fdc.efc16",
        "z": "25c4c21006073cf6",
        "name": "Iterate",
        "env": [],
        "x": 1510,
        "y": 380,
        "wires": [
            [
                "614189f8d4131f25",
                "631f55321b0a2094"
            ],
            []
        ]
    },
    {
        "id": "631f55321b0a2094",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1500,
        "y": 320,
        "wires": [
            [
                "89e5ce0200800e37"
            ]
        ]
    },
    {
        "id": "614189f8d4131f25",
        "type": "function",
        "z": "25c4c21006073cf6",
        "name": "findOne",
        "func": "var newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1680,
        "y": 380,
        "wires": [
            [
                "bdca3da9180806c9"
            ]
        ]
    },
    {
        "id": "32196219e341e50f",
        "type": "delay",
        "z": "25c4c21006073cf6",
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
        "x": 1360,
        "y": 380,
        "wires": [
            [
                "89e5ce0200800e37"
            ]
        ]
    },
    {
        "id": "6c288bc8eabc5493",
        "type": "comment",
        "z": "25c4c21006073cf6",
        "name": "웹페이지 초기화",
        "info": "",
        "x": 1320,
        "y": 320,
        "wires": []
    },
    {
        "id": "a9831b15bd87dca5",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 620,
        "y": 500,
        "wires": [
            [
                "6ad709dc75bb6bf0"
            ]
        ]
    },
    {
        "id": "5a135241644a33c2",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1230,
        "y": 480,
        "wires": [
            [
                "dd593862be8d4d79"
            ]
        ]
    },
    {
        "id": "e561da8b1c50f7f8",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 870,
        "y": 240,
        "wires": [
            [
                "a4ec770b26d8d842"
            ]
        ]
    },
    {
        "id": "72858b0be67b3a6b",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 862,
        "y": 190,
        "wires": [
            [
                "0cce6bd63465f7ef"
            ]
        ]
    },
    {
        "id": "694eb15c86ac6e21",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 730,
        "y": 320,
        "wires": [
            [
                "b56a1302e3cef9f7"
            ]
        ]
    },
    {
        "id": "bdca3da9180806c9",
        "type": "mongodb2 in",
        "z": "25c4c21006073cf6",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 1540,
        "y": 480,
        "wires": [
            [
                "3ef4b0ed6679a58e"
            ]
        ]
    },
    {
        "id": "6d613dc1fc11602a",
        "type": "ip",
        "z": "25c4c21006073cf6",
        "name": "node-red-contrib-ip",
        "https": false,
        "timeout": "5000",
        "internalIPv4": true,
        "internalIPv6": true,
        "publicIPv4": true,
        "publicIPv6": false,
        "x": 1310,
        "y": 120,
        "wires": [
            [
                "41262f1a92dd1f66"
            ]
        ]
    },
    {
        "id": "3e391e66171329ef",
        "type": "debug",
        "z": "25c4c21006073cf6",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 400,
        "y": 420,
        "wires": []
    },
    {
        "id": "f15d66b3.c7edf8",
        "type": "mqtt-broker",
        "name": "",
        "broker": "a35y3l4o97ieag-ats.iot.us-east-2.amazonaws.com",
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
        "certname": "feeadb9b6d971595086298d8f4e8b2fdb812b9eb1823734de5527829e5de0cc4-certificate.pem.crt",
        "keyname": "feeadb9b6d971595086298d8f4e8b2fdb812b9eb1823734de5527829e5de0cc4-private.pem.key",
        "caname": "AmazonRootCA1.pem",
        "servername": "",
        "verifyservercert": true,
        "alpnprotocol": ""
    }
]

```
