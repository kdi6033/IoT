## Leaf Humidity sensor
### 1. 프로토콜 확인 
  

|기능|보냄|응답|보냄 설명|응답 설명|
|----|----|-------|-------|-------|
|스테이션 번호 읽기 명령|00 03 00 01 00 01 CRCloCRChi |00 03 02 00  CRCloCRChi(xx=01-ff)|Device address/ Funtion code/ Start register address/ No. of register/ CRC check |Device address/ Funtion code/ Data length/ Data/ CRC check|
|스테이션 번호 쓰기 명령|00 10 00 01 00 01 02 00xx CRCloCRChi(xx=0x01-0xff)|00 10 00 01 00 01 CRCloCRChi (XX=01-FF)|Device address/ Funtion code/ Start register address/ No. of register/ Data length/ Data(new station number)/CRC check |Device address/ Funtion code/ Start register address/No. of registers / CRC check|
|데이터 읽기(single leaf humidity type) |XX 03 00 00 00 01 CRCloCRChi|XX 03 02 00yy CRCloCRChi|Device address/ Funtion code/ Start register address/ No. of registers/ CRC check |Device address/ Funtion code/ Data length/ Data/ CRC check |
|데이터 읽기(Leaf temperature and humidity type) |XX 03 00 00 00 02 CRCloCRChi|XX 03 04 00yy CRCloCRChi|Device address/ Funtion code/ Start register address/ No. of register/ CRC check |Device address/ Funtion code/ Data length/ Data/ CRC check |

Leaf Humidity sensor ry-g/w 구매링크 : https://en.nong-iot.com/product/35.html

Leaf Humidity sensor ry-g/w 메뉴얼 : https://en.nong-iot.com/assets/uploads/RY-CYM1.pdf

      
*NODE-RED FLOW  
```
[
    {
        "id": "2444ab4bb113d3cc",
        "type": "tab",
        "label": "Leaf humidity sensor 동작확인",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "f2ed6391ec7a4e8a",
        "type": "serial out",
        "z": "2444ab4bb113d3cc",
        "name": "COM8",
        "serial": "63957752.49a888",
        "x": 955,
        "y": 280,
        "wires": [],
        "l": false
    },
    {
        "id": "7f14d76e2de93b40",
        "type": "serial in",
        "z": "2444ab4bb113d3cc",
        "name": "",
        "serial": "63957752.49a888",
        "x": 790,
        "y": 400,
        "wires": [
            [
                "1b6407d73ea9d9c2"
            ]
        ]
    },
    {
        "id": "8e20dd5a8cf9e541",
        "type": "function",
        "z": "2444ab4bb113d3cc",
        "name": "modbus cal",
        "func": "var auchCRCHi = [\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,\n        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,\n        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,\n        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,\n        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40\n      ];\n\n      /* Table of CRC values for low-order byte */\n    var auchCRCLo = [\n        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,\n        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,\n        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,\n        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,\n        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,\n        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,\n        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,\n        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,\n        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,\n        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,\n        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,\n        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,\n        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,\n        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,\n        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,\n        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,\n        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,\n        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,\n        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,\n        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,\n        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,\n        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,\n        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,\n        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,\n        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,\n        0x43, 0x83, 0x41, 0x81, 0x80, 0x40\n      ];\n\n//var data=Buffer.from([1,15,0,0,0,10,2,255,0,0,0]);\nvar data=Buffer.from(msg.payload);\nvar nTemp;\nvar wCRCWord = 0xFFFF;\n//data=msg.payload;\nvar len=data.length-2;\nvar crc = [0xFF, 0xFF];\n    var nextByte = 0;\n    var uIndex; /* will index into CRC lookup*/ /* table */\n    /* pass through message buffer */\n    for (var i = 0; i < len && i < data.length; i++) {\n      nextByte = 0xFF & (data[i]);\n      uIndex = crc[0] ^ nextByte; //*puchMsg++; /* calculate the CRC */\n      crc[0] = crc[1] ^ auchCRCHi[uIndex];\n      crc[1] = auchCRCLo[uIndex];\n    }\n\ndata[len]=crc[0];\ndata[len+1]=crc[1];\nmsg.payload= data;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 770,
        "y": 280,
        "wires": [
            [
                "f2ed6391ec7a4e8a"
            ]
        ]
    },
    {
        "id": "95d7fec09cba652a",
        "type": "function",
        "z": "2444ab4bb113d3cc",
        "name": "Read Data(temp&humidity)",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\n//buf=[0xFF,0x03,0x00,0x00,0x00,0x01,0x91,0xD4];\nbuf=[0xff,0x03,0x00,0x00,0x00,0x02,0x00,0x00];\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 480,
        "y": 460,
        "wires": [
            [
                "8e20dd5a8cf9e541"
            ]
        ]
    },
    {
        "id": "c9c1c29303eb3730",
        "type": "inject",
        "z": "2444ab4bb113d3cc",
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
        "x": 200,
        "y": 460,
        "wires": [
            [
                "95d7fec09cba652a"
            ]
        ]
    },
    {
        "id": "e96a2204028295f8",
        "type": "function",
        "z": "2444ab4bb113d3cc",
        "name": "Read station number(fixed command)",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0x00,0x03,0x00,0x01,0x00,0x01,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 470,
        "y": 240,
        "wires": [
            [
                "8e20dd5a8cf9e541"
            ]
        ]
    },
    {
        "id": "e04431b473654a99",
        "type": "inject",
        "z": "2444ab4bb113d3cc",
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
        "x": 200,
        "y": 240,
        "wires": [
            [
                "e96a2204028295f8"
            ]
        ]
    },
    {
        "id": "3b456db85f37800f",
        "type": "comment",
        "z": "2444ab4bb113d3cc",
        "name": "프로토콜 참조 설명",
        "info": "https://ko.aliexpress.com/item/1005002356037000.html?spm=a2g0s.9042311.0.0.27424c4dc8WPuJ\n\n",
        "x": 230,
        "y": 200,
        "wires": []
    },
    {
        "id": "1b6407d73ea9d9c2",
        "type": "debug",
        "z": "2444ab4bb113d3cc",
        "name": "",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "payload",
        "targetType": "msg",
        "statusVal": "",
        "statusType": "auto",
        "x": 1030,
        "y": 400,
        "wires": []
    },
    {
        "id": "5edcb369d44476df",
        "type": "function",
        "z": "2444ab4bb113d3cc",
        "name": "Wriite station number",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\nbuf=[0x00,0x10,0x00,0x01,0x00,0x01,0x02,0x00,0xff,0x00,0x00];\n\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 420,
        "y": 300,
        "wires": [
            [
                "8e20dd5a8cf9e541"
            ]
        ]
    },
    {
        "id": "3217deb62fd6956c",
        "type": "inject",
        "z": "2444ab4bb113d3cc",
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
        "x": 200,
        "y": 300,
        "wires": [
            [
                "5edcb369d44476df"
            ]
        ]
    },
    {
        "id": "c15dccc790c1e008",
        "type": "function",
        "z": "2444ab4bb113d3cc",
        "name": "Read data(single humidity)",
        "func": "//Relay on: FF 0F 00 00 00 08 01 FF 30 1D\nvar buf = Buffer.alloc(8);\n//buf=[0xFF,0x03,0x00,0x00,0x00,0x01,0x91,0xD4];\nbuf=[0xff,0x03,0x00,0x00,0x00,0x01,0x00,0x00];\nmsg.payload=buf;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 480,
        "y": 380,
        "wires": [
            [
                "8e20dd5a8cf9e541"
            ]
        ]
    },
    {
        "id": "313ae9cbf8d3e4ed",
        "type": "inject",
        "z": "2444ab4bb113d3cc",
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
        "x": 200,
        "y": 380,
        "wires": [
            [
                "c15dccc790c1e008"
            ]
        ]
    },
    {
        "id": "63957752.49a888",
        "type": "serial-port",
        "serialport": "COM4",
        "serialbaud": "9600",
        "databits": "8",
        "parity": "none",
        "stopbits": "1",
        "waitfor": "",
        "dtr": "none",
        "rts": "none",
        "cts": "none",
        "dsr": "none",
        "newline": "100",
        "bin": "bin",
        "out": "time",
        "addchar": "",
        "responsetimeout": "10000"
    }
]

```

