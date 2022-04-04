String webTail="</body> </html>";

String webMenu="";
String webHtml="";

//***********************************************************************
char Head[] PROGMEM = R"=====(
<html>
  <head>
    <link rel="shortcut icon" href="#">
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <meta http-equiv='Content-Type' content='text/html;charset=utf-8' />
    <style>
      table, th, td {
        padding: 4px;
      }
      body {
        background: #eab0dc;
        font-family: "Lato", sans-serif;
      }
      .button {
        border: none;
          border-color:black;
          color: white;
          padding: 20px;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          margin: 4px 2px;
          cursor: pointer;
        }
        .sidenav {
          height: 100%;
          width: 0;
          position: fixed;
          z-index: 1;
          top: 0;
          left: 0;
          background-color: #111;
          overflow-x: hidden;
          transition: 0.5s;
          padding-top: 60px;
        }
        .sidenav a {
          padding: 8px 8px 8px 32px;
          text-decoration: none;
          font-size: 18px;
          color: #818181;
          display: block;
                transition: 0.3s;
        }
        .sidenav a:hover {
          color: #f1f1f1;
        }
        .sidenav .closebtn {
          position: absolute;
          top: 0;
          right: 25px;
          font-size: 36px;
          margin-left: 50px;
        }
        .button-box {background-color:#ff8000;color: white;border: none;padding: 6px 15px;}
        .button-on {border-radius: 100%; background-color: #4CAF50;}
        .button-off {border-radius: 100%;background-color: #707070;}
        .button-ledon {border-radius: 100%; padding: 10px; font-size: 8px; margin: 0px 0px; background-color: #ff4500;}
        .button-ledoff {border-radius: 100%; padding: 10px; font-size: 8px; background-color: #707070;}
    </style>
)=====";

char ScriptRoot[] PROGMEM = R"=====(
  <script>
      var Socket;
      function init() {
        Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
        Socket.onmessage = function(event){
          var data = JSON.parse(event.data);
          //var data = event.data;
          console.log(data);
          if(data.in0==1)
            document.getElementById("in0").innerHTML = "<button class='button button-ledon' ></button>";
          else 
            document.getElementById("in0").innerHTML = "<button class='button button-ledoff' ></button>";
          if(data.in1==1)
            document.getElementById("in1").innerHTML = "<button class='button button-ledon' ></button>";
          else 
            document.getElementById("in1").innerHTML = "<button class='button button-ledoff' ></button>";
          if(data.in2==1)
            document.getElementById("in2").innerHTML = "<button class='button button-ledon' ></button>";
          else 
            document.getElementById("in2").innerHTML = "<button class='button button-ledoff' ></button>";
          if(data.in3==1)
            document.getElementById("in3").innerHTML = "<button class='button button-ledon' ></button>";
          else 
            document.getElementById("in3").innerHTML = "<button class='button button-ledoff' ></button>";

          if(data.out0==1)
            document.getElementById("out0").innerHTML = "<button class='button button-on' onclick=\"sendAct('{\\'act\\':2,\\'no\\':0,\\'value\\':0}');\"></button>";
          else
            document.getElementById("out0").innerHTML = "<button class='button button-off' onclick=\"sendAct('{\\'act\\':2,\\'no\\':0,\\'value\\':1}');\"></button>";
          
          if(data.out1==1)
            document.getElementById("out1").innerHTML = "<button class='button button-on' onclick=\"sendAct('{\\'act\\':2,\\'no\\':1,\\'value\\':0}');\"></button>";
          else
            document.getElementById("out1").innerHTML = "<button class='button button-off' onclick=\"sendAct('{\\'act\\':2,\\'no\\':1,\\'value\\':1}');\"></button>";
          if(data.out2==1)
            document.getElementById("out2").innerHTML = "<button class='button button-on' onclick=\"sendAct('{\\'act\\':2,\\'no\\':2,\\'value\\':0}');\"></button>";
          else
            document.getElementById("out2").innerHTML = "<button class='button button-off' onclick=\"sendAct('{\\'act\\':2,\\'no\\':2,\\'value\\':1}');\"></button>";
          if(data.out3==1)
            document.getElementById("out3").innerHTML = "<button class='button button-on' onclick=\"sendAct('{\\'act\\':2,\\'no\\':3,\\'value\\':0}');\"></button>";
          else
            document.getElementById("out3").innerHTML = "<button class='button button-off' onclick=\"sendAct('{\\'act\\':2,\\'no\\':3,\\'value\\':1}');\"></button>";

        }
      }
      function sendAct(valueIn){
        Socket.send(valueIn);
      }
      function openNav() {
        document.getElementById("mySidenav").style.width = "150px"; 
      }
      function closeNav() {
        document.getElementById("mySidenav").style.width = "0";
      }
  </script> 
 </head>
 <body onload="javascript:init()">
)=====";

char ScriptHead[] PROGMEM = R"=====(
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
      }
    }
    function sendOption(){
      document.getElementById("ssidSelected").value = document.getElementById("ssid").value;
      //Socket.send("$"+document.getElementById("ssid").value);
    }
    function sendAct(valueIn){
      Socket.send(valueIn);
    }
    function openNav() {
      document.getElementById("mySidenav").style.width = "150px"; 
    }
    function closeNav() {
      document.getElementById("mySidenav").style.width = "0";
    }
  </script>
</head>
<body onload="javascript:init()">

)=====";

