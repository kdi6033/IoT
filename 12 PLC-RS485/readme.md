## RS485 릴레이보드 사용 Node-red로 사용
### 1. 프로토콜 확인 

기기와 통신하기 위해서는 다음 두가지믈 먼저한다.    
- 통신속도를 9600으로 하고 기기 번호를 읽어온다. 에러가 나면 통신속도를 변화 시키며 이를 찿아낸다.
- 원하는 기기번호로 세팅한다.
- 두가지를 설정한 후 제어 프로그램을 하면 된다.

처음에는 기기번호=0xff, 통신속도=9600
|기능|보냄|응답|설명|
|----|----|-------|-------|
|주소읽기|00 03 00 00 00 01 85 DB|00 03 02 00 FF C5 C4|응답의 5번째 FF가 plc 주소|
|주소를 255로 쓰기|00 10 00 00 00 01 02 00 FF EB 80|00 10 00 00 00 01 02 00 FF EB 80|9번째 FF가 plc 주소이다 통신속도 9600, 첫번째 문자 00 기기번호 상관 없이 00으로 한다.|
|주소를 1로 쓰기|00 10 00 00 00 01 02 00 01 EB 80|00 10 00 00 00 01 02 00 01 6A 00|9번째 01이 plc 주소이다 통신속도 9600, 첫번째 문자 00 기기번호 상관 없이 00으로 한다.|
|입력상태읽기|FF 02 00 00 00 08 6C 12|FF 02 01 03 51 AO|응답의 4번째 03이면 0V 0V 24V 24V 입력|
|출력상태읽기|FF 01 00 00 00 08 28 12|FF 01 01 03 A1 AO|응답의 4번째 03이면 0V 0V 24V 24V 출력|
|0번릴레이on|FF 05 00 00 FF 00 99 E4|FF 05 00 00 FF 00 99 E4|0번릴레이 24V 출력|
|0번릴레이off|FF 05 00 00 00 00 D8 14|FF 05 00 00 00 00 D8 14|0번릴레이 0V 출력|
|1번릴레이on|FF 05 00 01 FF 00 - -|생략|0번릴레이 24V 출력|
|1번릴레이off|FF 05 00 01 00 00 - -|생략|0번릴레이 0V 출력|
|모든 릴레이on|FF 0F 00 00 00 08 01 FF 30 1D|FF 0F 00 00 00 08 41 D3|모든 릴레이 24V 출력|
|모든 릴레이off|FF 0F 00 00 08 01 00 70 5D|FF 0F 00 00 08 41 D3|모든 릴레이 0V 출력|
|통신속도 4800|FF 10 03 E9 00 01 02 00 02 4A 0C|FF 10 03 E9 00 01 C5 A7|9번째 문자에 따른 통신독도 0x02=4800 0x03=9600 0x04=19200|    

예제 (와이파이-RS485 보드를 이용한 제어)
|예제번호|설명|
|----|----|
|PlcRs485-01|(function code 05) 한개 릴레이 출력|
|PlcRs485-02|(function code 0f) 모든 릴레이 출력|
|PlcRs485-03|(function code 02) 입력포트 읽기|     

Wifi-Rs485 보드구매처: https://smartstore.naver.com/i2r/products/4584482642     
PLC 구매처 : https://ko.aliexpress.com/item/1005002356037000.html?spm=a2g0s.9042311.0.0.27424c4dpXx4Xh     
            https://ko.aliexpress.com/item/1005001665868860.html?spm=a2g0o.productlist.0.0.17e37ef4BEAYsf&algo_pvid=96484fdf-20e6-45d2-9cb7-54e51467da72&algo_exp_id=96484fdf-20e6-45d2-9cb7-54e51467da72-

