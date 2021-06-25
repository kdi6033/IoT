String webTail="</body> </html>";

String webMenu="";
String webHtml="";

//***********************************************************************
char Head[] PROGMEM = R"=====(
<html>
  <head>
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
        .buttonMenu {
          padding: 5px 24px;
          margin-left:20%;
          background-color:black;
          border: none;
          border-color:black;
          color:white;
          text-align: left;
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

char RootScript[] PROGMEM = R"=====(
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function(event){
        var data = JSON.parse(event.data);
        //console.log(data.temp);
        document.getElementById("ec").innerHTML = data.ec;
        document.getElementById("ph").innerHTML = data.ph;
        document.getElementById("temperature").innerHTML = data.temp;
      }
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


char HeadScript[] PROGMEM = R"=====(
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
    function sendDownOption() {
      document.getElementById("downFile").innerHTML=document.getElementById("download").value;
      var s="#"+"{'act':2,'value':'"+document.getElementById("download").value+"'}";
      Socket.send(s);
    }
    function sendDownOptionMqtt() {
      document.getElementById("downFile").innerHTML=document.getElementById("downloadMqtt").value;
      var s="#"+"{'act':2,'value':'"+document.getElementById("downloadMqtt").value+"'}";
      Socket.send(s);
    }
    function sendDownOptionAws() {
      document.getElementById("downFile").innerHTML=document.getElementById("downloadAws").value;
      var s="#"+"{'act':2,'value':'"+document.getElementById("downloadAws").value+"'}";
      Socket.send(s);
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

char Body[] PROGMEM = R"=====(
  <br><br>다운로드 파일명 down-local-monit-02.bin
  <br><br>
  <label>EC : </label>
  <span id="ec">%EC%</span>
  
  <br><label>PH : </label>
  <span id="ph">%PH%</span>
  
  <br><span class="dht-labels">온도</span> 
    <span id="temperature">%TEMPERATURE%</span>
  <sup class="units">&deg;C</sup>
  
)=====";

char Menu[] PROGMEM = R"=====(
  <div id='mySidenav' class='sidenav'>
  <a href='javascript:void(0)' class='closebtn' onclick='closeNav()'>&times;</a>
  <a href='/'>홈</a>
  <a href='/download'>다운로드</a>
  <a href='/wifi'>와이파이설정</a>
  <a href='/config'>환경설정</a>
  <a href='/manual'>메뉴얼</a>
  <a href='http://i2r.link'>김동일홈피</a>
  </div>
  <span style='font-size:30px;cursor:pointer' onclick='openNav()'>&#9776; </span>
  메뉴열기
)=====";

char Download[] PROGMEM = R"=====(
  <br><br>다운로드
  추가된 기기를 다운로드 받으려면 펌웨어 업그레이드를 하세요.
  <div> <button id="onButton" class='button button-box' onclick="sendAct('#'+'{\'act\':1}');">최신펌웨어 업그레이드</button> </div>  
  <hr>
  <br>현장에서 이 기기만으로 모니터링/제어 하는 프로그램<br>
  <select id='download' name='download' onclick='sendDownOption();'>
    <option value='down-local-monit-02.bin'>[sensecube] PE-300</option>";
  </select>
  <br><br>현장에서 mqtt통신을 사용해 로컬서버를 구성하여 모니터링/제어 하는 프로그램<br>
  <select id='downloadMqtt' name='downloadMqtt' onclick='sendDownOptionMqtt();'>
    <option value='down-local-mqtt-01.bin'>mqtt 온도</option>";
    <option value='down-local-mqtt-02.bin'>mqtt 습도</option>";
  </select>
  <br><br>아마존 서버에 접속하여 크라우드 환경에서 모니터링/제어 하는 프로그램
  <br>E-mail ID로 회원 가입하여 로그인 하면 자동 접속한 기기를 볼 수 있음
  <br>접속사이트 : <a href='http://i2r.link/'>http://i2r.link/</a>
  <br>
  <select id='downloadAws' name='downloadAws' onclick='sendDownOptionAws();'>
    <option value='down-aws-monit-01.bin'>aws 온도</option>";
    <option value='down-aws-monit-02.bin'>aws 습도</option>";
  </select>
  <br>다운로드 파일 이름 : <span id="downFile">파일없음</span>
  <button id="onButton" class='button button-box' onclick="sendAct('#'+'{\'act\':3}');">다운로드</button>
  <hr>
)=====";