/*
char webBody[] PROGMEM = R"=====(
  <br>입력<br>
  <span id="in0"></span>
  <span id="in1"></span>
  <span id="in2"></span>
  <span id="in3"></span>
  <br>출력<br>
  <span id="out0">0</span>
  <span id="out1">1</span>
  <span id="out2">2</span>
  <span id="out3">3</span>
  </body>
  </html>
)=====";
*/

char Body[] PROGMEM = R"=====(
  <br>입력<br>
  <table>
    <tr>
      <th>00</th>
      <th>01</th>
      <th>02</th>
      <th>03</th>
    </tr>
    <tr>
      <th><span id="in0"></span></th>
      <th><span id="in1"></span></th>
      <th><span id="in2"></span></th>
      <th><span id="in3"></span></th>
    </tr>
  </table>

  <br>출력<br>
  <table>
    <tr>
      <th>00</th>
      <th>01</th>
      <th>02</th>
      <th>03</th>
    </tr>
    <tr>
      <th><span id="out0">0</span></th>
      <th><span id="out1">1</span></th>
      <th><span id="out2">2</span></th>
      <th><span id="out3">3</span></th>
    </tr>
  </table>
)=====";


char Menu[] PROGMEM = R"=====(
  <div id='mySidenav' class='sidenav'>
  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()' rel="shortcut icon">&times;</a>
  <a href='/' rel="shortcut icon">홈</a>
  <a href='/download' rel="shortcut icon">다운로드</a>
  <a href='/wifi' rel="shortcut icon">와이파이설정</a> 
  <a href='https://github.com/kdi6033/IoT' target='_blank' rel="shortcut icon">메뉴얼</a>
  <a href='http://i2r.link' target='_blank' rel="shortcut icon">김동일홈피</a>
  </div>
  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>
  메뉴열기
)=====";

char Download[] PROGMEM = R"=====(
  <br><br>다운로드<br>
  새로운 기기를 다운로드 받으려면 [최신펌웨어 업그레이드]를 하면 기기 리스트가 보입니다.<br> 
  기기선택 후 업그레이드 하세요.<br>
  <div> <button id="onButton" class='button button-box' onclick="sendAct('{\'act\':1}');">기기선택 펌웨어 다운로드</button> </div>  
  <hr>

)=====";

char Tail[] PROGMEM = R"=====(
</body>
</html>
)=====";

void handleRoot() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(ScriptRoot);
  s+=FPSTR(Menu);
  s+=FPSTR(Body);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}

void GoHome() {
  String s;
  s="<meta http-equiv='refresh' content=\"0;url='http://"+ipAct+"/'\">";
  server.send(200, "text/html", s);
}

void GoHomeWifi() {
  String s,ipS;
  IPAddress ip;
  if(bootMode==1)
    ip = apIP;
  else
    ip = WiFi.localIP();
  
  ipS=String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
    s="<meta http-equiv='refresh' content=\"0;url='http://"+ipS+"/wifi'\">";
  server.send(200, "text/html", s);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

//와이파이 강도측정
String sensWifi(int in) {
  if(in < -80)
    return "9783";
  else if(in < -70)
    return "9779";
  else if(in < -50)
    return "9777";
  else 
    return "9776";
}

void handleWifi() {
  String s=""; 
  String s1= String(ssid);
  s="<h1>Wifi 선택</h1>";
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  delay(100);

  if (n > 0) {
    //s+="검색된 와이파이";
    s+="<select id='ssid' name='ssid' onclick=\"sendOption();\">";
    for (int i = 0; i < n; i++) {
      s+="<option value='"+WiFi.SSID(i)+"'>";
      s+=WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"&emsp;&#128274")+"&emsp; &#"+ sensWifi(WiFi.RSSI(i));
      s+="</option>";
    }
    s+="</select>";
  } 
  else 
    s=s+"<br>No WLAN found";

  s=s+"<p><a href='/wifi'>와이파이가 없으면 다시 검색하세요.</a></p>";
  s+="<form method='POST' action='/wifisave'>";
  s+="<table>";
    s+="<tr>";
      s+="<th>와이파이 이름</th>";
      s+="<th><input type='text' id='ssidSelected' value='"+(String)ssid+"' name='n'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th>비밀번호</th>";
      s+="<th><input type='password' value='"+(String)password+"' name='p'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th></th>";
      s+="<th><input type='submit' value='    저  장    ' style='background-color:#ff8000;color: white;border: none;padding: 6px 15px;'/></form></th>";
    s+="</tr>";
  String sOut;
  sOut=FPSTR(Head);
  sOut+=FPSTR(ScriptHead);
  sOut+=FPSTR(Menu);
  sOut+=s;
  sOut+=FPSTR(Tail);
  server.send(200, "text/html", sOut);
}
/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  displayOled(4);
  GoHome();
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveConfig();
  //connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
  Serial.println(WiFi.waitForConnectResult());
  Serial.println(ssid);
  Serial.println(password);
  Serial.println("Reset");
  ESP.reset();
  delay(2000);
}

void handleScan() {
  String s;
  String sIn="";
  sIn=String(In[0])+String(In[1])+String(In[2])+String(In[3])+String(In[4])+String(In[5])+String(In[6])+String(In[7]);
  s="{\"mac\":\""+sMac+"\",\"ip\":\""+WiFi.localIP().toString()+"\",\"type\":"+type+",\"in\":\""+sIn+"\"}";
  server.send(200, "text/html", s);
}

void handleDownload() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(ScriptHead);
  s+=FPSTR(Menu);
  s+=FPSTR(Download);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}