* NODE-RED FLOW  
```
[
    {
        "id": "437d4cb4ae8cb695",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "7 주소읽기",
        "func": "var buf = Buffer.alloc(8);\n//장치 주소를 읽기: 00 03 00 00 00 01 85 DB \nbuf[0]=0x00;\nbuf[1]=0x03;\nbuf[2]=0x00;\nbuf[3]=0x00;\nbuf[4]=0x00;\nbuf[5]=0x01;\n//buf[6]=0x85;\n//buf[7]=0xdb;\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 290,
        "y": 443,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "b2a303f2d14bb0a8",
        "type": "serial out",
        "z": "d13ebbb690b59258",
        "name": "COM8",
        "serial": "63957752.49a888",
        "x": 655,
        "y": 140,
        "wires": [],
        "l": false
    },
    {
        "id": "db7d2da467161f0e",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 140,
        "y": 443,
        "wires": [
            [
                "437d4cb4ae8cb695"
            ]
        ]
    },
    {
        "id": "3d3d5a21a3f56beb",
        "type": "serial in",
        "z": "d13ebbb690b59258",
        "name": "",
        "serial": "63957752.49a888",
        "x": 550,
        "y": 220,
        "wires": [
            [
                "2c4f68c6d7360b43"
            ]
        ]
    },
    {
        "id": "2c4f68c6d7360b43",
        "type": "debug",
        "z": "d13ebbb690b59258",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 710,
        "y": 220,
        "wires": []
    },
    {
        "id": "2d08ab72562d5b26",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "modbus cal",
        "func": "var auchCRCHi = [\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40\n      ];\n\n      /* Table of CRC values for low-order byte */\n    var auchCRCLo = [\n        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,\n        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,\n        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,\n        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,\n        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,\n        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,\n        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,\n        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,\n        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,\n        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,\n        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,\n        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,\n        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,\n        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,\n        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,\n        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,\n        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,\n        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,\n        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,\n        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,\n        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,\n        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,\n        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,\n        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,\n        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,\n        0x43, 0x83, 0x41, 0x81, 0x80, 0x40\n      ];\n\n//var data=Buffer.from([1,15,0,0,0,10,2,255,0,0,0]);\nvar data=Buffer.from(msg.payload);\nvar nTemp;\nvar wCRCWord = 0xFFFF;\n//data=msg.payload;\nvar len=data.length-2;\nvar crc = [0xFF, 0xFF];\n    var nextByte = 0;\n    var uIndex; /* will index into CRC lookup*/ /* table */\n    /* pass through message buffer */\n    for (var i = 0; i < len && i < data.length; i++) {\n      nextByte = 0xFF & (data[i]);\n      uIndex = crc[0] ^ nextByte; //*puchMsg++; /* calculate the CRC */\n      crc[0] = crc[1] ^ auchCRCHi[uIndex];\n      crc[1] = auchCRCLo[uIndex];\n    }\n\ndata[len]=crc[0];\ndata[len+1]=crc[1];\nmsg.payload= data;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 550,
        "y": 140,
        "wires": [
            [
                "b2a303f2d14bb0a8"
            ]
        ]
    },
    {
        "id": "45b8424324f9309b",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "3 모든 릴레이 on",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(10);\nbuf=[0xff,0x0f,0x00,0x00,0x00,0x08,0x00,0xff,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 310,
        "y": 220,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "6f9ba6a83c6cbfe9",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 220,
        "wires": [
            [
                "45b8424324f9309b"
            ]
        ]
    },
    {
        "id": "f3488d0dfe8ef2ed",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "4 모든 릴레이 off",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(10);\nbuf=[0xff,0x0f,0x00,0x00,0x00,0x08,0x01,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 310,
        "y": 260,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "148c9d9880ad94b8",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 260,
        "wires": [
            [
                "f3488d0dfe8ef2ed"
            ]
        ]
    },
    {
        "id": "7c90d6f3ab039b80",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "4번 on",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x03,0xff,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 540,
        "wires": [
            []
        ]
    },
    {
        "id": "0a2df5f108293996",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 540,
        "wires": [
            [
                "7c90d6f3ab039b80"
            ]
        ]
    },
    {
        "id": "8e9068ed01757b2c",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "14 일정시간 on 후에 off",
        "func": "//14. 1 릴레이를 켜십시오 (플래시 닫기 모드 2 초)\n//Relay1 on: FF 10 00 03 00 02 04 00 04 00 14 C5 9F\nvar buf = Buffer.alloc(13);\nbuf=[0xff,0x10,0x00,0x03,0x00,0x02,0x04,0x00,0x04,0x00,0x14,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 330,
        "y": 543,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "2b1e2907dadb4e60",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 543,
        "wires": [
            [
                "8e9068ed01757b2c"
            ]
        ]
    },
    {
        "id": "5ec9310e4e986150",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "4번 off",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x03,0x00,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 580,
        "wires": [
            []
        ]
    },
    {
        "id": "2fdd66409c6b97d6",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 580,
        "wires": [
            [
                "5ec9310e4e986150"
            ]
        ]
    },
    {
        "id": "e1e9b7e9c27bac05",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "3번 on",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x02,0xff,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 460,
        "wires": [
            []
        ]
    },
    {
        "id": "e3b2dc21a4a7f196",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 460,
        "wires": [
            [
                "e1e9b7e9c27bac05"
            ]
        ]
    },
    {
        "id": "84d9d02ae256262b",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "3번 off",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x02,0x00,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 500,
        "wires": [
            []
        ]
    },
    {
        "id": "0e655adffb12a048",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 500,
        "wires": [
            [
                "84d9d02ae256262b"
            ]
        ]
    },
    {
        "id": "5425cb131685afdd",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "2번 on",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x01,0xff,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 380,
        "wires": [
            []
        ]
    },
    {
        "id": "d6f921e711ea70b8",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 380,
        "wires": [
            [
                "5425cb131685afdd"
            ]
        ]
    },
    {
        "id": "b819adbd173bdbf8",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "2번 off",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x01,0x00,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 790,
        "y": 420,
        "wires": [
            []
        ]
    },
    {
        "id": "c81c039a129a79bb",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 640,
        "y": 420,
        "wires": [
            [
                "b819adbd173bdbf8"
            ]
        ]
    },
    {
        "id": "6e73f6b92e4ba0e1",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "1 1번 릴레이 on",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x00,0xff,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 320,
        "y": 140,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "4a9e673fc65c2a78",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 140,
        "wires": [
            [
                "6e73f6b92e4ba0e1"
            ]
        ]
    },
    {
        "id": "aa436a487cc139c2",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "2 1번 릴레이 off",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x00,0x00,0x00,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 320,
        "y": 180,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "3b5818068388bc43",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 180,
        "wires": [
            [
                "aa436a487cc139c2"
            ]
        ]
    },
    {
        "id": "6ca9ea89c52fb0e7",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "8 릴레이 상태 읽기",
        "func": "//FF 01 00 00 00 08 28 12\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x01,0x00,0x00,0x00,0x08,0x00,0x00];\nbuf[4]=msg.payload;\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 310,
        "y": 483,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "9db2abaaef54d592",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payloadType": "date",
        "x": 140,
        "y": 483,
        "wires": [
            [
                "6ca9ea89c52fb0e7"
            ]
        ]
    },
    {
        "id": "bc018c9e4b803132",
        "type": "comment",
        "z": "d13ebbb690b59258",
        "name": "2,3,4번 on off",
        "info": "buf[3] : 릴레이 번지수\nbuf[4] : 릴레이 상태 0xff(on), 0x00(off)",
        "x": 630,
        "y": 340,
        "wires": []
    },
    {
        "id": "d8ce3e69072d0d27",
        "type": "comment",
        "z": "d13ebbb690b59258",
        "name": "프로토콜 참조 설명",
        "info": "https://ko.aliexpress.com/item/1005002356037000.html?spm=a2g0s.9042311.0.0.27424c4dc8WPuJ\n\n",
        "x": 150,
        "y": 100,
        "wires": []
    },
    {
        "id": "6f9780b11d352d3b",
        "type": "comment",
        "z": "d13ebbb690b59258",
        "name": "5 address(국번) 01로 설정",
        "info": "보내기 : 00 10 00 00 00 01 02 00 _01_ 6A 00\n반환 그대로 : 00 10 00 00 00 01 02 00 _01_ 6A 00\n\n9번째가 주소 정보 : 01\n\n01로 수정하게 될 경우 기존 릴레이 제어 프로토콜에서\nFF를 01로 수정\n예시) 1번릴레이 on\nbuf=[0xff,0x05,0x00,0x00,0xff,0x00,0x00,0x00];\n>> 어드레스 01로 변경\nbuf=[0x01,0x05,0x00,0x00,0xff,0x00,0x00,0x00];",
        "x": 170,
        "y": 300,
        "wires": []
    },
    {
        "id": "3022c2ca1b87f543",
        "type": "comment",
        "z": "d13ebbb690b59258",
        "name": "6 address(국번) 255로 설정",
        "info": "보내기 : 00 10 00 00 00 01 02 00 _FF_ EB 80\n반환 그대로: 00 10 00 00 00 01 02 00 _FF_ EB 80\n\n9번째가 주소 정보 : FF",
        "x": 180,
        "y": 340,
        "wires": []
    },
    {
        "id": "85081ddefb5c487b",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "15 일정시간 off 후에 on",
        "func": "//15, No. 1 릴레이 해제 (플래시 모드 3 초)\n//Relay1 on: FF 10 00 03 00 02 04 00 02 00 1E A5 99\nvar buf = Buffer.alloc(13);\nbuf=[0xff,0x10,0x00,0x03,0x00,0x02,0x04,0x00,0x02,0x00,0x1E,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 330,
        "y": 583,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "77ac0271cba3cc55",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 140,
        "y": 583,
        "wires": [
            [
                "85081ddefb5c487b"
            ]
        ]
    },
    {
        "id": "a0e3d4b18f24c6b1",
        "type": "function",
        "z": "d13ebbb690b59258",
        "name": "6 주소 0xff로 설정",
        "func": "var buf = Buffer.alloc(11);\n// 주소를 255로 설정: 00 10 00 00 00 01 02 00 FF EB 80\n// 통신속도 9600 \n// 첫번째 문자 00 기기번호 상관 없이 00으로 한다.\nbuf[0]=0x00;\nbuf[1]=0x10;\nbuf[2]=0x00;\nbuf[3]=0x00;\nbuf[4]=0x00;\nbuf[5]=0x01;\nbuf[6]=0x02;\nbuf[7]=0x00;\nbuf[8]=0xff;\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 310,
        "y": 380,
        "wires": [
            [
                "2d08ab72562d5b26"
            ]
        ]
    },
    {
        "id": "5f47c772469812f8",
        "type": "inject",
        "z": "d13ebbb690b59258",
        "name": "",
        "props": [
            {
                "p": "payload"
            },
            {
                "p": "topic",
                "vt": "str"
            }
        ],
        "repeat": "",
        "crontab": "",
        "once": false,
        "onceDelay": 0.1,
        "topic": "",
        "payload": "",
        "payloadType": "date",
        "x": 130,
        "y": 380,
        "wires": [
            [
                "a0e3d4b18f24c6b1"
            ]
        ]
    },
    {
        "id": "63957752.49a888",
        "type": "serial-port",
        "serialport": "COM10",
        "serialbaud": "9600",
        "databits": "8",
        "parity": "none",
        "stopbits": "1",
        "waitfor": "",
        "dtr": "none",
        "rts": "none",
        "cts": "none",
        "dsr": "none",
        "newline": "0",
        "bin": "bin",
        "out": "time",
        "addchar": "",
        "responsetimeout": "10000"
    }
]
```

