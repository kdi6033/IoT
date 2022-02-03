# 연기센서 node-red ui활용하기(worldmap)

![제목을-입력해주세요_-001](https://user-images.githubusercontent.com/37902752/152289597-d584e959-d1a2-4c20-9843-11e4aac3741b.jpg)

## node-red worldmap
---

*node-red 불러오기 (node-red.txt 똑같습니다.)
```   
[
    {
        "id": "606f791661d959c9",
        "type": "tab",
        "label": "플로우 1",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "3caaae3e3a35405e",
        "type": "ui_gauge",
        "z": "606f791661d959c9",
        "name": "",
        "group": "0a8a5f9339645ba3",
        "order": 1,
        "width": 3,
        "height": 3,
        "gtype": "wave",
        "title": "gas",
        "label": "obs/m",
        "format": "{{value}}",
        "min": 0,
        "max": "1000",
        "colors": [
            "#00b500",
            "#e6e600",
            "#ca3838"
        ],
        "seg1": "",
        "seg2": "",
        "className": "",
        "x": 790,
        "y": 120,
        "wires": []
    },
    {
        "id": "3650f016756c80f3",
        "type": "ui_audio",
        "z": "606f791661d959c9",
        "name": "",
        "group": "0a8a5f9339645ba3",
        "voice": "Microsoft Heami - Korean (Korean)",
        "always": false,
        "x": 800,
        "y": 160,
        "wires": []
    },
    {
        "id": "a29ae3b3429584fa",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "msg.payload=\"게이지 값은\"+msg.payload.gas+\"입니다.\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 640,
        "y": 160,
        "wires": [
            []
        ]
    },
    {
        "id": "5ce7a8856f235d4f",
        "type": "link in",
        "z": "606f791661d959c9",
        "name": "",
        "links": [
            "71680d1f9b1264ca"
        ],
        "x": 415,
        "y": 120,
        "wires": [
            [
                "bbfc784ce1c4ba94",
                "a29ae3b3429584fa",
                "d9d0110dd9d7d92f",
                "54fcec7e1bcaadfb",
                "9deeef76f524adf4"
            ]
        ]
    },
    {
        "id": "bbfc784ce1c4ba94",
        "type": "debug",
        "z": "606f791661d959c9",
        "name": "",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 650,
        "y": 60,
        "wires": []
    },
    {
        "id": "d9d0110dd9d7d92f",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "gas",
        "func": "msg.payload = msg.payload.gas\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 630,
        "y": 120,
        "wires": [
            [
                "3caaae3e3a35405e",
                "ab77e82449b3488e",
                "0342503d544f019c"
            ]
        ]
    },
    {
        "id": "54fcec7e1bcaadfb",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "if(msg.payload.gas > 300)\n    msg.payload=\"경보  경보\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 640,
        "y": 200,
        "wires": [
            [
                "a22189391023c791",
                "8d32559062734d42"
            ]
        ]
    },
    {
        "id": "a22189391023c791",
        "type": "ui_audio",
        "z": "606f791661d959c9",
        "name": "",
        "group": "0a8a5f9339645ba3",
        "voice": "Microsoft Heami - Korean (Korean)",
        "always": "",
        "x": 800,
        "y": 200,
        "wires": []
    },
    {
        "id": "30914980c2fa3a0f",
        "type": "template",
        "z": "606f791661d959c9",
        "name": "",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<iframe src=\"../worldmap\" height=500px width=\"100%\" ></iframe>",
        "x": 800,
        "y": 340,
        "wires": [
            [
                "58c14c0ca9410ac2",
                "22fcb34a60eac8e0"
            ]
        ]
    },
    {
        "id": "fa22629853d86527",
        "type": "worldmap",
        "z": "606f791661d959c9",
        "name": "",
        "lat": "",
        "lon": "",
        "zoom": "",
        "layer": "OSMG",
        "cluster": "",
        "maxage": "",
        "usermenu": "show",
        "layers": "show",
        "panit": "false",
        "panlock": "false",
        "zoomlock": "false",
        "hiderightclick": "false",
        "coords": "none",
        "showgrid": "false",
        "allowFileDrop": "false",
        "path": "/worldmap",
        "overlist": "DR,CO,RA,DN,HM",
        "maplist": "OSMG,OSMC,EsriC,EsriS,EsriT,EsriDG,UKOS,SW",
        "mapname": "",
        "mapurl": "",
        "mapopt": "",
        "mapwms": false,
        "x": 960,
        "y": 280,
        "wires": []
    },
    {
        "id": "5c9fbd1e7d02f27e",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "msg.payload = {\n    name:\"서울 사무실\",\n    lat:37.56372,\n    lon:126.91256,\n    icon: \"power-off\",\n    iconColor:msg.iconColor,\n    \"command\": { \"layer\":\"OSM\", \"lat\":37.56372, \"lon\":126.91256, \"zoom\":10 },\n    gas:msg.gas\n};\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 800,
        "y": 260,
        "wires": [
            [
                "d91fa685887f2343",
                "fa22629853d86527"
            ]
        ]
    },
    {
        "id": "d91fa685887f2343",
        "type": "debug",
        "z": "606f791661d959c9",
        "name": "",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 970,
        "y": 240,
        "wires": []
    },
    {
        "id": "ab77e82449b3488e",
        "type": "debug",
        "z": "606f791661d959c9",
        "name": "",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 1170,
        "y": 120,
        "wires": []
    },
    {
        "id": "d1349520e0e49e6b",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "//msg.payload = { \"command\": { \"layer\":\"Esri Satellite\", \"lat\":51, \"lon\":3, \"zoom\":10 }};//지정위치로 맵에서 이동\n/*var imageBounds = [[40.712216, -74.22655], [40.773941, -74.12544]];\nmsg.payload = { command : { lat:40.74, lon:-74.175, zoom:13 } };\nmsg.payload.command.map = {\n    overlay: \"New York Historical\",\n    url: 'https://www.lib.utexas.edu/maps/historical/newark_nj_1922.jpg',\n    bounds: imageBounds,\n    opt: { opacity:0.8, attribution:\"&copy; University of Texas\" }\n};*/   //사진 띄우고 줌 인\n/*msg.payload = { command : { grid : {\n    showgrid: true,\n    opt: { showLabel:true, dashArray:[5, 5], fontColor:\"#900\" }\n  }\n }\n};*/ //눈금 추가\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 100,
        "y": 1140,
        "wires": [
            []
        ]
    },
    {
        "id": "dc83a6f6a186fd66",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "var newMsg={\n};\nif(msg.payload.gas > 200)\n    newMsg.iconColor=\"red\";\nelse if(msg.payload.gas < 200)\n    newMsg.iconColor=\"black\";\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 260,
        "y": 1100,
        "wires": [
            []
        ]
    },
    {
        "id": "9f7d905ee88e1d8c",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "var newMsg1={\n};\n    newMsg1.gas=msg.payload.gas;\nreturn newMsg1;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 260,
        "y": 1140,
        "wires": [
            []
        ]
    },
    {
        "id": "9deeef76f524adf4",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "",
        "func": "var newMsg={\n};\nif(msg.payload.gas > 300)\n    newMsg.iconColor=\"red\";\nelse if(msg.payload.gas < 300)\n    newMsg.iconColor=\"black\";\nnewMsg.gas=msg.payload.gas;    \nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 640,
        "y": 260,
        "wires": [
            [
                "5c9fbd1e7d02f27e"
            ]
        ]
    },
    {
        "id": "58c14c0ca9410ac2",
        "type": "ui_worldmap",
        "z": "606f791661d959c9",
        "group": "0a8a5f9339645ba3",
        "order": 6,
        "width": 6,
        "height": 5,
        "name": "",
        "lat": "",
        "lon": "",
        "zoom": "",
        "layer": "OSMG",
        "cluster": "",
        "maxage": "",
        "usermenu": "hide",
        "layers": "hide",
        "panit": "false",
        "panlock": "false",
        "zoomlock": "false",
        "hiderightclick": "true",
        "coords": "none",
        "showgrid": "false",
        "allowFileDrop": "false",
        "path": "/worldmap",
        "overlist": "DR,CO,RA,DN,HM",
        "maplist": "OSMG,OSMC,EsriC,EsriS,EsriT,EsriDG,UKOS,SW",
        "mapname": "",
        "mapurl": "",
        "mapopt": "",
        "mapwms": false,
        "x": 960,
        "y": 340,
        "wires": []
    },
    {
        "id": "dcc2ae2425d85ee2",
        "type": "ui_button",
        "z": "606f791661d959c9",
        "name": "",
        "group": "0a8a5f9339645ba3",
        "order": 5,
        "width": 6,
        "height": 1,
        "passthru": false,
        "label": "지도 초기값",
        "tooltip": "",
        "color": "",
        "bgcolor": "",
        "className": "",
        "icon": "",
        "payload": "0",
        "payloadType": "num",
        "topic": "topic",
        "topicType": "msg",
        "x": 230,
        "y": 260,
        "wires": [
            [
                "5028d64d76033a34",
                "9deeef76f524adf4"
            ]
        ]
    },
    {
        "id": "96cd004ab626ff6e",
        "type": "mongodb2 in",
        "z": "606f791661d959c9",
        "service": "_ext_",
        "configNode": "dcbfb6eb.1d4318",
        "name": "",
        "collection": "",
        "operation": "",
        "x": 590,
        "y": 400,
        "wires": [
            [
                "b2e6e6e727f83be9",
                "9deeef76f524adf4"
            ]
        ]
    },
    {
        "id": "b2e6e6e727f83be9",
        "type": "debug",
        "z": "606f791661d959c9",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 770,
        "y": 400,
        "wires": []
    },
    {
        "id": "01defc876d76f3fd",
        "type": "ui_button",
        "z": "606f791661d959c9",
        "name": "",
        "group": "6821af6a6517f957",
        "order": 3,
        "width": 18,
        "height": 1,
        "passthru": false,
        "label": "지도 초기값",
        "tooltip": "",
        "color": "",
        "bgcolor": "",
        "className": "",
        "icon": "",
        "payload": "0",
        "payloadType": "num",
        "topic": "topic",
        "topicType": "msg",
        "x": 230,
        "y": 300,
        "wires": [
            [
                "5028d64d76033a34",
                "9deeef76f524adf4"
            ]
        ]
    },
    {
        "id": "0342503d544f019c",
        "type": "ui_gauge",
        "z": "606f791661d959c9",
        "name": "",
        "group": "6821af6a6517f957",
        "order": 2,
        "width": 8,
        "height": 8,
        "gtype": "wave",
        "title": "gas",
        "label": "obs/m",
        "format": "{{value}}",
        "min": 0,
        "max": "1000",
        "colors": [
            "#00b500",
            "#e6e600",
            "#ca3838"
        ],
        "seg1": "",
        "seg2": "",
        "className": "",
        "x": 950,
        "y": 100,
        "wires": []
    },
    {
        "id": "be26b907fe40a9ca",
        "type": "ui_audio",
        "z": "606f791661d959c9",
        "name": "",
        "group": "6821af6a6517f957",
        "voice": "Microsoft Heami - Korean (Korean)",
        "always": "",
        "x": 960,
        "y": 160,
        "wires": []
    },
    {
        "id": "8d32559062734d42",
        "type": "ui_audio",
        "z": "606f791661d959c9",
        "name": "",
        "group": "6821af6a6517f957",
        "voice": "Microsoft Heami - Korean (Korean)",
        "always": "",
        "x": 960,
        "y": 200,
        "wires": []
    },
    {
        "id": "22fcb34a60eac8e0",
        "type": "ui_worldmap",
        "z": "606f791661d959c9",
        "group": "6821af6a6517f957",
        "order": 6,
        "width": 18,
        "height": 13,
        "name": "",
        "lat": "",
        "lon": "",
        "zoom": "",
        "layer": "OSMG",
        "cluster": "",
        "maxage": "",
        "usermenu": "hide",
        "layers": "hide",
        "panit": "false",
        "panlock": "false",
        "zoomlock": "false",
        "hiderightclick": "true",
        "coords": "none",
        "showgrid": "false",
        "allowFileDrop": "false",
        "path": "/worldmap",
        "overlist": "DR,CO,RA,DN,HM",
        "maplist": "OSMG,OSMC,EsriC,EsriS,EsriT,EsriDG,UKOS,SW",
        "mapname": "",
        "mapurl": "",
        "mapopt": "",
        "mapwms": false,
        "x": 960,
        "y": 400,
        "wires": []
    },
    {
        "id": "5028d64d76033a34",
        "type": "function",
        "z": "606f791661d959c9",
        "name": "findOne",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 460,
        "y": 400,
        "wires": [
            [
                "96cd004ab626ff6e"
            ]
        ]
    },
    {
        "id": "ad8fb98ae9977e1c",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "0a8a5f9339645ba3",
        "order": 2,
        "width": 3,
        "height": 1
    },
    {
        "id": "507251ae0bd5a2c2",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "0a8a5f9339645ba3",
        "order": 3,
        "width": 3,
        "height": 1
    },
    {
        "id": "f2924321243b0573",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "0a8a5f9339645ba3",
        "order": 4,
        "width": 3,
        "height": 1
    },
    {
        "id": "1874819af3246e28",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 1,
        "width": 2,
        "height": 1
    },
    {
        "id": "a737836f08bc1a0c",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 4,
        "width": 2,
        "height": 1
    },
    {
        "id": "be6707173155e522",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 5,
        "width": 2,
        "height": 1
    },
    {
        "id": "2637bad7b0b35d58",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 7,
        "width": 2,
        "height": 1
    },
    {
        "id": "3a99d26e1877d7ed",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 8,
        "width": 2,
        "height": 1
    },
    {
        "id": "c5ef5c937316adf4",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 9,
        "width": 2,
        "height": 1
    },
    {
        "id": "b8714d6e837bfe87",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 10,
        "width": 2,
        "height": 1
    },
    {
        "id": "b42d6fe25278b5bb",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 11,
        "width": 2,
        "height": 1
    },
    {
        "id": "2002ead7ed722d9f",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 12,
        "width": 2,
        "height": 1
    },
    {
        "id": "f020733a2172c2d9",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 13,
        "width": 2,
        "height": 1
    },
    {
        "id": "13d8ad740188b21e",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 14,
        "width": 2,
        "height": 1
    },
    {
        "id": "9169f20e9d51e3e9",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 15,
        "width": 2,
        "height": 1
    },
    {
        "id": "965c024266d6de03",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 16,
        "width": 2,
        "height": 1
    },
    {
        "id": "415e402ed7ed225d",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 17,
        "width": 2,
        "height": 1
    },
    {
        "id": "43b5f18e6ccd9c63",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 18,
        "width": 2,
        "height": 1
    },
    {
        "id": "974959240da08d16",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 19,
        "width": 2,
        "height": 1
    },
    {
        "id": "2cdb2eaed4544789",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 20,
        "width": 10,
        "height": 1
    },
    {
        "id": "a5f60ae10a7f0d99",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 21,
        "width": 2,
        "height": 1
    },
    {
        "id": "01199b4f51d160c3",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 22,
        "width": 10,
        "height": 1
    },
    {
        "id": "2ade8743588d4fe0",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 23,
        "width": 2,
        "height": 1
    },
    {
        "id": "6aa5a5dbe8a030b9",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 24,
        "width": 10,
        "height": 1
    },
    {
        "id": "d3eab9797125190d",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 25,
        "width": 2,
        "height": 1
    },
    {
        "id": "73039b0dc69421a7",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 26,
        "width": 10,
        "height": 1
    },
    {
        "id": "c5b859d3f9c4991c",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 27,
        "width": 2,
        "height": 1
    },
    {
        "id": "43cc61bc3bb41ba9",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 28,
        "width": 10,
        "height": 1
    },
    {
        "id": "e7dfb4f535016ca7",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 29,
        "width": 2,
        "height": 1
    },
    {
        "id": "b397bed9fa0eec89",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 30,
        "width": 10,
        "height": 1
    },
    {
        "id": "dc02deddea8e7281",
        "type": "ui_spacer",
        "z": "606f791661d959c9",
        "name": "spacer",
        "group": "6821af6a6517f957",
        "order": 31,
        "width": 2,
        "height": 1
    },
    {
        "id": "0a8a5f9339645ba3",
        "type": "ui_group",
        "name": "연기측정",
        "tab": "a346644b60a5ceba",
        "order": 1,
        "disp": true,
        "width": "6",
        "collapse": false,
        "className": ""
    },
    {
        "id": "dcbfb6eb.1d4318",
        "type": "mongodb2",
        "uri": "mongodb://localhost:27000/local",
        "name": "local",
        "options": "",
        "parallelism": "-1"
    },
    {
        "id": "6821af6a6517f957",
        "type": "ui_group",
        "name": "연기측정",
        "tab": "fdbb833a654661c3",
        "order": 1,
        "disp": true,
        "width": "30",
        "collapse": false,
        "className": ""
    },
    {
        "id": "a346644b60a5ceba",
        "type": "ui_tab",
        "name": "현대하이텍(6x6 스마트폰)",
        "icon": "dashboard",
        "order": 1,
        "disabled": false,
        "hidden": false
    },
    {
        "id": "fdbb833a654661c3",
        "type": "ui_tab",
        "name": "현대하이텍(laptop)",
        "icon": "dashboard",
        "order": 3,
        "disabled": false,
        "hidden": false
    }
]
```   

## 서버 라이트 홈피 노드
---

*위의 노드를 붙여넣기 전에 이전에 올려두었던 서버 라이트 홈피 노드가 있어야 합니다.

```   
[
    {
        "id": "e4dab0afad2daadd",
        "type": "subflow",
        "name": "Iterate",
        "in": [
            {
                "x": 220,
                "y": 219,
                "wires": [
                    {
                        "id": "539c210799f59b0a"
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
                        "id": "539c210799f59b0a",
                        "port": 0
                    }
                ]
            },
            {
                "x": 455,
                "y": 259,
                "wires": [
                    {
                        "id": "539c210799f59b0a",
                        "port": 1
                    }
                ]
            }
        ]
    },
    {
        "id": "539c210799f59b0a",
        "type": "function",
        "z": "e4dab0afad2daadd",
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
        "id": "3a4cfca6de027178",
        "type": "tab",
        "label": "서버 라이트 홈피",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "a37d07fb96b51491",
        "type": "mqtt in",
        "z": "3a4cfca6de027178",
        "name": "",
        "topic": "/i2r/outTopic",
        "qos": "0",
        "datatype": "auto",
        "broker": "f15d66b3.c7edf8",
        "nl": false,
        "rap": true,
        "rh": 0,
        "inputs": 0,
        "x": 230,
        "y": 440,
        "wires": [
            [
                "5d9b5c43936805a1"
            ]
        ]
    },
    {
        "id": "c82cfbe92a99723d",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOne",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac, 'type':msg.payload.type};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 520,
        "y": 440,
        "wires": [
            [
                "1a1ee9ceb8d3fbff"
            ]
        ]
    },
    {
        "id": "745b28d778cd1a88",
        "type": "switch",
        "z": "3a4cfca6de027178",
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
        "x": 790,
        "y": 440,
        "wires": [
            [
                "d32b7adf874e57b3"
            ],
            [
                "4ec858ac1ecde328"
            ]
        ]
    },
    {
        "id": "d32b7adf874e57b3",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "insert one",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar model=[];\nmodel[2]='PE-300';\nmodel[3]='키우미';\nmodel[4]='KSM-8900';\nmodel[5]='simpleTH';\nmodel[6]='XEX-DR14E';\nmodel[7]='DHT11';\nmodel[8]='RY-WS301';\nmodel[9]='MP-508E';\nmodel[10]='사운드';\nmodel[11]='충격센서';\nmodel[12]='PLC-RS485';\nmodel[13]='차양모터';\nmodel[14]='MASTER-K120s';\nmodel[15]='smoke';\nmodel[16]='methane';\n\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'insert';\nif(msg.payload.type==6)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0,0,0] };\nelse if(msg.payload.type==12 || msg.payload.type==14)\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':'', 'out':[0,0,0,0] };\nelse\n    newMsg.payload = {'type':msg.payload.type,'mac':msg.payload.mac,'model':model[msg.payload.type],'ip':msg.payload.ip, 'name':''};\nnewMsg.payload.mac = msg.payload.mac;\nnewMsg.projection = { 'email' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 932,
        "y": 427,
        "wires": [
            [
                "85b500031bd888e6"
            ]
        ]
    },
    {
        "id": "4ec858ac1ecde328",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOneUpdate",
        "func": "var time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nif(msg.payload.type==2)\n    newMsg.payload = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'ec':msg.payload.ec, 'ph':msg.payload.ph, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==3)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'co2':msg.payload.co2, 'time':time}} ];\nelse if(msg.payload.type==4)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'mo':msg.payload.mo, 'tem':msg.payload.tem, 'ec':msg.payload.ec, 'time':time}} ];\nelse if(msg.payload.type==5)\n    newMsg.payload    = [{ 'mac' : msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip, 'humi':msg.payload.humi, 'temp':msg.payload.temp, 'time':time}} ];\nelse if(msg.payload.type==6)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==7)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'time':time }} ];\nelse if(msg.payload.type==8)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'pres':msg.payload.pres, 'time':time }} ];\nelse if(msg.payload.type==9)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'humi':msg.payload.humi\n    , 'temp':msg.payload.temp, 'ec':msg.payload.ec, 'time':time }} ];\nelse if(msg.payload.type==10)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'sound':msg.payload.sound\n    , 'time':time }} ];\nelse if(msg.payload.type==11)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'knock':msg.payload.knock\n    , 'time':time }} ];\nelse if(msg.payload.type==12)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5)\n    , 'in.6':msg.payload.in.charAt(6), 'in.7':msg.payload.in.charAt(7), 'time':time\n     }} ];\nelse if(msg.payload.type==13)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time\n     }} ];\nelse if(msg.payload.type==14)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{'ip':msg.payload.ip, 'in.0':msg.payload.in.charAt(0), 'in.1':msg.payload.in.charAt(1)\n    , 'in.2':msg.payload.in.charAt(2), 'in.3':msg.payload.in.charAt(3), 'in.4':msg.payload.in.charAt(4), 'in.5':msg.payload.in.charAt(5), 'time':time }} ];\nelse if(msg.payload.type==15)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\nelse if(msg.payload.type==16)\n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'ip':msg.payload.ip,'gas':msg.payload.gas\n    , 'time':time }} ];\n    \nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 960,
        "y": 480,
        "wires": [
            [
                "f27515fcbe3ea1f5"
            ]
        ]
    },
    {
        "id": "5d9b5c43936805a1",
        "type": "json",
        "z": "3a4cfca6de027178",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 377,
        "y": 440,
        "wires": [
            [
                "c82cfbe92a99723d",
                "71680d1f9b1264ca"
            ]
        ]
    },
    {
        "id": "ba496d907ee7af61",
        "type": "http in",
        "z": "3a4cfca6de027178",
        "name": "",
        "url": "/login",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 280,
        "y": 69,
        "wires": [
            [
                "bf192058a4ac5513"
            ]
        ]
    },
    {
        "id": "f1d333e7a32b7b63",
        "type": "template",
        "z": "3a4cfca6de027178",
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
                "7b5e421251a9feab"
            ]
        ]
    },
    {
        "id": "8e3a644bff4e519c",
        "type": "template",
        "z": "3a4cfca6de027178",
        "name": "script",
        "field": "payload.script",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "// Get the modal\nvar modal = document.getElementById('id01');\n\n// When the user clicks anywhere outside of the modal, close it\nwindow.onclick = function(event) {\n    if (event.target == modal) {\n        modal.style.display = \"none\";\n    }\n}\n\nfunction openNav() {\n  document.getElementById(\"mySidenav\").style.width = \"150px\"; \n}\nfunction closeNav() {\n  document.getElementById(\"mySidenav\").style.width = \"0\";\n}\n",
        "output": "str",
        "x": 565,
        "y": 70,
        "wires": [
            [
                "f1d333e7a32b7b63"
            ]
        ]
    },
    {
        "id": "ee2fc651219cbabd",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "global style",
        "func": "global.set(\"style\",msg.payload.style);\nglobal.set(\"script\",msg.payload.script);\nglobal.set(\"menu\",msg.payload.menu);\nglobal.set(\"msg_main\",msg);\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 961,
        "y": 70,
        "wires": [
            [
                "fcf6f5f1ce993b3b"
            ]
        ]
    },
    {
        "id": "7b5e421251a9feab",
        "type": "template",
        "z": "3a4cfca6de027178",
        "name": "menu",
        "field": "payload.menu",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "  <div id='mySidenav' class='sidenav'>\n  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>\n  <a href='/display?act=3'>홈</a>\n  <a href='https://github.com/kdi6033/IoT' target='_blank'>메뉴얼</a>\n  <a href='http://i2r.link' target='_blank'>김동일홈피</a>\n  </div>\n  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>\n    메뉴<br>  ",
        "x": 810,
        "y": 70,
        "wires": [
            [
                "ee2fc651219cbabd"
            ]
        ]
    },
    {
        "id": "342e057542f2d15e",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "find.toArray",
        "func": "global.set(\"msg_main\",msg);\nmsg.collection = 'localRecord';\nmsg.operation  =  'find.toArray';\nmsg.payload    = { };\nmsg.projection = { 'name' : 1 , '_id' : 0 };\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 180,
        "wires": [
            [
                "414fb6a539d87582"
            ]
        ]
    },
    {
        "id": "8073941b663b6bc7",
        "type": "http response",
        "z": "3a4cfca6de027178",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1750,
        "y": 180,
        "wires": []
    },
    {
        "id": "805923aa9208a220",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "0,1,2,3 List",
        "func": "msg.payload.style=global.get(\"style\");\nmsg.payload.script=global.get(\"script\");\nmsg.payload.menu=global.get(\"menu\");\nvar i,j;\nvar s;\nvar r;\nsOut=\"<table>\"\nsOut+=\"<tr>   <th>모델</th>  <th>모니터링</th> <th>이름</th> <th>IP</th> </tr>\"\nfor(i=0;i<msg.payload.length;i++) {\n    //r=\"<input type='hidden' name='chip' value='\"+msg.payload[i].chip+\"'>\";\n    s=\"\";\n    s+=\"<tr><th>\";\n    s+=\"<form action='/display'>\";\n        s+=\"<input type='hidden' name='mac' value='\"+msg.payload[i].mac+\"'>\";\n        s+=\"<input type='hidden' name='act' value='1'>\";\n        s+=\"<button type='submit' name='value' value='0' class='button buttonM'>\"+msg.payload[i].model+\"</button></a>\";\n    s+=\"</form>\";\n    s+=\"</th>\";\n    if(msg.payload[i].type==2) {\n        //s+=\"<td>ec:\"+msg.payload[i].ec+\"  ph:\"+msg.payload[i].ph+\"  온도:\"+msg.payload[i].temp+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==3) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"  CO2:\"+msg.payload[i].co2+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==4) {\n        //s+=\"<th>습도:\"+msg.payload[i].mo+\"  온도:\"+msg.payload[i].tem+\"  EC:\"+msg.payload[i].ec+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    }\n    else if(msg.payload[i].type==5) {\n        //s+=\"<th>습도:\"+msg.payload[i].humi+\"  온도:\"+msg.payload[i].temp+\"</th> \"\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<th>\"+msg.payload[i].name+\"</th>\"\n    } \n    /*\n    else if(msg.payload[i].type==6) {\n        s+=\"<td>\"\n            for(var j=0;j<8;j++) {\n                if(msg.payload[i].in[j]==1) \n                    s+=\"<button class='button button-ledon'>\"+j+\"</button>&nbsp;\";\n                else\n                    s+=\"<button class='button button-ledoff'>\"+j+\"</button>&nbsp;\";\n            }\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    */\n    else if(msg.payload[i].type==6) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<8;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==7) {\n        s+=\"<td>\";\n        for(j=0;j<2;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==8) {\n        //s+=\"<td>습도:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  대기압:\"+msg.payload[i].pres+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==9) {\n        //s+=\"<td>함수율:\"+msg.payload[i].humi+\"%  온도:\"+msg.payload[i].temp+\"도  EC:\"+msg.payload[i].ec+\"</td> \"\n        s+=\"<td>\";\n        for(j=0;j<3;j++)\n            s+=\"<span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==10) {\n        //s+=\"<td>사운드:\"+msg.payload[i].sound+\" \"+msg.payload[i].time+\"</td> \";\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==11) {\n        //s+=\"<td>충격발생:\"+msg.payload[i].time+\"</td> \"\n        s+=\"<td><span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==12) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==13) {\n        s+=\"<td>\"\n            s+=\"<table align='center' style='margin: 0px auto;'>\";\n            s+=\"<tr>\";\n            s+=\"<td>정지</td>\";\n            s+=\"<td>올림</td>\";\n            s+=\"<td>내림</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==14) {\n        s+=\"<td>\"\n            s+=\"<table>\";\n            s+=\"<tr>\";\n            s+=\"<td></td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td>\"+String(j)+\"</td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>입력</td>\";\n            for(j=0;j<6;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-in\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"<tr>\";\n            s+=\"<td>출력</td>\";\n            for(j=0;j<4;j++)\n                s+=\"<td><span id='\"+msg.payload[i].mac+\"-out\"+String(j)+\"'></span></td>\";\n            s+=\"</tr>\";\n            s+=\"</table>\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==15) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    else if(msg.payload[i].type==16) {\n        s+=\"<td>\";\n        s+=\"<span id='\"+msg.payload[i].mac+\"-in0'></span>&emsp;\";\n        s+=\"</td>\";\n        s+=\"<td>\"+msg.payload[i].name+\"</td>\";\n    }\n    \n    s+=\"<td><a href='http://\"+msg.payload[i].ip+\"' target='_blank'>\"+msg.payload[i].ip+\"</a></td>\";\n    s+=\"</tr>\"\n    \n    sOut=sOut+s+\"<br>\";\n} \n\nmsg.payload.list=sOut;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1210,
        "y": 180,
        "wires": [
            [
                "19cb87ccd859975b",
                "87402721d6aaa4ee",
                "c5e64b85cc720684"
            ]
        ]
    },
    {
        "id": "d82b881eadd280ca",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "msg 받음",
        "func": "var msg1=msg.payload;\nvar msg=global.get(\"msg_main\")||\"\";\nmsg.payload=msg1;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1040,
        "y": 180,
        "wires": [
            [
                "805923aa9208a220"
            ]
        ]
    },
    {
        "id": "0aef6ca39988045f",
        "type": "http in",
        "z": "3a4cfca6de027178",
        "name": "",
        "url": "/display",
        "method": "get",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 150,
        "wires": [
            [
                "6ba1aab88e62aa25"
            ]
        ]
    },
    {
        "id": "2975efe3da057b18",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "Display",
        "info": "1: 선택한 기기 config\n2: 메뉴의 환경설정\n3: main  기기 리스트 모니터링\n4: 메뉴얼",
        "x": 114,
        "y": 150,
        "wires": []
    },
    {
        "id": "f66a265a66a3147a",
        "type": "function",
        "z": "3a4cfca6de027178",
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
                "9a9f4d088446da3f"
            ]
        ]
    },
    {
        "id": "9a9f4d088446da3f",
        "type": "template",
        "z": "3a4cfca6de027178",
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
                "8237b76696a2fa7b"
            ]
        ]
    },
    {
        "id": "5f7b89dc395f1fc2",
        "type": "function",
        "z": "3a4cfca6de027178",
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
                "2ccaa283683214f4"
            ]
        ]
    },
    {
        "id": "8237b76696a2fa7b",
        "type": "http response",
        "z": "3a4cfca6de027178",
        "name": "",
        "statusCode": "",
        "headers": {},
        "x": 1522,
        "y": 130,
        "wires": []
    },
    {
        "id": "6ba1aab88e62aa25",
        "type": "function",
        "z": "3a4cfca6de027178",
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
                "7b59877105eaf47a"
            ]
        ]
    },
    {
        "id": "ab5262a78b759bda",
        "type": "function",
        "z": "3a4cfca6de027178",
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
                "f66a265a66a3147a"
            ]
        ]
    },
    {
        "id": "7b59877105eaf47a",
        "type": "switch",
        "z": "3a4cfca6de027178",
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
                "5f7b89dc395f1fc2"
            ],
            [],
            [
                "342e057542f2d15e"
            ],
            []
        ]
    },
    {
        "id": "8187a446eeedd881",
        "type": "http in",
        "z": "3a4cfca6de027178",
        "name": "",
        "url": "/save",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 290,
        "y": 260,
        "wires": [
            [
                "e5e61a99ef4d949e"
            ]
        ]
    },
    {
        "id": "b80f4fd794a948c7",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "Save",
        "info": "1: name 저장\n",
        "x": 114,
        "y": 260,
        "wires": []
    },
    {
        "id": "7577faf61c210736",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\nnewMsg.payload    = [{ 'mac' : msg.payload.mac}, {$set:{ 'name':msg.payload.name}} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 600,
        "y": 260,
        "wires": [
            [
                "55aca276c80b6830"
            ]
        ]
    },
    {
        "id": "154eb4dff43b5503",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "8877bf44bbdfe8ae",
            "2044220.61e99de"
        ],
        "x": 1015,
        "y": 260,
        "wires": []
    },
    {
        "id": "28f037425e56a0db",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "msg 받음",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 920,
        "y": 260,
        "wires": [
            [
                "154eb4dff43b5503"
            ]
        ]
    },
    {
        "id": "e5e61a99ef4d949e",
        "type": "switch",
        "z": "3a4cfca6de027178",
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
        "y": 260,
        "wires": [
            [
                "7577faf61c210736"
            ],
            [],
            []
        ]
    },
    {
        "id": "481ad2c559a926ee",
        "type": "template",
        "z": "3a4cfca6de027178",
        "name": "html",
        "field": "payload",
        "fieldType": "msg",
        "format": "handlebars",
        "syntax": "mustache",
        "template": "<html>\n    <head>\n        <meta name='viewport' content='width=device-width, initial-scale=1.0'/>\n        <meta http-equiv='refresh' content='0; url=http://{{payload.publicIPv4}}:1880/display?act=3' method='get'> \n    </head>\n    <body>\n       </body>\n</html",
        "output": "str",
        "x": 1517,
        "y": 60,
        "wires": [
            [
                "e33ffe532282079e"
            ]
        ]
    },
    {
        "id": "e33ffe532282079e",
        "type": "http response",
        "z": "3a4cfca6de027178",
        "name": "Go Main Page",
        "statusCode": "",
        "headers": {},
        "x": 1669,
        "y": 60,
        "wires": []
    },
    {
        "id": "8877bf44bbdfe8ae",
        "type": "link in",
        "z": "3a4cfca6de027178",
        "name": "Main",
        "links": [
            "6457ed09.277674",
            "1f045d85.6a9eb2",
            "250eafef.fdf1f",
            "68a5f2a5.f5958c",
            "d1b3b938.5fd1b8",
            "425e599b.c1d808",
            "174175dc.9ef7ea",
            "fcf6f5f1ce993b3b",
            "154eb4dff43b5503"
        ],
        "x": 1215,
        "y": 60,
        "wires": [
            [
                "913e7d00d70ef881"
            ]
        ]
    },
    {
        "id": "fcf6f5f1ce993b3b",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "8877bf44bbdfe8ae"
        ],
        "x": 1075,
        "y": 70,
        "wires": []
    },
    {
        "id": "cf58876de1bd95da",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "Act",
        "info": "1: \n",
        "x": 110,
        "y": 320,
        "wires": []
    },
    {
        "id": "80c0d40ac4e98d45",
        "type": "http in",
        "z": "3a4cfca6de027178",
        "name": "",
        "url": "/act",
        "method": "post",
        "upload": false,
        "swaggerDoc": "",
        "x": 280,
        "y": 320,
        "wires": [
            []
        ]
    },
    {
        "id": "bf192058a4ac5513",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "",
        "func": "global.set(\"serverIP\",msg.payload.publicIPv4);\n//msg.payload=msg.payload.publicIPv4;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 440,
        "y": 70,
        "wires": [
            [
                "8e3a644bff4e519c"
            ]
        ]
    },
    {
        "id": "d3490c97a28061bf",
        "type": "websocket out",
        "z": "3a4cfca6de027178",
        "name": "",
        "server": "10600f0d605ab122",
        "client": "",
        "x": 1760,
        "y": 420,
        "wires": []
    },
    {
        "id": "546a6699b53f2337",
        "type": "websocket in",
        "z": "3a4cfca6de027178",
        "name": "",
        "server": "10600f0d605ab122",
        "client": "",
        "x": 240,
        "y": 540,
        "wires": [
            [
                "edb6d3d60225daf8",
                "02bbbb77a662bc45"
            ]
        ]
    },
    {
        "id": "aa6c69dbe9d1ee78",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "출력버튼 웹소켙",
        "info": "",
        "x": 240,
        "y": 500,
        "wires": []
    },
    {
        "id": "88547462510aa2be",
        "type": "template",
        "z": "3a4cfca6de027178",
        "name": "Main html",
        "field": "payload",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "<!DOCTYPE HTML>\n<html>\n<head>\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n    <style> {{{payload.style}}} </style>\n    <script> {{{payload.script}}} </script>\n    <script type=\"text/javascript\"> {{{payload.scriptSocket}}} </script>\n</head>\n<body onload=\"wsConnect();\" onunload=\"ws.disconnect();\">\n    {{{payload.menu}}} \n    {{{payload.list}}}\n</body>\n</html>\n\n",
        "x": 1620,
        "y": 180,
        "wires": [
            [
                "8073941b663b6bc7"
            ]
        ]
    },
    {
        "id": "19cb87ccd859975b",
        "type": "template",
        "z": "3a4cfca6de027178",
        "name": "Web Socket scriptSocket",
        "field": "payload.scriptSocket",
        "fieldType": "msg",
        "format": "html",
        "syntax": "mustache",
        "template": "        var ws;\n        var wsUri = \"ws:\";\n        var loc = window.location;\n        console.log(loc);\n        if (loc.protocol === \"https:\") { wsUri = \"wss:\"; }\n        // This needs to point to the web socket in the Node-RED flow\n        // ... in this case it's ws/simple\n        wsUri += \"//\" + loc.host + loc.pathname.replace(\"display\",\"ws/simple\");\n\n        function wsConnect() {\n            console.log(\"connect\",wsUri);\n            ws = new WebSocket(wsUri);\n            //var line = \"\";    // either uncomment this for a building list of messages\n            ws.onmessage = function(msg) {\n                var line = \"\";  // or uncomment this to overwrite the existing message\n                // parse the incoming message as a JSON object\n                var data = JSON.parse(msg.data);\n                //var data = msg.data;\n                console.log(data);\n                if(data.type==2) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"EC:\"+data.ec;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"PH:\"+data.ph;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                if(data.type==3) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"CO2:\"+data.co2;\n                }\n                if(data.type==4) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.mo+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.tem+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                if(data.type==5) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                }\n                else if(data.type==6) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[6]=='1') \n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in6').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[7]=='1') \n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in7').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:3,^value^:1}\\\");'></button>\";\n                    if(data.out[4]=='1')\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out4').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:4,^value^:1}\\\");'></button>\";\n                    if(data.out[5]=='1')\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out5').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:6,^outNo^:5,^value^:1}\\\");'></button>\";\n                }\n                else if(data.type==7) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                }\n                else if(data.type==8) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"습도:\"+data.humi;\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp;\n                    document.getElementById(data.mac+'-in2').innerHTML = \"대기압:\"+data.pres;\n                }\n                else if(data.type==9) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"함수율:\"+data.humi+\"%\";\n                    document.getElementById(data.mac+'-in1').innerHTML = \"온도:\"+data.temp+\"도\";\n                    document.getElementById(data.mac+'-in2').innerHTML = \"EC:\"+data.ec;\n                }\n                else if(data.type==10) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"사운드:\"+data.sound;\n                }\n                else if(data.type==11) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"충격발생:\"+data.knock;\n                }\n                else if(data.type==12) {\n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n\n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:12,^outNo^:3,^value^:1}\\\");'></button>\";\n                }\n                if(data.type==13) {\n                    if(data.state==0) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==1) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n                    else if(data.state==2) {\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:0}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:1}\\\");'></button>\";\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:13,^state^:2}\\\");'></button>\";\n                    }\n\n                 }\n                 else if(data.type==14) {\n                    \n                    if(data.out[0]=='1')\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out0').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:0,^value^:1}\\\");'></button>\";\n                    if(data.out[1]=='1')\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out1').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:1,^value^:1}\\\");'></button>\";\n                    if(data.out[2]=='1')\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out2').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:2,^value^:1}\\\");'></button>\";\n                    if(data.out[3]=='1')\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-on' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:0}\\\");'></button>\";\n                    else\n                        document.getElementById(data.mac+'-out3').innerHTML = \"<button class='button button-off' onclick='doit(\\\"{^mac^:^\"+data.mac+\"^,^type^:14,^outNo^:3,^value^:1}\\\");'></button>\";\n                    \n                    if(data.in[0]=='1') \n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in0').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[1]=='1') \n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in1').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[2]=='1') \n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in2').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[3]=='1') \n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in3').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[4]=='1') \n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in4').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    if(data.in[5]=='1') \n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledon' ></button>\";\n                    else\n                        document.getElementById(data.mac+'-in5').innerHTML = \"<button class='button button-ledoff' ></button>\";\n                    \n                }\n                else if(data.type==15) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"연기농도:\"+data.gas;\n                }\n                else if(data.type==16) {\n                    document.getElementById(data.mac+'-in0').innerHTML = \"메탄농도:\"+data.gas;\n                }\n            }\n            ws.onopen = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"connected\";\n                //ws.send(\"Open for data\");\n                console.log(\"connected\");\n            }\n            ws.onclose = function() {\n                // update the status div with the connection status\n                //document.getElementById('status').innerHTML = \"not connected\";\n                // in case of lost connection tries to reconnect every 3 secs\n                setTimeout(wsConnect,3000);\n            }\n        }\n        \n        function doit(m) {\n            if (ws) { ws.send(m); }\n        }\n",
        "x": 1410,
        "y": 180,
        "wires": [
            [
                "88547462510aa2be"
            ]
        ]
    },
    {
        "id": "edb6d3d60225daf8",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "",
        "func": "var msg = {payload:msg.payload.replace(/['^']/g, \"\\\"\")};\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 420,
        "y": 540,
        "wires": [
            [
                "938ed7c12c99b3bd",
                "fa26cbe544e6dcf5"
            ]
        ]
    },
    {
        "id": "938ed7c12c99b3bd",
        "type": "json",
        "z": "3a4cfca6de027178",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 570,
        "y": 540,
        "wires": [
            [
                "343fceed549dd1b8"
            ]
        ]
    },
    {
        "id": "343fceed549dd1b8",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOneUpdate",
        "func": "global.set(\"msg_in\",msg);\nvar time = new Date().toLocaleString('ko-KR', { timeZone: 'Asia/Seoul' });\nvar newMsg = {};\n\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOneAndUpdate';\n\nif(msg.payload.type==6 ||msg.payload.type==12 || msg.payload.type==14) {\n    if(msg.payload.outNo == 0)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.0':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 1)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.1':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 2)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.2':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 3)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.3':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 4)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.4':msg.payload.value, 'time':time }} ];\n    else if(msg.payload.outNo == 5)\n        newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'out.5':msg.payload.value, 'time':time }} ];\n}\nelse if(msg.payload.type==13) \n    newMsg.payload = [{'mac': msg.payload.mac,'type':msg.payload.type}, {$set:{ 'state':msg.payload.state, 'time':time }} ];\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 740,
        "y": 540,
        "wires": [
            [
                "4f21a03c18b4f795"
            ]
        ]
    },
    {
        "id": "a0cdb6362a04df69",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "입력버튼 웹소켙",
        "info": "",
        "x": 240,
        "y": 400,
        "wires": []
    },
    {
        "id": "fa26cbe544e6dcf5",
        "type": "mqtt out",
        "z": "3a4cfca6de027178",
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
        "x": 590,
        "y": 600,
        "wires": []
    },
    {
        "id": "85b500031bd888e6",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "90ab830c6b7883fb"
        ],
        "x": 1035,
        "y": 427,
        "wires": []
    },
    {
        "id": "f27515fcbe3ea1f5",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "90ab830c6b7883fb"
        ],
        "x": 1075,
        "y": 480,
        "wires": []
    },
    {
        "id": "4f21a03c18b4f795",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "90ab830c6b7883fb"
        ],
        "x": 875,
        "y": 540,
        "wires": []
    },
    {
        "id": "90ab830c6b7883fb",
        "type": "link in",
        "z": "3a4cfca6de027178",
        "name": "",
        "links": [
            "85b500031bd888e6",
            "f27515fcbe3ea1f5",
            "4f21a03c18b4f795"
        ],
        "x": 1175,
        "y": 420,
        "wires": [
            [
                "3d28c96bce7275c1"
            ]
        ]
    },
    {
        "id": "3c302cc5c350abdf",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOne",
        "func": "var msg=global.get(\"msg_in\")||\"\";\nvar newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1420,
        "y": 420,
        "wires": [
            [
                "8115fe2b71b35703"
            ]
        ]
    },
    {
        "id": "3959295dcd23ec26",
        "type": "subflow:e4dab0afad2daadd",
        "z": "3a4cfca6de027178",
        "name": "Iterate",
        "env": [],
        "x": 1550,
        "y": 320,
        "wires": [
            [
                "6c48e9421e45adeb",
                "17ea8e41a274fc37"
            ],
            []
        ]
    },
    {
        "id": "17ea8e41a274fc37",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "",
        "func": "\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1540,
        "y": 260,
        "wires": [
            [
                "3959295dcd23ec26"
            ]
        ]
    },
    {
        "id": "6c48e9421e45adeb",
        "type": "function",
        "z": "3a4cfca6de027178",
        "name": "findOne",
        "func": "var newMsg = {};\nnewMsg.collection = 'localRecord';\nnewMsg.operation  = 'findOne';\nnewMsg.payload    = { 'mac' : msg.payload.mac};\nnewMsg.projection = { 'mac' : 1 , '_id' : 0 };\nreturn newMsg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 1720,
        "y": 320,
        "wires": [
            [
                "8115fe2b71b35703"
            ]
        ]
    },
    {
        "id": "87402721d6aaa4ee",
        "type": "delay",
        "z": "3a4cfca6de027178",
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
        "x": 1400,
        "y": 320,
        "wires": [
            [
                "3959295dcd23ec26"
            ]
        ]
    },
    {
        "id": "2e69794b3f9c90dd",
        "type": "comment",
        "z": "3a4cfca6de027178",
        "name": "웹페이지 초기화",
        "info": "",
        "x": 1360,
        "y": 260,
        "wires": []
    },
    {
        "id": "1a1ee9ceb8d3fbff",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 660,
        "y": 440,
        "wires": [
            [
                "745b28d778cd1a88"
            ]
        ]
    },
    {
        "id": "3d28c96bce7275c1",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "ff55d734.a9cd28",
        "name": "등록",
        "collection": "",
        "operation": "",
        "x": 1270,
        "y": 420,
        "wires": [
            [
                "3c302cc5c350abdf"
            ]
        ]
    },
    {
        "id": "414fb6a539d87582",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "60fc73ed260834ef",
        "name": "local",
        "collection": "",
        "operation": "",
        "x": 890,
        "y": 200,
        "wires": [
            [
                "d82b881eadd280ca"
            ]
        ]
    },
    {
        "id": "2ccaa283683214f4",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "60fc73ed260834ef",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 902,
        "y": 130,
        "wires": [
            [
                "ab5262a78b759bda"
            ]
        ]
    },
    {
        "id": "55aca276c80b6830",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "60fc73ed260834ef",
        "name": "등록 name",
        "collection": "",
        "operation": "",
        "x": 770,
        "y": 260,
        "wires": [
            [
                "28f037425e56a0db",
                "0a6523c744fa4865"
            ]
        ]
    },
    {
        "id": "8115fe2b71b35703",
        "type": "mongodb2 in",
        "z": "3a4cfca6de027178",
        "service": "_ext_",
        "configNode": "60fc73ed260834ef",
        "name": "기기 검색",
        "collection": "",
        "operation": "",
        "x": 1580,
        "y": 420,
        "wires": [
            [
                "d3490c97a28061bf",
                "0a7ebbb07c3e51e5",
                "9bd2188240ddcb15"
            ]
        ]
    },
    {
        "id": "913e7d00d70ef881",
        "type": "ip",
        "z": "3a4cfca6de027178",
        "name": "node-red-contrib-ip",
        "https": false,
        "timeout": "5000",
        "internalIPv4": true,
        "internalIPv6": true,
        "publicIPv4": true,
        "publicIPv6": false,
        "x": 1350,
        "y": 60,
        "wires": [
            [
                "481ad2c559a926ee"
            ]
        ]
    },
    {
        "id": "71680d1f9b1264ca",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "mode": "link",
        "links": [
            "2f594e6cd0478e06",
            "5ce7a8856f235d4f"
        ],
        "x": 495,
        "y": 380,
        "wires": []
    },
    {
        "id": "0a6523c744fa4865",
        "type": "debug",
        "z": "3a4cfca6de027178",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "false",
        "statusVal": "",
        "statusType": "auto",
        "x": 960,
        "y": 320,
        "wires": []
    },
    {
        "id": "0a7ebbb07c3e51e5",
        "type": "debug",
        "z": "3a4cfca6de027178",
        "name": "",
        "active": false,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 1790,
        "y": 520,
        "wires": []
    },
    {
        "id": "9bd2188240ddcb15",
        "type": "link out",
        "z": "3a4cfca6de027178",
        "name": "",
        "mode": "link",
        "links": [],
        "x": 1735,
        "y": 460,
        "wires": []
    },
    {
        "id": "c5e64b85cc720684",
        "type": "debug",
        "z": "3a4cfca6de027178",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "true",
        "targetType": "full",
        "statusVal": "",
        "statusType": "auto",
        "x": 1220,
        "y": 260,
        "wires": []
    },
    {
        "id": "02bbbb77a662bc45",
        "type": "debug",
        "z": "3a4cfca6de027178",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "true",
        "targetType": "full",
        "statusVal": "",
        "statusType": "auto",
        "x": 340,
        "y": 620,
        "wires": []
    },
    {
        "id": "f15d66b3.c7edf8",
        "type": "mqtt-broker",
        "name": "",
        "broker": "a3aq8t057uz8il-ats.iot.us-east-2.amazonaws.com",
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
        "id": "10600f0d605ab122",
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
        "id": "60fc73ed260834ef",
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
        "certname": "f63dd04081758207017b3f5a6fe5d533d524c6a90792adf2780779cb64a626d8-certificate.pem.crt",
        "keyname": "f63dd04081758207017b3f5a6fe5d533d524c6a90792adf2780779cb64a626d8-private.pem.key",
        "caname": "AmazonRootCA1.pem",
        "servername": "",
        "verifyservercert": true,
        "alpnprotocol": ""
    }
]
```   

### node-red ui 기능 확인
1. 연기농도가 300이하 일 때
![연기200](https://user-images.githubusercontent.com/37902752/152289543-1fe19db5-88ee-45f6-a7f1-79b240205872.JPG)

2. 연기농도가 300이상 일 때
![연기200이상](https://user-images.githubusercontent.com/37902752/152289508-ca47e0b3-b960-405e-b447-a432aa7f852a.JPG)
