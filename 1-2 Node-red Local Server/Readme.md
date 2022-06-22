로컬서버 프로그램 웹소켙으로 구성 (2022년 3월24일 버젼)
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
        "id": "5a4ce9cc8e6f352a",
        "type": "tab",
        "label": "Node-red Local Server",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "5093075ced600785",
        "type": "mqtt in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 290,
        "y": 560,
        "wires": [
            [
                "48ac2ec515f28973",
                "3f532047eec5125b"
            ]
        ]
    },
    {
        "id": "0aeb08b4eca5e85c",
        "type": "subflow:10a86fdc.efc16",
        "z": "5a4ce9cc8e6f352a",
        "name": "Iterate",
        "env": [],
        "x": 750,
        "y": 440,
        "wires": [
            [
                "14ed300bd199a92f",
                "3c1b4e82a85c352f"
            ],
            []
        ]
    },
    {
        "id": "3c1b4e82a85c352f",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 748,
        "y": 380,
        "wires": [
            [
                "0aeb08b4eca5e85c"
            ]
        ]
    },
    {
        "id": "222e3ea95b1d6a10",
        "type": "udp out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "addr": "",
        "iface": "",
        "port": "",
        "ipv": "udp4",
        "outport": "",
        "base64": false,
        "multicast": "false",
        "x": 1214,
        "y": 436,
        "wires": []
    },
    {
        "id": "14ed300bd199a92f",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "var server=msg.server;\nvar newMsg={};\nnewMsg.payload=\"{\\\"mqttIp\\\":\\\"\"+server+\"\\\"}\";\nnewMsg.ip=msg.payload;\nnewMsg.port=4210;\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 921,
        "y": 438,
        "wires": [
            [
                "1e85548d2e72799e"
            ]
        ]
    },
    {
        "id": "1e85548d2e72799e",
        "type": "change",
        "z": "5a4ce9cc8e6f352a",
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
        "y": 437,
        "wires": [
            [
                "222e3ea95b1d6a10"
            ]
        ]
    },
    {
        "id": "b57047248c21731b",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "var ipList=[];\nvar server=global.get(\"serverIp\");\nvar a=server.split('.');\nvar ip=a[0]+'.'+a[1]+'.'+a[2]+'.';\n\nmsg.server=server;\nfor(var i=2;i<255;i++)\n    ipList.push(ip+i);\nmsg.payload=ipList;\nreturn msg;\n",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 580,
        "y": 440,
        "wires": [
            [
                "0aeb08b4eca5e85c"
            ]
        ]
    },
    {
        "id": "dd4ba67131b1c96d",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 580,
        "y": 560,
        "wires": [
            [
                "4579ddc2bfa28109"
            ]
        ]
    },
    {
        "id": "4579ddc2bfa28109",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "5ffbb241da37a3ee",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 720,
        "y": 560,
        "wires": [
            [
                "fc527773cbccc7cc"
            ]
        ]
    },
    {
        "id": "fc527773cbccc7cc",
        "type": "switch",
        "z": "5a4ce9cc8e6f352a",
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
        "y": 560,
        "wires": [
            [
                "b29f5da82751b74c"
            ],
            [
                "843960b9ee8dbf4c"
            ]
        ]
    },
    {
        "id": "b8f617b26029882c",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "5ffbb241da37a3ee",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1350,
        "y": 620,
        "wires": [
            [
                "2328befc4ea29b4d"
            ]
        ]
    },
    {
        "id": "48ac2ec515f28973",
        "type": "json",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 437,
        "y": 560,
        "wires": [
            [
                "dd4ba67131b1c96d"
            ]
        ]
    },
    {
        "id": "cf5b2f60331433bc",
        "type": "http in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 100,
        "y": 70,
        "wires": [
            [
                "f464dd0bfdc334ea"
            ]
        ]
    },
    {
        "id": "e80ddbdc0c436b07",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "style",
        "field": "payload.style",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "body {\n    background: #eab0dc;\n    font-family: Arial, Helvetica, sans-serif;\n}\ntable, th, td {\n    padding: 2px;\n    border-collapse: collapse;\n    border: 1px solid #dddddd;\n    text-align: center;\n    vertical-align: middle;\n}\n\n\n/* Full-width input fields */\ninput[type=text], input[type=password] {\n  width: 150px;\n  padding: 5px 10px;\n  margin: 8px 0;\n  display: inline-block;\n  border: 1px solid #ccc;\n  box-sizing: border-box;\n}\n\n/* Set a style for all buttons */\nbutton {\n  background-color: #4CAF50; /*Green*/\n  color: white;\n  padding: 14px 20px;\n  margin: 8px 0;\n  border: none;\n  cursor: pointer;\n}\n\n.buttonMenu {\n          padding: 5px 24px;\n          margin-left:20%;\n          background-color:black;\n          border: none;\n          border-color:black;\n          color:white;\n          text-align: left;\n          text-decoration: none;\n          display: inline-block;\n          font-size: 16px;\n          margin: 4px 2px;\n          cursor: pointer;\n        }\n        .sidenav {\n          height: 100%;\n          width: 0;\n          position: fixed;\n          z-index: 1;\n          top: 0;\n          left: 0;\n          background-color: #111;\n          overflow-x: hidden;\n          transition: 0.5s;\n          padding-top: 60px;\n        }\n        .sidenav a {\n          padding: 8px 8px 8px 32px;\n          text-decoration: none;\n          font-size: 18px;\n          color: #818181;\n          display: block;\n                transition: 0.3s;\n        }\n        .sidenav a:hover {\n          color: #f1f1f1;\n        }\n        .sidenav .closebtn {\n          position: absolute;\n          top: 0;\n          right: 25px;\n          font-size: 36px;\n          margin-left: 50px;\n        }\n\n.buttonM {background-color: #ff66cc;color:white; width:100px; height:20px; padding: 0px 0px; font-size: 16px} /* 기기선택 */  \n.buttonL {background-color: #ff66cc;color:white; width:100px; height:25px; padding: 0px 0px; font-size: 16px} /* 선택 */  \n.buttonMenu {background-color: #000000;} \n.button2 {background-color: #008CBA;} /* Blue */\n.button3 {background-color: #f44336;} /* Red */ \n.button4 {background-color: #e7e7e7; color: black;} /* Gray */ \n.button5 {background-color: #555555;} /* Black */\n.button20 {width: 20%;} \n.button-on {border-radius: 100%; padding: 20px; font-size: 18px; margin: 0px 0px; background-color: #4CAF50;}\n.button-off {border-radius: 100%; padding: 20px; font-size: 18px; background-color: #707070;}\n.button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}\n.button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}\n\nbutton:hover {\n  opacity: 0.8;\n}\n\n/* Extra styles for the cancel button */\n.cancelbtn {\n  width: auto;\n  padding: 10px 18px;\n  background-color: #f44336;\n}\n\n/* Center the image and position the close button */\n.imgcontainer {\n  text-align: center;\n  margin: 24px 0 12px 0;\n  position: relative;\n}\n\nimg.avatar {\n  width: 40%;\n  border-radius: 50%;\n}\n\n.container {\n  padding: 16px;\n}\n\nspan.psw {\n  float: right;\n  padding-top: 16px;\n}\n\n/* The Modal (background) */\n.modal {\n  display: none; /* Hidden by default */\n  position: fixed; /* Stay in place */\n  z-index: 1; /* Sit on top */\n  left: 0;\n  top: 0;\n  width: 100%; /* Full width */\n  height: 100%; /* Full height */\n  overflow: auto; /* Enable scroll if needed */\n  background-color: rgb(0,0,0); /* Fallback color */\n  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */\n  padding-top: 60px;\n}\n\n/* Modal Content/Box */\n.modal-content {\n  background-color: #fefefe;\n  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */\n  border: 1px solid #888;\n  width: 80%; /* Could be more or less, depending on screen size */\n}\n\n/* The Close Button (x) */\n.close {\n  position: absolute;\n  right: 25px;\n  top: 0;\n  color: #000;\n  font-size: 35px;\n  font-weight: bold;\n}\n\n.close:hover,\n.close:focus {\n  color: red;\n  cursor: pointer;\n}\n\n/* Add Zoom Animation */\n.animate {\n  -webkit-animation: animatezoom 0.6s;\n  animation: animatezoom 0.6s\n}\n\n@-webkit-keyframes animatezoom {\n  from {-webkit-transform: scale(0)} \n  to {-webkit-transform: scale(1)}\n}\n  \n@keyframes animatezoom {\n  from {transform: scale(0)} \n  to {transform: scale(1)}\n}\n\n/* Change styles for span and cancel button on extra small screens */\n@media screen and (max-width: 300px) {\n  span.psw {\n     display: block;\n     float: none;\n  }\n  .cancelbtn {\n     width: 100%;\n  }\n}",
        "x": 690,
        "y": 70,
        "wires": [
            [
                "1171bc8683b0da0b"
            ]
        ]
    },
    {
        "id": "f4ceeb92978ff15d",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}\n",
        "output": "str",
        "x": 570,
        "y": 80,
        "wires": [
            [
                "e80ddbdc0c436b07"
            ]
        ]
    },
    {
        "id": "398fd8056b60d14c",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\n/*\nmsg.collection = 'localRecord';\nmsg.operation  = 'find.toArray';\nmsg.payload    = {  };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\n*/\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 950,
        "y": 40,
        "wires": [
            [
                "05088901bdf6ef13"
            ]
        ]
    },
    {
        "id": "1171bc8683b0da0b",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='/display?act=2'>환경설정</a>\n  <a href='/display?act=4'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 810,
        "y": 70,
        "wires": [
            [
                "398fd8056b60d14c"
            ]
        ]
    },
    {
        "id": "e7318f71d3cce548",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
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
                "1db35913dad6660a"
            ]
        ]
    },
    {
        "id": "1db35913dad6660a",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 910,
        "y": 220,
        "wires": [
            [
                "bdc1888a3d2f9be4"
            ]
        ]
    },
    {
        "id": "1c5545d57bb93855",
        "type": "http response",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1830,
        "y": 220,
        "wires": []
    },
    {
        "id": "bdc1888a3d2f9be4",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
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
                "648dbb8cc1a02c80"
            ]
        ]
    },
    {
        "id": "a997c7eb8b92af9e",
        "type": "http in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 150,
        "wires": [
            [
                "33f1030279b4aedb"
            ]
        ]
    },
    {
        "id": "b02734afa119a826",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 114,
        "y": 150,
        "wires": []
    },
    {
        "id": "9b5b38a7d3a31bac",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "get global",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nmsg.payload.act=global.get(\"act\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1192,
        "y": 130,
        "wires": [
            [
                "d27956fe7cde2bab"
            ]
        ]
    },
    {
        "id": "d27956fe7cde2bab",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "html 기기 name",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n   <br> &emsp;\n   <table>\n         <th>mac</th> <th>{{{payload.mac}}}</th>\n      </tr>\n      <tr>\n        <th>접속IP</th> <th>{{{payload.ip}}}</th>\n      </tr>\n      <tr>\n        <th>이름</th> \n            <form action=\"/save\" method=\"post\">\n                <input type='hidden' name='mac' value='{{{payload.mac}}}'>\n                <input type='hidden' name='act' value='1'>\n                <th><input type=\"text\" id=\"name\" name=\"name\" value=\"{{{payload.name}}}\"></th>\n                <th><button class='button buttonL' type=\"submit\" style=\"width:50px;\" >저장</button></th>\n            </form> \n      </tr>\n    </table>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 1352,
        "y": 130,
        "wires": [
            [
                "c9fb9df343ae95ac"
            ]
        ]
    },
    {
        "id": "33b9466c1096d5cf",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 762,
        "y": 130,
        "wires": [
            [
                "04250f758a2c1b32"
            ]
        ]
    },
    {
        "id": "04250f758a2c1b32",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 902,
        "y": 130,
        "wires": [
            [
                "b5c6328b5c28c643"
            ]
        ]
    },
    {
        "id": "c9fb9df343ae95ac",
        "type": "http response",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1522,
        "y": 130,
        "wires": []
    },
    {
        "id": "33f1030279b4aedb",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "global.set(\"msg_display\",msg);\nglobal.set(\"act\",msg.payload.act);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 460,
        "y": 150,
        "wires": [
            [
                "d4958413be79c2b0"
            ]
        ]
    },
    {
        "id": "b5c6328b5c28c643",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_display\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1052,
        "y": 130,
        "wires": [
            [
                "9b5b38a7d3a31bac"
            ]
        ]
    },
    {
        "id": "d4958413be79c2b0",
        "type": "switch",
        "z": "5a4ce9cc8e6f352a",
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
        "y": 150,
        "wires": [
            [
                "33b9466c1096d5cf"
            ],
            [
                "089bbd24598e654d"
            ],
            [
                "e7318f71d3cce548"
            ],
            [
                "bba13e1057eb8567"
            ]
        ]
    },
    {
        "id": "9a8628f1ac94473e",
        "type": "http in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 320,
        "wires": [
            [
                "d2dbd14e76e254e4"
            ]
        ]
    },
    {
        "id": "3f005e85866b5e5a",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 114,
        "y": 320,
        "wires": []
    },
    {
        "id": "0f8e9825fa1bbfca",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 600,
        "y": 320,
        "wires": [
            [
                "f29103fbaa921316"
            ]
        ]
    },
    {
        "id": "f29103fbaa921316",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 770,
        "y": 320,
        "wires": [
            [
                "8d061e26dd2460b7"
            ]
        ]
    },
    {
        "id": "d25c1fe5f8816f18",
        "type": "link out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "b664005510d9718d",
            "2044220.61e99de"
        ],
        "x": 1015,
        "y": 320,
        "wires": []
    },
    {
        "id": "8d061e26dd2460b7",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 920,
        "y": 320,
        "wires": [
            [
                "d25c1fe5f8816f18"
            ]
        ]
    },
    {
        "id": "d2dbd14e76e254e4",
        "type": "switch",
        "z": "5a4ce9cc8e6f352a",
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
        "y": 320,
        "wires": [
            [
                "0f8e9825fa1bbfca"
            ],
            [],
            []
        ]
    },
    {
        "id": "289e22e4b22e195e",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.internalIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1490,
        "y": 40,
        "wires": [
            [
                "c5ee27b2668f7349"
            ]
        ]
    },
    {
        "id": "c5ee27b2668f7349",
        "type": "http response",
        "z": "5a4ce9cc8e6f352a",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1660,
        "y": 60,
        "wires": []
    },
    {
        "id": "b664005510d9718d",
        "type": "link in",
        "z": "5a4ce9cc8e6f352a",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "05088901bdf6ef13",
            "d25c1fe5f8816f18"
        ],
        "x": 1259,
        "y": 70,
        "wires": [
            [
                "87d6ebcf286cd3b9"
            ]
        ]
    },
    {
        "id": "05088901bdf6ef13",
        "type": "link out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "b664005510d9718d"
        ],
        "x": 1075,
        "y": 70,
        "wires": []
    },
    {
        "id": "a01e40f283179c13",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "html 환경설정",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    환경설정 <br>\n    센서와 기기를 서버에 연결하려면 아래 버튼을 누르세요\n    <form action=\"/act\" method=\"post\">\n        <input type='hidden' name='act' value='1'>\n        <div class=\"container\">\n            <br> &emsp;\n            <button class='button buttonL' type=\"submit\" style=\"width:auto;\">- mqtt 통신연결 -</button>\n        </div>\n    </form>\n\n</body>\n</html>\n",
        "output": "str",
        "x": 920,
        "y": 176,
        "wires": [
            [
                "b0bbe832b7a4a1fe"
            ]
        ]
    },
    {
        "id": "089bbd24598e654d",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 750,
        "y": 176,
        "wires": [
            [
                "a01e40f283179c13"
            ]
        ]
    },
    {
        "id": "b0bbe832b7a4a1fe",
        "type": "http response",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1087,
        "y": 176,
        "wires": []
    },
    {
        "id": "94a5a5076587968b",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "html 메뉴얼",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <style> {{{payload.style}}} </style>\n        <script> {{{payload.script}}} </script>\n</head>\n<body>\n   {{{payload.menu}}} \n    메뉴얼\n\n</body>\n</html>\n",
        "output": "str",
        "x": 910,
        "y": 266,
        "wires": [
            [
                "6fa4d55a4ca17fb7"
            ]
        ]
    },
    {
        "id": "bba13e1057eb8567",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "Config",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 750,
        "y": 266,
        "wires": [
            [
                "94a5a5076587968b"
            ]
        ]
    },
    {
        "id": "6fa4d55a4ca17fb7",
        "type": "http response",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1087,
        "y": 266,
        "wires": []
    },
    {
        "id": "42669fdf42b5bd8f",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "Act",
        "info": "1: \n",
        "x": 110,
        "y": 440,
        "wires": []
    },
    {
        "id": "4ab6b3ea4a4c2945",
        "type": "http in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 280,
        "y": 440,
        "wires": [
            [
                "e3e50822ed664769"
            ]
        ]
    },
    {
        "id": "e3e50822ed664769",
        "type": "switch",
        "z": "5a4ce9cc8e6f352a",
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
        "y": 440,
        "wires": [
            [
                "b57047248c21731b"
            ],
            [],
            []
        ]
    },
    {
        "id": "ecc713a630c665b1",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "var server=msg.payload[1].address;\nglobal.set(\"serverIp\",server);\nmsg.server=server;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 420,
        "y": 70,
        "wires": [
            [
                "f4ceeb92978ff15d"
            ]
        ]
    },
    {
        "id": "87d6ebcf286cd3b9",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "var serverIp=global.get(\"serverIp\");\nmsg.payload.internalIPv4=serverIp;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1360,
        "y": 70,
        "wires": [
            [
                "289e22e4b22e195e"
            ]
        ]
    },
    {
        "id": "8ec9554f36e58204",
        "type": "websocket out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 1840,
        "y": 620,
        "wires": []
    },
    {
        "id": "ec0427a50fc7384a",
        "type": "websocket in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "server": "bf4882ef33e69fa3",
        "client": "",
        "x": 300,
        "y": 660,
        "wires": [
            [
                "0790832deeed59bd"
            ]
        ]
    },
    {
        "id": "1553a31007928d62",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "출력버튼 웹소켙",
        "info": "",
        "x": 300,
        "y": 620,
        "wires": []
    },
    {
        "id": "02109c10105b4145",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "<!DOCTYPE HTML>\n<html>\n<head>\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n    <style> {{{payload.style}}} </style>\n    <script> {{{payload.script}}} </script>\n    <script type=\"text/javascript\"> {{{payload.scriptSocket}}} </script>\n</head>\n<body onload=\"wsConnect();\" onunload=\"ws.disconnect();\">\n    {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>\n\n",
        "x": 1700,
        "y": 220,
        "wires": [
            [
                "1c5545d57bb93855"
            ]
        ]
    },
    {
        "id": "0790832deeed59bd",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "var msg = {payload:msg.payload.replace(/['^']/g, \"\\\"\")};\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 480,
        "y": 660,
        "wires": [
            [
                "5325ef62c77c6bae",
                "1b2d6fde3930ad8c"
            ]
        ]
    },
    {
        "id": "5325ef62c77c6bae",
        "type": "json",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 630,
        "y": 660,
        "wires": [
            [
                "8dd3979983a233e6"
            ]
        ]
    },
    {
        "id": "15a6eff95cb20d28",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "http://localhost:1880/login",
        "info": "",
        "x": 330,
        "y": 740,
        "wires": []
    },
    {
        "id": "833781c5d6ebca95",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "입력버튼 웹소켙",
        "info": "",
        "x": 300,
        "y": 520,
        "wires": []
    },
    {
        "id": "1b2d6fde3930ad8c",
        "type": "mqtt out",
        "z": "5a4ce9cc8e6f352a",
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
        "x": 710,
        "y": 720,
        "wires": []
    },
    {
        "id": "ea32a2d1f9cad461",
        "type": "link out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "8378c6aa193758da"
        ],
        "x": 1135,
        "y": 547,
        "wires": []
    },
    {
        "id": "b73b6d8cf43f80ba",
        "type": "link out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "8378c6aa193758da"
        ],
        "x": 1155,
        "y": 600,
        "wires": []
    },
    {
        "id": "29e8bf16fa951973",
        "type": "link out",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "8378c6aa193758da"
        ],
        "x": 1035,
        "y": 660,
        "wires": []
    },
    {
        "id": "8378c6aa193758da",
        "type": "link in",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "links": [
            "ea32a2d1f9cad461",
            "29e8bf16fa951973",
            "b73b6d8cf43f80ba"
        ],
        "x": 1255,
        "y": 620,
        "wires": [
            [
                "b8f617b26029882c"
            ]
        ]
    },
    {
        "id": "2328befc4ea29b4d",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOne",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1500,
        "y": 620,
        "wires": [
            [
                "36b3cabd833e0efd"
            ]
        ]
    },
    {
        "id": "36b3cabd833e0efd",
        "type": "mongodb2 in",
        "z": "5a4ce9cc8e6f352a",
        "service": "_ext_",
        "configNode": "8364b0aa.ebd7e",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 1660,
        "y": 620,
        "wires": [
            [
                "8ec9554f36e58204"
            ]
        ]
    },
    {
        "id": "cdd94d993498a0ba",
        "type": "subflow:10a86fdc.efc16",
        "z": "5a4ce9cc8e6f352a",
        "name": "Iterate",
        "env": [],
        "x": 1590,
        "y": 480,
        "wires": [
            [
                "255a97ee7128798c",
                "251c0bd111e26a41"
            ],
            []
        ]
    },
    {
        "id": "251c0bd111e26a41",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1580,
        "y": 420,
        "wires": [
            [
                "cdd94d993498a0ba"
            ]
        ]
    },
    {
        "id": "255a97ee7128798c",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOne",
        "func": "var newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1760,
        "y": 480,
        "wires": [
            [
                "36b3cabd833e0efd"
            ]
        ]
    },
    {
        "id": "91d52d6a7721c21b",
        "type": "delay",
        "z": "5a4ce9cc8e6f352a",
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
        "x": 1440,
        "y": 480,
        "wires": [
            [
                "cdd94d993498a0ba"
            ]
        ]
    },
    {
        "id": "a47171d9d555f831",
        "type": "comment",
        "z": "5a4ce9cc8e6f352a",
        "name": "웹페이지 초기화",
        "info": "",
        "x": 1400,
        "y": 420,
        "wires": []
    },
    {
        "id": "f464dd0bfdc334ea",
        "type": "hostip",
        "z": "5a4ce9cc8e6f352a",
        "name": "Host IP",
        "x": 280,
        "y": 71,
        "wires": [
            [
                "ecc713a630c665b1"
            ]
        ]
    },
    {
        "id": "648dbb8cc1a02c80",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "0,1,2,3 List (수정)",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i,j;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    s+=\"<tr><th>\";\n    s+=\"<form action='/display'>\";\n        s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n        s+=\"<input type='hidden' name='act' value='1'>\";\n        s+=\"<button type='submit' name='value' value='0' class='button buttonM'>\"+msg.payload[i].model+\"</button></a>\";\n    s+=\"</form>\";\n    s+=\"</th>\";\n    if(msg.payload[i].type==2) {\n        //s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        //s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    else if(msg.payload[i].type==6) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        //s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==9) {\n        //s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==10) {\n        //s+=\"<td>사운드:\"+msg.payload[i].sound+\" \"+msg.payload[i].time+\"</td> \";\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==11) {\n        //s+=\"<td>충격발생:\"+msg.payload[i].time+\"</td> \"\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==12) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==13) {\n        s+=\"<td>\"\n            s+=\"<table align='center' style='margin: 0px auto;'>\";\n            s+=\"<tr>\";\n            s+=\"<td>정지</td>\";\n            s+=\"<td>올림</td>\";\n            s+=\"<td>내림</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==14) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==15) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==16) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==17) {\n        //s+=\"<td>lux:\"+msg.payload[i].lux+\"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==18) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    if(msg.payload[i].type==20) {\n        //s+=\"<td>hum:\"+msg.payload[i].hum+\"   온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==21) {\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==23) {\n        s+=\"<td>\";\n        for(j=0;j<5;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==24) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==25) {\n        s+=\"<td>\";\n        for(j=0;j<1;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }    \n    else if(msg.payload[i].type==26) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-dir'></span>&emsp;<br>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-vel'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n     else if(msg.payload[i].type==27) {\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n     }\n     else if(msg.payload[i].type==28) {\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n     }\n     \n     else if(msg.payload[i].type==29) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<2;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<2;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<2;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1230,
        "y": 220,
        "wires": [
            [
                "1bacae8bdd50274f",
                "91d52d6a7721c21b"
            ]
        ]
    },
    {
        "id": "1bacae8bdd50274f",
        "type": "template",
        "z": "5a4ce9cc8e6f352a",
        "name": "Web Socket scriptSocket (수정)",
        "field": "payload.scriptSocket",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "        var ws;\n        var wsUri = \"ws:\";\n        var loc = window.location;\n        console.log(loc);\n        if (loc.protocol === \"https:\") { wsUri = \"wss:\"; }\n        // This needs to point to the web socket in the Node-RED flow\n        // ... in this case it's ws/simple\n        wsUri += \"//\" + loc.host + loc.pathname.replace(\"display\",\"ws/simple\");\n\n        function wsConnect() {\n            console.log(\"connect\",wsUri);\n            ws = new WebSocket(wsUri);\n            //var line = \"\";    // either uncomment this for a building list of messages\n            ws.onmessage = function(msg) {\n                var line = \"\";  // or uncomment this to overwrite the existing message\n                // parse the incoming message as a JSON object\n                var data = JSON.parse(msg.data);\n                //var data = msg.data;\n                console.log(data);\n                if(data.type==2) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"EC:\"+data.ec;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"PH:\"+data.ph;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                if(data.type==3) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"CO2:\"+data.co2;\n                }\n                if(data.type==4) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.mo+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.tem+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                if(data.type==5) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==6) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:1}\\\");'></button>\";\n                }\n                else if(data.type==7) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                }\n                else if(data.type==8) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"대기압:\"+data.pres;\n                }\n                else if(data.type==9) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                else if(data.type==10) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"사운드:\"+data.sound;\n                }\n                else if(data.type==11) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"충격발생:\"+data.knock;\n                }\n                else if(data.type==12) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:1}\\\");'></button>\";\n                }\n                if(data.type==13) {\n                    if(data.state==0) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==1) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==2) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n\n                 }\n                 else if(data.type==14) {\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:1}\\\");'></button>\";\n                    \n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                }\n                else if(data.type==15) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"연기농도:\"+data.gas;\n                }\n                else if(data.type==16) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"메탄농도:\"+data.gas;\n                }\n                if(data.type==17) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"Lux:\"+data.lux;\n                }\n                else if(data.type==18) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:5,^value^:1}\\\");'></button>\";\n                    if(data.out[6]=='1')\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out6').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:6,^value^:1}\\\");'></button>\";\n                    if(data.out[7]=='1')\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out7').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:18,^outNo^:7,^value^:1}\\\");'></button>\";\n                }\n                 else if(data.type==20) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.hum+\"%\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==21) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==23) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"조도:\"+data.light+\" LUX\";\n                    document.getElementById(data.mac+'-in3').innerHTML = \"비옥도:\"+data.light+\" uS/cm\";\n                    document.getElementById(data.mac+'-in4').innerHTML = \"밭데리:\"+data.batt+\" %\";\n                }\n                else if(data.type==24) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==25) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"Loadcell:\"+data.Loadcell+\"kg\";\n                }\n                else if(data.type==26) {\n                    document.getElementById(data.mac+'-dir').innerHTML = \"<input type='radio' name='inveter' id='\"+data.mac+\"' value='0' onclick='handleRadio(event)'/>정지<input type='radio' name='inveter' id='\"+data.mac+\"' value='1' onclick='handleRadio(event)'/>정회전<input type='radio' name='inveter' id='\"+data.mac+\"' value='2' onclick='handleRadio(event)'/>역회전\";\n                    document.getElementById(data.mac+'-vel').innerHTML = \"속도<input type='range' id='\"+data.mac+\"' min='0' max='6000' value='2000' step='10' onchange='handleRange(event)'>\";\n                }\n                 else if(data.type==27) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"풍속:\"+data.windspeed;\n                }\n                else if(data.type==28) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"풍향:\"+data.wind_direction;\n                }\n                else if(data.type==29) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:29,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:29,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:29,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:29,^outNo^:1,^value^:1}\\\");'></button>\";\n                }\n            }\n            ws.onopen = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"connected\";\n                //ws.send(\"Open for data\");\n                console.log(\"connected\");\n            }\n            ws.onclose = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"not connected\";\n                // in case of lost connection tries to reconnect every 3 secs\n                setTimeout(wsConnect,3000);\n            }\n        }\n        \n        function doit(m) {\n            if (ws) { ws.send(m); }\n        }\n        \n        function handleRange(e){\n            doit(\"{'mac':'\"+e.target.id+\"','vel':\"+e.target.value+\"}\");\n        }\n        \n        function handleRadio(e){\n            doit(\"{'mac':'\"+e.target.id+\"','dir':\"+e.target.value+\"}\");\n        }\n",
        "x": 1470,
        "y": 220,
        "wires": [
            [
                "02109c10105b4145"
            ]
        ]
    },
    {
        "id": "b29f5da82751b74c",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "insert one (수정)",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar model=[];\nmodel[2]='PE-300';\nmodel[3]='키우미';\nmodel[4]='KSM-8900';\nmodel[5]='simpleTH';\nmodel[6]='XEX-DR14E';\nmodel[7]='DHT11';\nmodel[8]='RY-WS301';\nmodel[9]='MP-508E';\nmodel[10]='사운드';\nmodel[11]='충격센서';\nmodel[12]='PLC-RS485';\nmodel[13]='차양모터';\nmodel[14]='MASTER-K120s';\nmodel[15]='smoke';\nmodel[16]='methane';\nmodel[17]='ILLUMINACE SENSOR';\nmodel[18]='PLC-RS485-8ch';\nmodel[20]='Leaf Humidity';\nmodel[21]='RK520-01';\nmodel[23]='Xiaomi Flower';\nmodel[24]='Xiaomi Temp Humidity SENSOR';\nmodel[25]='Loadcell'\nmodel[26]='VFD-M Inverter'\nmodel[27]='SWWS-7320';\nmodel[28]='SWWS-7300';\nmodel[29]='valve';\n\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nif(msg.payload.type==6)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0] };\nelse if(msg.payload.type==12 || msg.payload.type==14)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0] };\nelse if(msg.payload.type==29)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0] };\nelse if(msg.payload.type==18)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0,0,0] };\nelse\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1020,
        "y": 546,
        "wires": [
            [
                "ea32a2d1f9cad461"
            ]
        ]
    },
    {
        "id": "843960b9ee8dbf4c",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOneUpdate (수정)",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\nelse if(msg.payload.type==10)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'sound':msg.payload.sound\n    , 'time':time }} ];\nelse if(msg.payload.type==11)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'knock':msg.payload.knock\n    , 'time':time }} ];\nelse if(msg.payload.type==12)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==13)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time\n     }} ];\nelse if(msg.payload.type==14)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{'ip':msg.payload.ip, 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5), 'time':time }} ];\nelse if(msg.payload.type==15)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==16)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==17)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'lux':msg.payload.lux,'time':time}} ];\nelse if(msg.payload.type==18)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), \n    'out.0':msg.payload.out.charAt(0), 'out.1':msg.payload.out.charAt(1),'out.2':msg.payload.out.charAt(2), 'out.3':msg.payload.out.charAt(3),\n    'out.4':msg.payload.out.charAt(4), 'out.5':msg.payload.out.charAt(5),'out.6':msg.payload.out.charAt(6), 'out.7':msg.payload.out.charAt(7),\n    'time':time\n     }} ];\nelse if(msg.payload.type==20)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'hum':msg.payload.hum, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==21)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==21)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==23)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi, 'temp':msg.payload.temp,\n    'light':msg.payload.light, 'condu':msg.payload.condu, 'batt':msg.payload.batt, 'time':time }} ];\nelse if(msg.payload.type==24)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==25)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'Loadcell':msg.payload.Loadcell, 'time':time }} ];\nelse if(msg.payload.type==26)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'dir':msg.payload.dir, 'vel':msg.payload.vel, 'time':time }} ];\nelse if(msg.payload.type==27)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'wind':msg.payload.windspeed,'time':time}} ];\nelse if(msg.payload.type==28)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'wind_direction':msg.payload.wind_direction,'time':time}} ];\nelse if(msg.payload.type==29)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1), 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'time':time}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1020,
        "y": 600,
        "wires": [
            [
                "b73b6d8cf43f80ba"
            ]
        ]
    },
    {
        "id": "8dd3979983a233e6",
        "type": "function",
        "z": "5a4ce9cc8e6f352a",
        "name": "findOneUpdate (출력있는 경우만 수정)",
        "func": "global.set(\"msg_in\",msg);\nvar time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\n\nif(msg.payload.type==6 ||msg.payload.type==12 || msg.payload.type==14 || msg.payload.type==18 || msg.payload.type==29) {\n    if(msg.payload.outNo == 0)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.0':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 1)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.1':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 2)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.2':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 3)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.3':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 4)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.4':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 5)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.5':msg.payload.value, 'time':time }} ];\n}\nelse if(msg.payload.type==13) \n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time }} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 850,
        "y": 660,
        "wires": [
            [
                "29e8bf16fa951973"
            ]
        ]
    },
    {
        "id": "3f532047eec5125b",
        "type": "debug",
        "z": "5a4ce9cc8e6f352a",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 560,
        "y": 500,
        "wires": []
    },
    {
        "id": "f15d66b3.c7edf8",
        "type": "mqtt-broker",
        "name": "",
        "broker": "broker.mqtt-dashboard.com",
        "port": "1883",
        "tls": "509a45a9.089bbc",
        "clientid": "",
        "autoConnect": true,
        "usetls": false,
        "protocolVersion": "3",
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
        "id": "5ffbb241da37a3ee",
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
        "id": "bf4882ef33e69fa3",
        "type": "websocket-listener",
        "path": "/ws/simple",
        "wholemsg": "false"
    },
    {
        "id": "509a45a9.089bbc",
        "type": "tls-config",
        "name": "",
        "cert": "",
        "key": "",
        "ca": "",
        "certname": "94191b3ef7-certificate.pem.crt",
        "keyname": "94191b3ef7-private.pem.key",
        "caname": "MtestCA.pem",
        "servername": "",
        "verifyservercert": true,
        "alpnprotocol": ""
    }
]

```
