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
        "id": "6e961799d6ba7d71",
        "type": "mqtt in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "x": 290,
        "y": 540,
        "wires": [
            [
                "3a4c7c36854f8009"
            ]
        ]
    },
    {
        "id": "6a0e8b52acb5efda",
        "type": "subflow:10a86fdc.efc16",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Iterate",
        "env": [],
        "x": 750,
        "y": 460,
        "wires": [
            [
                "be7de8bbf2d54276",
                "ee15f14ab317b333"
            ],
            []
        ]
    },
    {
        "id": "ee15f14ab317b333",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 748,
        "y": 400,
        "wires": [
            [
                "6a0e8b52acb5efda"
            ]
        ]
    },
    {
        "id": "28c2cec2a7283571",
        "type": "udp out",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "addr": "",
        "iface": "",
        "port": "",
        "ipv": "udp4",
        "outport": "",
        "base64": false,
        "multicast": "false",
        "x": 1214,
        "y": 456,
        "wires": []
    },
    {
        "id": "be7de8bbf2d54276",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "var server=msg.server;\nvar newMsg={};\nnewMsg.payload=\"{\\\"mqttIp\\\":\\\"\"+server+\"\\\"}\";\nnewMsg.ip=msg.payload;\nnewMsg.port=4210;\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 921,
        "y": 458,
        "wires": [
            [
                "b90e0350a6cd0744"
            ]
        ]
    },
    {
        "id": "b90e0350a6cd0744",
        "type": "change",
        "z": "2f0dfbbf2f2b14e4",
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
        "x": 1075,
        "y": 457,
        "wires": [
            [
                "28c2cec2a7283571"
            ]
        ]
    },
    {
        "id": "5f7e70b2d1a7de60",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "var ipList=[];\nvar server=global.get(\"serverIp\");\nvar a=server.split('.');\nvar ip=a[0]+'.'+a[1]+'.'+a[2]+'.';\n\nmsg.server=server;\nfor(var i=2;i<255;i++)\n    ipList.push(ip+i);\nmsg.payload=ipList;\nreturn msg;\n",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 580,
        "y": 460,
        "wires": [
            [
                "6a0e8b52acb5efda"
            ]
        ]
    },
    {
        "id": "71f2381e0a3e5844",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 580,
        "y": 540,
        "wires": [
            [
                "c5459dbf22046474"
            ]
        ]
    },
    {
        "id": "c5459dbf22046474",
        "type": "mongodb2 in",
        "z": "2f0dfbbf2f2b14e4",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 720,
        "y": 540,
        "wires": [
            [
                "3908bb69e6e6f96c"
            ]
        ]
    },
    {
        "id": "3908bb69e6e6f96c",
        "type": "switch",
        "z": "2f0dfbbf2f2b14e4",
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
        "x": 850,
        "y": 540,
        "wires": [
            [
                "90e881efaf32aed2"
            ],
            [
                "8b8bbf59608a0919"
            ]
        ]
    },
    {
        "id": "90e881efaf32aed2",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "insert one",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nnewMsg.payload    = {'type':msg.payload.type,'mac':msg.payload.mac,'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 992,
        "y": 527,
        "wires": [
            [
                "213648bbce156d00"
            ]
        ]
    },
    {
        "id": "213648bbce156d00",
        "type": "mongodb2 in",
        "z": "2f0dfbbf2f2b14e4",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1170,
        "y": 528,
        "wires": [
            []
        ]
    },
    {
        "id": "8b8bbf59608a0919",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "findOneUpdate",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\nelse if(msg.payload.type==10)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'sound':msg.payload.sound\n    , 'time':time }} ];\n\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1000,
        "y": 580,
        "wires": [
            [
                "213648bbce156d00"
            ]
        ]
    },
    {
        "id": "3a4c7c36854f8009",
        "type": "json",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 437,
        "y": 540,
        "wires": [
            [
                "71f2381e0a3e5844"
            ]
        ]
    },
    {
        "id": "3e995c559f505943",
        "type": "http in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 120,
        "y": 60,
        "wires": [
            [
                "df68795e495d97e4"
            ]
        ]
    },
    {
        "id": "a8dc7e002cc96f6f",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "style",
        "field": "payload.style",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "body {\n    background: #eab0dc;\n    font-family: Arial, Helvetica, sans-serif;\n}\ntable, th, td {\n    padding: 2px;\n    border-collapse: collapse;\n    border: 1px solid #dddddd;\n}\n\n\n/* Full-width input fields */\ninput[type=text], input[type=password] {\n  width: 150px;\n  padding: 5px 10px;\n  margin: 8px 0;\n  display: inline-block;\n  border: 1px solid #ccc;\n  box-sizing: border-box;\n}\n\n/* Set a style for all buttons */\nbutton {\n  background-color: #4CAF50; /*Green*/\n  color: white;\n  padding: 14px 20px;\n  margin: 8px 0;\n  border: none;\n  cursor: pointer;\n}\n\n.buttonMenu {\n          padding: 5px 24px;\n          margin-left:20%;\n          background-color:black;\n          border: none;\n          border-color:black;\n          color:white;\n          text-align: left;\n          text-decoration: none;\n          display: inline-block;\n          font-size: 16px;\n          margin: 4px 2px;\n          cursor: pointer;\n        }\n        .sidenav {\n          height: 100%;\n          width: 0;\n          position: fixed;\n          z-index: 1;\n          top: 0;\n          left: 0;\n          background-color: #111;\n          overflow-x: hidden;\n          transition: 0.5s;\n          padding-top: 60px;\n        }\n        .sidenav a {\n          padding: 8px 8px 8px 32px;\n          text-decoration: none;\n          font-size: 18px;\n          color: #818181;\n          display: block;\n                transition: 0.3s;\n        }\n        .sidenav a:hover {\n          color: #f1f1f1;\n        }\n        .sidenav .closebtn {\n          position: absolute;\n          top: 0;\n          right: 25px;\n          font-size: 36px;\n          margin-left: 50px;\n        }\n\n.buttonM {background-color: #ff66cc;color:white; width:100px; height:20px; padding: 0px 0px; font-size: 16px} /* 기기선택 */  \n.buttonL {background-color: #ff66cc;color:white; width:100px; height:25px; padding: 0px 0px; font-size: 16px} /* 선택 */  \n.buttonMenu {background-color: #000000;} \n.button2 {background-color: #008CBA;} /* Blue */\n.button3 {background-color: #f44336;} /* Red */ \n.button4 {background-color: #e7e7e7; color: black;} /* Gray */ \n.button5 {background-color: #555555;} /* Black */\n.button20 {width: 20%;} \n.button-on {background-color:Lime; color:blue; width:20%;} \n.button-off {background-color:LightGrey; color:white; width:20%;} \n.button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}\n.button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}\n\nbutton:hover {\n  opacity: 0.8;\n}\n\n/* Extra styles for the cancel button */\n.cancelbtn {\n  width: auto;\n  padding: 10px 18px;\n  background-color: #f44336;\n}\n\n/* Center the image and position the close button */\n.imgcontainer {\n  text-align: center;\n  margin: 24px 0 12px 0;\n  position: relative;\n}\n\nimg.avatar {\n  width: 40%;\n  border-radius: 50%;\n}\n\n.container {\n  padding: 16px;\n}\n\nspan.psw {\n  float: right;\n  padding-top: 16px;\n}\n\n/* The Modal (background) */\n.modal {\n  display: none; /* Hidden by default */\n  position: fixed; /* Stay in place */\n  z-index: 1; /* Sit on top */\n  left: 0;\n  top: 0;\n  width: 100%; /* Full width */\n  height: 100%; /* Full height */\n  overflow: auto; /* Enable scroll if needed */\n  background-color: rgb(0,0,0); /* Fallback color */\n  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */\n  padding-top: 60px;\n}\n\n/* Modal Content/Box */\n.modal-content {\n  background-color: #fefefe;\n  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */\n  border: 1px solid #888;\n  width: 80%; /* Could be more or less, depending on screen size */\n}\n\n/* The Close Button (x) */\n.close {\n  position: absolute;\n  right: 25px;\n  top: 0;\n  color: #000;\n  font-size: 35px;\n  font-weight: bold;\n}\n\n.close:hover,\n.close:focus {\n  color: red;\n  cursor: pointer;\n}\n\n/* Add Zoom Animation */\n.animate {\n  -webkit-animation: animatezoom 0.6s;\n  animation: animatezoom 0.6s\n}\n\n@-webkit-keyframes animatezoom {\n  from {-webkit-transform: scale(0)} \n  to {-webkit-transform: scale(1)}\n}\n  \n@keyframes animatezoom {\n  from {transform: scale(0)} \n  to {transform: scale(1)}\n}\n\n/* Change styles for span and cancel button on extra small screens */\n@media screen and (max-width: 300px) {\n  span.psw {\n     display: block;\n     float: none;\n  }\n  .cancelbtn {\n     width: 100%;\n  }\n}",
        "x": 690,
        "y": 60,
        "wires": [
            [
                "e7e8aa92cf74657f"
            ]
        ]
    },
    {
        "id": "1ebf665ca8c17bf4",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "\n        \n// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}",
        "output": "str",
        "x": 565,
        "y": 60,
        "wires": [
            [
                "a8dc7e002cc96f6f"
            ]
        ]
    },
    {
        "id": "eb7c030d0891b519",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\n/*\nmsg.collection = 'localRecord';\nmsg.operation  = 'find.toArray';\nmsg.payload    = {  };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\n*/\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 961,
        "y": 60,
        "wires": [
            [
                "8775dd92dbdf3daa"
            ]
        ]
    },
    {
        "id": "e7e8aa92cf74657f",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='/display?act=2'>환경설정</a>\n  <a href='/display?act=4'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 810,
        "y": 60,
        "wires": [
            [
                "eb7c030d0891b519"
            ]
        ]
    },
    {
        "id": "a9fa68d9f742ddb2",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "find.toArray",
        "func": "global.set(\"msg_main\",msg);\nmsg.collection = 'localRecord';\nmsg.operation  =  'find.toArray';\nmsg.payload    = { };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 220,
        "wires": [
            [
                "741938e9548e84d1"
            ]
        ]
    },
    {
        "id": "741938e9548e84d1",
        "type": "mongodb2 in",
        "z": "2f0dfbbf2f2b14e4",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 910,
        "y": 220,
        "wires": [
            [
                "fa7be61a8903ee2e"
            ]
        ]
    },
    {
        "id": "fe485c93f10fbdbe",
        "type": "http response",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1530,
        "y": 220,
        "wires": []
    },
    {
        "id": "d30a5549ea7a0e07",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "0,1,2,3 List",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    if(msg.payload[i].type==2) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>PE-300</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>키우미</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>KSM-8900</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n        s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>simpleTH</button></a>\";\n        s+=\"</form>\";\n        s+=\"</th>\";\n       s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    else if(msg.payload[i].type==6) {\n        s+=\"<tr><th>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>XEX-DR14E</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>\"\n            for(var j=0;j<8;j++) {\n                if(msg.payload[i].in[j]==1) \n                    s+=\"<button class='button button-ledon'>\"+j+\"</button>&nbsp;\";\n                else\n                    s+=\"<button class='button button-ledoff'>\"+j+\"</button>&nbsp;\";\n            }\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>DHT11</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>RY-WS301</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==9) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>MP-508E</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==10) {\n        s+=\"<tr><td>\";\n        s+=\"<form action='/display'>\";\n            s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n            s+=\"<input type='hidden' name='act' value='1'>\";\n            s+=\"<button type='submit' name='value' value='0' class='button buttonM'>사운드</button></a>\";\n        s+=\"</form>\";\n        s+=\"</td>\";\n        s+=\"<td>사운드:\"+msg.payload[i].sound+\"</td> \"\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    \n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1210,
        "y": 220,
        "wires": [
            [
                "d6e996804d718bef"
            ]
        ]
    },
    {
        "id": "d6e996804d718bef",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<meta http-equiv='refresh' content='10'/>\n<style> {{{payload.style}}} </style>\n<script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>",
        "output": "str",
        "x": 1380,
        "y": 220,
        "wires": [
            [
                "fe485c93f10fbdbe"
            ]
        ]
    },
    {
        "id": "fa7be61a8903ee2e",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_main\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1040,
        "y": 220,
        "wires": [
            [
                "d30a5549ea7a0e07"
            ]
        ]
    },
    {
        "id": "10f783d19c47491f",
        "type": "http in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 140,
        "wires": [
            [
                "f55f13b02f4620ea"
            ]
        ]
    },
    {
        "id": "b0a94a6f9da4af81",
        "type": "comment",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 114,
        "y": 140,
        "wires": []
    },
    {
        "id": "b50fc3e657c69d6c",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "get global",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nmsg.payload.act=global.get(\"act\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1220,
        "y": 120,
        "wires": [
            [
                "cf431ef1e6a38deb"
            ]
        ]
    },
    {
        "id": "cf431ef1e6a38deb",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "html 기기 name",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n   <br> &emsp;\n   <table>\n         <th>mac</th> <th>{{{payload.mac}}}</th>\n      </tr>\n      <tr>\n        <th>접속IP</th> <th>{{{payload.ip}}}</th>\n      </tr>\n      <tr>\n        <th>이름</th> \n            <form action=\"/save\" method=\"post\">\n                <input type='hidden' name='mac' value='{{{payload.mac}}}'>\n                <input type='hidden' name='act' value='1'>\n                <th><input type=\"text\" id=\"name\" name=\"name\" value=\"{{{payload.name}}}\"></th>\n                <th><button class='button buttonL' type=\"submit\" style=\"width:50px;\" >저장</button></th>\n            </form> \n      </tr>\n    </table>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 1380,
        "y": 120,
        "wires": [
            [
                "9a8f0ee438a42c1e"
            ]
        ]
    },
    {
        "id": "9b5dc14dc113e4c4",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 120,
        "wires": [
            [
                "21beaaca40da7389"
            ]
        ]
    },
    {
        "id": "21beaaca40da7389",
        "type": "mongodb2 in",
        "z": "2f0dfbbf2f2b14e4",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 930,
        "y": 120,
        "wires": [
            [
                "857a1b696fe6b17a"
            ]
        ]
    },
    {
        "id": "9a8f0ee438a42c1e",
        "type": "http response",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1550,
        "y": 120,
        "wires": []
    },
    {
        "id": "f55f13b02f4620ea",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "global.set(\"msg_display\",msg);\nglobal.set(\"act\",msg.payload.act);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 460,
        "y": 140,
        "wires": [
            [
                "5afb51cb6f97fd5b"
            ]
        ]
    },
    {
        "id": "857a1b696fe6b17a",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_display\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1080,
        "y": 120,
        "wires": [
            [
                "b50fc3e657c69d6c"
            ]
        ]
    },
    {
        "id": "5afb51cb6f97fd5b",
        "type": "switch",
        "z": "2f0dfbbf2f2b14e4",
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
        "x": 610,
        "y": 140,
        "wires": [
            [
                "9b5dc14dc113e4c4"
            ],
            [
                "a6b214e7337338f6"
            ],
            [
                "a9fa68d9f742ddb2"
            ],
            [
                "8bbc018cf3ba7040"
            ]
        ]
    },
    {
        "id": "d9f80716be95a17d",
        "type": "http in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 340,
        "wires": [
            [
                "a816dfbd545a03d1"
            ]
        ]
    },
    {
        "id": "4143e8b3fdcf5e4e",
        "type": "comment",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 114,
        "y": 340,
        "wires": []
    },
    {
        "id": "c38e7ea7ee4204dd",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 600,
        "y": 340,
        "wires": [
            [
                "a2d1ba017cc3dcc6"
            ]
        ]
    },
    {
        "id": "a2d1ba017cc3dcc6",
        "type": "mongodb2 in",
        "z": "2f0dfbbf2f2b14e4",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 770,
        "y": 340,
        "wires": [
            [
                "47b03ce1279d50b1"
            ]
        ]
    },
    {
        "id": "b61bf7f3b33ef7af",
        "type": "link out",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "links": [
            "4f239390b4bf4e73",
            "2044220.61e99de"
        ],
        "x": 1015,
        "y": 340,
        "wires": []
    },
    {
        "id": "47b03ce1279d50b1",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 920,
        "y": 340,
        "wires": [
            [
                "b61bf7f3b33ef7af"
            ]
        ]
    },
    {
        "id": "a816dfbd545a03d1",
        "type": "switch",
        "z": "2f0dfbbf2f2b14e4",
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
        "x": 430,
        "y": 340,
        "wires": [
            [
                "c38e7ea7ee4204dd"
            ],
            [],
            []
        ]
    },
    {
        "id": "ce0231bdf0cf708b",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.internalIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1490,
        "y": 60,
        "wires": [
            [
                "36d45ebfe058cf6d"
            ]
        ]
    },
    {
        "id": "36d45ebfe058cf6d",
        "type": "http response",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1642,
        "y": 60,
        "wires": []
    },
    {
        "id": "4f239390b4bf4e73",
        "type": "link in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "8775dd92dbdf3daa",
            "b61bf7f3b33ef7af"
        ],
        "x": 1259,
        "y": 60,
        "wires": [
            [
                "61398c7445fb12ee"
            ]
        ]
    },
    {
        "id": "8775dd92dbdf3daa",
        "type": "link out",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "links": [
            "4f239390b4bf4e73"
        ],
        "x": 1075,
        "y": 60,
        "wires": []
    },
    {
        "id": "cd7a7498d3d0df73",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "html 환경설정",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    환경설정 <br>\n    센서와 기기를 서버에 연결하려면 아래 버튼을 누르세요\n    <form action=\"/act\" method=\"post\">\n        <input type='hidden' name='act' value='1'>\n        <div class=\"container\">\n            <br> &emsp;\n            <button class='button buttonL' type=\"submit\" style=\"width:auto;\">- mqtt 통신연결 -</button>\n        </div>\n    </form>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 960,
        "y": 180,
        "wires": [
            [
                "97624ff259b497ba"
            ]
        ]
    },
    {
        "id": "a6b214e7337338f6",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 180,
        "wires": [
            [
                "cd7a7498d3d0df73"
            ]
        ]
    },
    {
        "id": "97624ff259b497ba",
        "type": "http response",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1127,
        "y": 180,
        "wires": []
    },
    {
        "id": "d05f57363043fce9",
        "type": "template",
        "z": "2f0dfbbf2f2b14e4",
        "name": "html 메뉴얼",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    메뉴얼\n\n</body>\n</html>\n",
        "output": "str",
        "x": 910,
        "y": 280,
        "wires": [
            [
                "4f8ea51dd9a631c1"
            ]
        ]
    },
    {
        "id": "8bbc018cf3ba7040",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 750,
        "y": 280,
        "wires": [
            [
                "d05f57363043fce9"
            ]
        ]
    },
    {
        "id": "4f8ea51dd9a631c1",
        "type": "http response",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1087,
        "y": 280,
        "wires": []
    },
    {
        "id": "1410dd80595f093f",
        "type": "comment",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Act",
        "info": "1: \n",
        "x": 110,
        "y": 460,
        "wires": []
    },
    {
        "id": "4705f8c27f3a5b5f",
        "type": "http in",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 280,
        "y": 460,
        "wires": [
            [
                "8759205b94790eec"
            ]
        ]
    },
    {
        "id": "8759205b94790eec",
        "type": "switch",
        "z": "2f0dfbbf2f2b14e4",
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
        "x": 430,
        "y": 460,
        "wires": [
            [
                "5f7e70b2d1a7de60"
            ],
            [],
            []
        ]
    },
    {
        "id": "ba05f4e47f2ac728",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "var server=msg.payload[1].address;\nglobal.set(\"serverIp\",server);\nmsg.server=server;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 420,
        "y": 60,
        "wires": [
            [
                "1ebf665ca8c17bf4"
            ]
        ]
    },
    {
        "id": "61398c7445fb12ee",
        "type": "function",
        "z": "2f0dfbbf2f2b14e4",
        "name": "",
        "func": "var serverIp=global.get(\"serverIp\");\nmsg.payload.internalIPv4=serverIp;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1360,
        "y": 60,
        "wires": [
            [
                "ce0231bdf0cf708b"
            ]
        ]
    },
    {
        "id": "df68795e495d97e4",
        "type": "hostip",
        "z": "2f0dfbbf2f2b14e4",
        "name": "Host IP",
        "x": 260,
        "y": 60,
        "wires": [
            [
                "ba05f4e47f2ac728"
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