*  flow 사진
 <img src = "https://user-images.githubusercontent.com/37902752/135479568-292109bd-87aa-4cae-b9d2-4ab2c97e9139.PNG" width="550" height="300">

### 2. 프로토콜 UI 제어
* NODE-RED FLOW
```
[{"id":"46c0024bbdec4c5c","type":"tab","label":"프로토콜 UI 제어","disabled":false,"info":""},{"id":"b19342da74974d70","type":"ui_switch","z":"46c0024bbdec4c5c","name":"1","label":"","tooltip":"","group":"dc278f48.9ecfa","order":1,"width":"0","height":"0","passthru":true,"decouple":"false","topic":"a","topicType":"str","style":"","onvalue":"255","onvalueType":"num","onicon":"","oncolor":"","offvalue":"0","offvalueType":"num","officon":"","offcolor":"","animate":true,"x":150,"y":120,"wires":[["b0f376526c19ea84","6764c5b252172ea2"]]},{"id":"3ca133c343115fe6","type":"ui_switch","z":"46c0024bbdec4c5c","name":"2","label":"","tooltip":"","group":"dc278f48.9ecfa","order":2,"width":"0","height":"0","passthru":true,"decouple":"false","topic":"a","topicType":"str","style":"","onvalue":"255","onvalueType":"num","onicon":"","oncolor":"","offvalue":"0","offvalueType":"num","officon":"","offcolor":"","animate":true,"x":150,"y":160,"wires":[["18ef3cfebdb402f0","6764c5b252172ea2"]]},{"id":"bfe5d1701a8e1886","type":"ui_switch","z":"46c0024bbdec4c5c","name":"3","label":"","tooltip":"","group":"dc278f48.9ecfa","order":3,"width":"0","height":"0","passthru":true,"decouple":"false","topic":"a","topicType":"str","style":"","onvalue":"255","onvalueType":"num","onicon":"","oncolor":"","offvalue":"0","offvalueType":"num","officon":"","offcolor":"","animate":true,"x":150,"y":200,"wires":[["76e6642cbf3136a9","6764c5b252172ea2"]]},{"id":"37a1eb94d38c61ab","type":"ui_switch","z":"46c0024bbdec4c5c","name":"4","label":"","tooltip":"","group":"dc278f48.9ecfa","order":4,"width":"0","height":"0","passthru":true,"decouple":"false","topic":"a","topicType":"str","style":"","onvalue":"255","onvalueType":"num","onicon":"","oncolor":"","offvalue":"0","offvalueType":"num","officon":"","offcolor":"","animate":true,"x":150,"y":240,"wires":[["0bb66f79939cf267","6764c5b252172ea2"]]},{"id":"0d23488cf3d9d5e4","type":"serial out","z":"46c0024bbdec4c5c","name":"COM8","serial":"63957752.49a888","x":655,"y":180,"wires":[],"l":false},{"id":"8faaf48a83ac31a1","type":"function","z":"46c0024bbdec4c5c","name":"modbus cal","func":"var auchCRCHi = [\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40\n      ];\n\n      /* Table of CRC values for low-order byte */\n    var auchCRCLo = [\n        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,\n        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,\n        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,\n        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,\n        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,\n        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,\n        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,\n        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,\n        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,\n        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,\n        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,\n        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,\n        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,\n        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,\n        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,\n        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,\n        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,\n        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,\n        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,\n        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,\n        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,\n        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,\n        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,\n        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,\n        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,\n        0x43, 0x83, 0x41, 0x81, 0x80, 0x40\n      ];\n\n//var data=Buffer.from([1,15,0,0,0,10,2,255,0,0,0]);\nvar data=Buffer.from(msg.payload);\nvar nTemp;\nvar wCRCWord = 0xFFFF;\n//data=msg.payload;\nvar len=data.length-2;\nvar crc = [0xFF, 0xFF];\n    var nextByte = 0;\n    var uIndex; /* will index into CRC lookup*/ /* table */\n    /* pass through message buffer */\n    for (var i = 0; i < len && i < data.length; i++) {\n      nextByte = 0xFF & (data[i]);\n      uIndex = crc[0] ^ nextByte; //*puchMsg++; /* calculate the CRC */\n      crc[0] = crc[1] ^ auchCRCHi[uIndex];\n      crc[1] = auchCRCLo[uIndex];\n    }\n\ndata[len]=crc[0];\ndata[len+1]=crc[1];\nmsg.payload= data;\nreturn msg;","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":550,"y":180,"wires":[["0d23488cf3d9d5e4"]]},{"id":"b0f376526c19ea84","type":"function","z":"46c0024bbdec4c5c","name":"쓰기","func":"//FF 05 00 00 FF 00 99 E4\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x00,0xff,0x00,0x00,0x00];\nbuf[4]=msg.payload; //실린더에서 보낸 payload값에 따라 온오프\n\nmsg.payload=buf;\nreturn msg;","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":330,"y":120,"wires":[["8faaf48a83ac31a1"]]},{"id":"18ef3cfebdb402f0","type":"function","z":"46c0024bbdec4c5c","name":"쓰기","func":"//FF 05 00 00 FF 00 99 E4\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x01,0xff,0x00,0x00,0x00];\nbuf[4]=msg.payload;\n\nmsg.payload=buf;\nreturn msg;","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":330,"y":160,"wires":[["8faaf48a83ac31a1"]]},{"id":"76e6642cbf3136a9","type":"function","z":"46c0024bbdec4c5c","name":"쓰기","func":"//FF 05 00 00 FF 00 99 E4\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x02,0xff,0x00,0x00,0x00];\nbuf[4]=msg.payload;\n\nmsg.payload=buf;\nreturn msg;","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":330,"y":200,"wires":[["8faaf48a83ac31a1"]]},{"id":"0bb66f79939cf267","type":"function","z":"46c0024bbdec4c5c","name":"쓰기","func":"//FF 05 00 00 FF 00 99 E4\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x05,0x00,0x03,0xff,0x00,0x00,0x00];\nbuf[4]=msg.payload;\n\nmsg.payload=buf;\nreturn msg;","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":330,"y":240,"wires":[["8faaf48a83ac31a1"]]},{"id":"6764c5b252172ea2","type":"function","z":"46c0024bbdec4c5c","name":"8 릴레이 상태 읽기","func":"if(msg.topic==\"a\"){\n//FF 01 00 00 00 08 28 12\nvar buf = Buffer.alloc(8);\nbuf=[0xff,0x01,0x00,0x00,0x00,0x08,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;\n}","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":370,"y":80,"wires":[["d451650f0e91ebb9"]]},{"id":"d451650f0e91ebb9","type":"delay","z":"46c0024bbdec4c5c","name":"1초 딜레이","pauseType":"delay","timeout":"1","timeoutUnits":"seconds","rate":"1","nbRateUnits":"1","rateUnits":"second","randomFirst":"1","randomLast":"5","randomUnits":"seconds","drop":false,"allowrate":false,"x":550,"y":80,"wires":[["8faaf48a83ac31a1"]]},{"id":"892c3469289c7d7e","type":"serial in","z":"46c0024bbdec4c5c","name":"","serial":"63957752.49a888","x":150,"y":320,"wires":[["2f261be7f0e97efc","02f4e426c7702013"]]},{"id":"9a4af32bd17e4141","type":"ui_led","z":"46c0024bbdec4c5c","order":1,"group":"ccfaa0417e4da0be","width":"0","height":"0","label":"1","labelPlacement":"left","labelAlignment":"center","colorForValue":[{"color":"#b8b8b8","value":"0","valueType":"num"},{"color":"#ff0000","value":"1","valueType":"num"}],"allowColorForValueInMessage":false,"shape":"circle","showGlow":true,"name":"01","x":530,"y":360,"wires":[]},{"id":"2f261be7f0e97efc","type":"function","z":"46c0024bbdec4c5c","name":"릴레이 상태 표시","func":"if(msg.payload[1]==0x01){\nvar ori=msg.payload[3];\nmsg.payload = ori\nreturn msg;\n}","outputs":1,"noerr":0,"initialize":"","finalize":"","libs":[],"x":330,"y":320,"wires":[["8e489c11eb4f77aa"]]},{"id":"8e489c11eb4f77aa","type":"ui_text","z":"46c0024bbdec4c5c","group":"ccfaa0417e4da0be","order":5,"width":0,"height":0,"name":"","label":"16진수","format":"{{msg.payload}}","layout":"row-spread","x":540,"y":320,"wires":[]},{"id":"48c7adb3894ce96f","type":"ui_led","z":"46c0024bbdec4c5c","order":4,"group":"ccfaa0417e4da0be","width":0,"height":0,"label":"4","labelPlacement":"left","labelAlignment":"center","colorForValue":[{"color":"#b8b8b8","value":"0","valueType":"num"},{"color":"#ff0000","value":"1","valueType":"num"}],"allowColorForValueInMessage":false,"shape":"circle","showGlow":true,"name":"04","x":530,"y":480,"wires":[]},{"id":"18c3cd22e61bc564","type":"ui_led","z":"46c0024bbdec4c5c","order":2,"group":"ccfaa0417e4da0be","width":0,"height":0,"label":"2","labelPlacement":"left","labelAlignment":"center","colorForValue":[{"color":"#b8b8b8","value":"0","valueType":"num"},{"color":"#ff0000","value":"1","valueType":"num"}],"allowColorForValueInMessage":false,"shape":"circle","showGlow":true,"name":"02","x":530,"y":400,"wires":[]},{"id":"e4311e24e4cda8d8","type":"ui_led","z":"46c0024bbdec4c5c","order":3,"group":"ccfaa0417e4da0be","width":0,"height":0,"label":"3","labelPlacement":"left","labelAlignment":"center","colorForValue":[{"color":"#b8b8b8","value":"0","valueType":"num"},{"color":"#ff0000","value":"1","valueType":"num"}],"allowColorForValueInMessage":false,"shape":"circle","showGlow":true,"name":"03","x":530,"y":440,"wires":[]},{"id":"02f4e426c7702013","type":"function","z":"46c0024bbdec4c5c","name":"디코드","func":"var msg1,msg2,msg3,msg4;\n\nif(msg.payload[4]==0xff)\n{\n    if(msg.payload[3]==0x00)\n     msg1 = { payload : msg.payload = 1};\n    if(msg.payload[3]==0x01)\n     msg2 = { payload : msg.payload = 1};\n    if(msg.payload[3]==0x02)\n     msg3 = { payload : msg.payload = 1};\n    if(msg.payload[3]==0x03)\n     msg4 = { payload : msg.payload = 1};\n} else {\n    if(msg.payload[3]==0x00)\n     msg1 = { payload : msg.payload = 0};\n    if(msg.payload[3]==0x01)\n     msg2 = { payload : msg.payload = 0};\n    if(msg.payload[3]==0x02)\n     msg3 = { payload : msg.payload = 0};\n    if(msg.payload[3]==0x03)\n     msg4 = { payload : msg.payload = 0};\n    \n}\nreturn [msg1,msg2,msg3,msg4];","outputs":4,"noerr":0,"initialize":"","finalize":"","libs":[],"x":290,"y":380,"wires":[["9a4af32bd17e4141"],["18c3cd22e61bc564"],["e4311e24e4cda8d8"],["48c7adb3894ce96f"]]},{"id":"dc278f48.9ecfa","type":"ui_group","name":"PLC Out","tab":"ea9935fb.a5cb68","order":2,"disp":true,"width":"3","collapse":false},{"id":"63957752.49a888","type":"serial-port","serialport":"COM8","serialbaud":"9600","databits":"8","parity":"none","stopbits":"1","waitfor":"","dtr":"none","rts":"none","cts":"none","dsr":"none","newline":"100","bin":"bin","out":"time","addchar":"","responsetimeout":"10000"},{"id":"ccfaa0417e4da0be","type":"ui_group","name":"PLC In","tab":"ea9935fb.a5cb68","order":1,"disp":true,"width":"2","collapse":false},{"id":"ea9935fb.a5cb68","type":"ui_tab","name":"출력제어","icon":"dashboard","order":0,"disabled":false,"hidden":false}]
```