### 2. 습도, 온도 계산하기
*  Example       

|응답 예시|프로토콜|
|----|----------|
|Example respond|ff 03 04 00 11 0E BA 30 2A|

Leaf humidity = 00 11 = 17/10 = 1.7 %

Leaf temperature = 0E BA = 3770/100-20 = 17.7 ℃

*  flow 사진
  
 ![온습도](https://user-images.githubusercontent.com/37902752/159205051-512e969f-f6c7-4e08-8450-4fbb36bc4c4a.PNG)



--------

### 준비물
|이름|사진|
|----|-----|
|Leaf humidity sensor|![센서 readme](https://user-images.githubusercontent.com/37902752/159655457-a0bd3b22-c079-4839-919e-4a962a22f039.png)|
|USB to 485/422 컨버터|![컨버터](https://user-images.githubusercontent.com/37902752/153120409-b8993064-b20b-4171-bdbd-e1003226707b.PNG)|
|Oled|![oleedd](https://user-images.githubusercontent.com/37902752/153120187-e9dfc7bd-49fb-484d-994d-68bb78064cb1.png)|
|esp8266|![d1mmini](https://user-images.githubusercontent.com/37902752/153120047-a398f2d9-739f-4dbd-a7e8-49a560cae8d0.png)|
|RS485보드|![rs485](https://user-images.githubusercontent.com/37902752/153120286-4402da02-ea72-4781-afa7-445473d1f783.png)|
|터미널|![터미널 readme](https://user-images.githubusercontent.com/37902752/159655549-d79ccf25-7bf4-4bab-9bd3-904fb3d2f4a8.png)|


## 온습도 센서와 RS485 컨버터 배선도 (온습도 센서 동작확인)

![온습도센서회로도](https://user-images.githubusercontent.com/37902752/159645945-2dc4be4a-dd16-4965-b180-022866fdbcb5.png)


## 온습도 센서와 esp8266및 RS485보드, oled, 터미널 배선도
![온습도 센서 회로도](https://user-images.githubusercontent.com/37902752/159651811-c64440bb-dd97-4206-a4ef-4b7fad2be09d.png)



### 485 통신
현장에서 가장많이쓰는 통신으로
4km정도로 통신이 가능하고 병렬로 연결하여 사용하기 편하다는 장점이 있다