char Manual[] PROGMEM = R"=====(
  <br><br>메뉴얼
  <a href='https://github.com/kdi6033/IoT/tree/main/11-2-1%20sensecube%20PE-300%20arduino'>PE-300 선연결</a>
  
)=====";

char Tail[] PROGMEM = R"=====(
</body>
</html>
)=====";

void handleRoot() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(RootScript);
  s+=FPSTR(Menu);
  s+=FPSTR(Body);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}

void handleOn() {
  act=server.arg("act").toInt();
  //int value=server.arg("value").toInt();

  Serial.println("----------------------------");
  Serial.println(act);
  //Serial.println(no);
  //Serial.println(value);

  if(act==2) {
    server.arg("ipMqtt").toCharArray(ipMqtt, sizeof(ipMqtt) - 1);
    server.arg("userMqtt").toCharArray(userMqtt, sizeof(userMqtt) - 1);
    server.arg("passwordMqtt").toCharArray(passwordMqtt, sizeof(passwordMqtt) - 1);
    server.arg("email").toCharArray(email, sizeof(email) - 1);
    Serial.println(ipMqtt);
    Serial.println(userMqtt);
    Serial.println(passwordMqtt);
    Serial.println(email);
    saveConfig();
  }
  GoHome();
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
  String s; 
  String s1= String(ssid);
  s=s+"<h1>Wifi 사양</h1>";
  if (server.client().localIP() == apIP) {
    s=s+String("<p>연결된 AP: 192.168.4.1") + "</p>";
  } else {
    s=s+"<p>연결된 와이파이 " + String(ssid) + "</p>";
  }

  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");

  if (n > 0) {
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
  sOut+=FPSTR(HeadScript);
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
}

void handleScan() {
  String s;
    s="{\"mac\":\""+sMac+"\",\"ip\":\""+WiFi.localIP().toString()+"\",\"ec\":"+ec+",\"ph\":"+ph+",\"temp\":"+temp+"}";
  server.send(200, "text/html", s);
}

void handleConfig() {
  String s;
  s="<br><br>현장에서 mqtt 통신을 사용하지 않고 웹페이지 모니터링만 하려면 입력하지 않아도 됩니다.<br>";
  s+="<br> 아마존 서버 http://i2r.link 접속하려면 회원가입 한 email을 입력하세요.<br>";
  s+="<form action='/on'>";
  s+="<input type='hidden' name='act' value='2'>";
  s+="<table>";
    s+="<tr>";
      s+="<th>mqtt서버 IP</th>";
      s+="<th><input type='text' value='"+(String)ipMqtt+"' name='ipMqtt'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th>사용자이름</th>";
      s+="<th><input ttype='text' value='"+(String)userMqtt+"' name='userMqtt'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th>비밀번호</th>";
      s+="<th><input type='password' value='"+(String)passwordMqtt+"' name='passwordMqtt'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th>email</th>";
      s+="<th><input type='text' value='"+(String)email+"' name='email'/></th>";
    s+="</tr>";
    s+="<tr>";
      s+="<th></th>";
      s+="<th><input type='submit' value='    저  장    ' style='background-color:#ff8000;color: white;border: none;padding: 6px 15px;'/></th>";
    s+="</tr>";
  s+="</table>";
  s+="</form>";

  String sOut;
  sOut=FPSTR(Head);
  sOut+=FPSTR(HeadScript);
  sOut+=FPSTR(Menu);
  sOut+=s;
  sOut+=FPSTR(Tail);
  server.send(200, "text/html", sOut);
}

void handleDownload() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(HeadScript);
  s+=FPSTR(Menu);
  if(FirmwareVer != FirmwareVerServer) {
    s+="<br><br>새로운 기기가 추가 되었습니다. 펌웨어 업그레이드 하세요.<br>";
    s+="새로운 버젼"+FirmwareVerServer+"&emsp; 보드버젼"+FirmwareVer;
  }
  s+=FPSTR(Download);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}

void handleManual() {
  String s;
  s=FPSTR(Head);
  s+=FPSTR(HeadScript);
  s+=FPSTR(Menu);
  s+=FPSTR(Manual);
  s+=FPSTR(Tail);
  server.send(200, "text/html", s);
}