* flow 사진
<img src = "https://user-images.githubusercontent.com/37902752/135479573-7e29538c-7d89-43e4-91bf-d15510ba4766.PNG" width="550" height="300">

--------

## 배선도
### 준비물
|이름|사진|
|----|---------------:|
|modbus 4CH 릴레이|<img src = "https://user-images.githubusercontent.com/37902752/135489115-6b66567c-bef4-4f96-aaab-b480730262d1.jpg" width="200" height="150">|
|modbus 2CH 릴레이|<img src = "https://user-images.githubusercontent.com/37902752/137663546-e9fbc48f-8331-4529-a5d8-c1323e582ec9.jpg" width="200" height="150">|
|FTDI 232 |<img src = "https://user-images.githubusercontent.com/37902752/135489157-7df4ba48-7daf-4e98-a7f2-330f4ffde4b9.png" width="200" height="100">|
|usb mini 5pin|<img src = "https://user-images.githubusercontent.com/37902752/135489076-78177efc-cc55-4dd5-95e0-9ecb2f3c8d79.PNG" width="200" height="100">|

[4CH 릴레이 구매링크(7~24V)](https://ko.aliexpress.com/item/1005002356037000.html?spm=a2g0s.9042311.0.0.27424c4dpXx4Xh) 
[2CH 릴레이 구매링크(12V)](https://ko.aliexpress.com/item/1005001665868860.html?spm=a2g0o.productlist.0.0.17e37ef4BEAYsf&algo_pvid=96484fdf-20e6-45d2-9cb7-54e51467da72&algo_exp_id=96484fdf-20e6-45d2-9cb7-54e51467da72-5&pdp_ext_f=%7B%22sku_id%22%3A%2212000017043959065%22%7D)

## 4CH 릴레이보드 배선도

* 방법1. Rs485 통신 (권장)
가장 많이 사용되며 병렬로 연결하여 추가로 사용가능
<img src = "https://user-images.githubusercontent.com/37902752/137487547-d1346db7-8682-4aaf-acf2-eae678f38967.jpg" width="650" height="350">

### TTL레벨의 UART RS232 통신 : 통신거리가 짧으므로 배선의 길이는 최소로 해야 합니다. 
* 방법2. Rs485와 TTL 직렬 포트
<img src = "https://user-images.githubusercontent.com/37902752/137488642-6bd0b301-5d5d-4d5c-8cda-362bd89183f7.png" width="650" height="350">

## 2CH 릴레이보드 배선도

### 485 통신
현장에서 가장많이쓰는 통신으로
4km정도로 통신이 가능하고 병렬로 연결하여 사용하기 편하다는 장점이 있다

<img src = "https://user-images.githubusercontent.com/37902752/137664424-6446f3e4-0ef4-42ed-8dde-59ec76545136.jpg" width="750" height="300">
