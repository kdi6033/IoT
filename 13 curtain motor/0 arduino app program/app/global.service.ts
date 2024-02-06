//global.Service.ts
import { Injectable} from '@angular/core';
import { BleClient, BleDevice } from '@capacitor-community/bluetooth-le';
import { BehaviorSubject,Observable } from 'rxjs';
declare var cordova: any;

// 인터페이스 정의
interface DataDevice {
  mac: string; // Bluetooth mac address
  out: number[];  //측정된 값
  in: number[];   //측정된 값
  humidity: number;
  temperature: number;
  strIn: string; // In[] 을 string으로 저장
  strInPre: string; // 이전 In[] 값을 저장
  sendData: string; // 보드의 입력, 출력, 전압 데이터를 json 형태로 저장
  no: number;     // 출력 밸브 번호를 전송
  value: boolean; // 출력 밸브 번호를 전송
  isSliderOn: boolean;
}

interface DataBle {
  SERVICE_UUID: string;  // 블루투스 서비스 UUID
  CHARACTERISTIC_UUID: string;  // 블루투스 특성 UUID
  isConnected: boolean;
  connectedDevice: BleDevice | null;
 }

export interface DataWifiMqtt {
  selectMqtt: boolean;    // false-Ble true-mqtt
  // use: boolean;
  isConnected: boolean;
  isConnectedMqtt: boolean;
  ssid: string;
  password: string;
  mqttBroker: string;
  email: string;
  customMqttBroker: string; // mqtt boker에서 custom을 선택해도 사라지지 않게
  outTopic: string;
  inTopic: string;
  bleLabelClass: string;
  wifiLabelClass: string;
}

@Injectable({
  providedIn: 'root'
})

export class GlobalService {

  dev: DataDevice = {
    mac: "",
    out: new Array(4).fill(0),
    in: new Array(4).fill(0),
    humidity: 0.0,
    temperature: 0.0,
    strIn: "0000",
    strInPre: "0000",
    sendData: "",
    no: 100,
    value: false,
    isSliderOn: false
  };

  // DataBle 타입의 객체 초기화
  ble: DataBle = {
    SERVICE_UUID: "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
    CHARACTERISTIC_UUID: "beb5483e-36e1-4688-b7f5-ea07361b26a8",
    isConnected: false,
    connectedDevice: null,
   };

  wifi: DataWifiMqtt = {
    selectMqtt: false,
    // use: false,
    isConnected: false,
    isConnectedMqtt: false,
    ssid: "",
    password: "",
    mqttBroker: "",
    customMqttBroker: "",
    email: "",
    outTopic: "",
    inTopic: "",
    bleLabelClass: "",
    wifiLabelClass: ""
  };

  devices: BleDevice[] = [];
  intervalId: any;
  mqttBroker1: string = ""; // 브로커 주소
  receivedMessage: string = ""; // 수신된 메시지 저장
  private mqttConnectionSubject = new BehaviorSubject<boolean>(this.wifi.isConnectedMqtt);
  mqttConnection$ = this.mqttConnectionSubject.asObservable();

  // MQTT 연결 상태 변경 시 BehaviorSubject 업데이트
  updateMqttConnectionStatus(isConnected: boolean) {
    this.wifi.isConnectedMqtt = isConnected;
    this.mqttConnectionSubject.next(isConnected);
  }

  private wifiDataSubject = new BehaviorSubject<DataWifiMqtt>(this.wifi);
  wifiData$ = this.wifiDataSubject.asObservable();

  // 현재 설정된 SSID를 반환하는 함수
  getWifiSSID(): string {
    return this.wifi.ssid;
  }

  updateWifiData(newData: DataWifiMqtt) {
    this.wifi = newData;
    this.wifiDataSubject.next(this.wifi);
  }

  private devInSubject = new BehaviorSubject<number[]>(this.dev.in);
  devIn$ = this.devInSubject.asObservable();

  updateDevIn(newIn: number[]) {
    this.dev.in = newIn;
    this.devInSubject.next(this.dev.in);
  }

  constructor() {
    this.initializeBluetooth();
    if (this.isLocalStorageAvailable()) {
      this.loadMqttBrokerFromLocalStorage();
    }
    // 블루투스와 MQTT 연결 상태를 주기적으로 확인하는 로직
    this.intervalId = setInterval(() => {
      this.checkAndReconnectBluetooth();
      if (this.wifi.selectMqtt) {
        this.checkMQTTConnection();
      }
    }, 2000); // 2초마다 실행

  this.reconnectToSavedDevice();
  }
  private async reconnectToSavedDevice() {
    if (this.isLocalStorageAvailable()) {
      const savedDeviceId = localStorage.getItem('connectedDeviceId');
      if (savedDeviceId) {
        try {
          const devices = await BleClient.getDevices([savedDeviceId]); // Pass savedDeviceId as an argument
          const device = devices.find(d => d.deviceId === savedDeviceId);
          if (device) {
            await this.connectToDevice(device);
          }
        } catch (error) {
          console.error('Failed to reconnect:', error);
        }
      }
    } else {
      console.error('Local Storage is not available');
    }
  }

  private saveDeviceIdToLocalStorage(deviceId: string) {
    if (this.isLocalStorageAvailable()) {
      localStorage.setItem('connectedDeviceId', deviceId);
    } else {
      console.error('Local Storage is not available');
    }
  }
async checkMQTTConnection() {
  if (this.wifi.selectMqtt && !this.wifi.isConnectedMqtt) {
    // MQTT가 선택되어 있지만 연결되어 있지 않은 경우
    try {
      await this.connectToMQTT();
      console.log('MQTT 재연결 시도');
    } catch (error) {
      console.error('MQTT 재연결 실패:', error);
    }
  }
}
ngOnInit() {
  this.autoReconnect();
}
private autoReconnect() {
  // Check connection status every few seconds and reconnect if needed
  this.intervalId = setInterval(() => {
    if (!this.ble.isConnected) {
      this.reconnectToSavedDevice();
    }
  }, 5000); // Check every 5 seconds
}
public attemptAutoReconnect() {
  // Logic to automatically reconnect to the saved device
  this.reconnectToSavedDevice();
}
  async initializeBluetooth() {
    try {
      await BleClient.initialize();
      console.log('Bluetooth initialized successfully');
    } catch (error) {
      console.error('Error initializing Bluetooth:', error);
    }
  }

  async scanAndConnect() {
    try {
      const device = await BleClient.requestDevice({
        namePrefix: 'i2r-',
        optionalServices: [this.ble.SERVICE_UUID]
      });

      if (device) {
        await this.connectToDevice(device);
      } else {
        console.log('No device selected');
      }
    } catch (error) {
      console.error('Error scanning and connecting to device:', error);
    }
  }

  async connectToDevice(device: BleDevice) {
    try {
      await BleClient.connect(device.deviceId);
      console.log('Connected to', device.name, 'with deviceId', device.deviceId);
      this.ble.connectedDevice = device;
      this.ble.isConnected = true;

      this.postConnectionSetup(device);
      this.saveDeviceIdToLocalStorage(device.deviceId);
    } catch (error) {
      console.error('Failed to connect:', error);
      this.ble.isConnected = false;
    }
  }
  postConnectionSetup(device: BleDevice) {
    // 토픽 설정 및 기타 설정
    this.wifi.outTopic = `i2r/${device.deviceId}/in`;
    this.wifi.inTopic = `i2r/${device.deviceId}/out`;
    console.log("outTopic", this.wifi.outTopic);
    console.log("inTopic", this.wifi.inTopic);

    this.connectToMQTT(); // MQTT 연결 시도
    this.startNotifications(device.deviceId); // 알림 시작
  }
  async disconnect() {
    if (this.ble.connectedDevice) {
      try {
        await BleClient.disconnect(this.ble.connectedDevice.deviceId);
        console.log('Disconnected from', this.ble.connectedDevice.name);
        this.ble.connectedDevice = null;
        this.ble.isConnected = false; // 연결 해제 시 전역 변수 업데이트
        this.dev.isSliderOn = true;
        this.wifi.selectMqtt = true;
        // Update label classes

        this.wifi.bleLabelClass = 'text-gray';
        this.wifi.wifiLabelClass = 'text-green';
      } catch (error) {
        console.error('Failed to disconnect:', error);
      }
    } else {
      console.log('No device is connected');
    }
  }

  //message를 받아서 처리한다.
  async startNotifications(deviceId: string) {
    try {
      await BleClient.startNotifications(
        deviceId,
        this.ble.SERVICE_UUID,
        this.ble.CHARACTERISTIC_UUID,
        (data) => {
          // 여기서 데이터 처리를 합니다.
          const message = new TextDecoder().decode(data);
          console.log('Received message from device:', message);
          this.processReceivedMessage(message);
        }
      );
      console.log('Started notifications for', deviceId);
    } catch (error) {
      console.error('Failed to start notifications:', error);
    }
  }

 // 수신된 메시지를 처리하는 함수
 processReceivedMessage(jsonString: string) {
  try {
    console.log("processReceivedMessage",jsonString)
    // JSON 문자열을 객체로 파싱합니다.
    const messageObject = JSON.parse(jsonString);
    // 'order' 값에 따라 다른 처리를 수행합니다.
    switch (messageObject.order) {
      case 1:
        // 'order'가 1일 때의 처리 로직
        this.updateWifiData({
          ...this.wifi,
          ssid: messageObject.ssid,
          password: messageObject.password,
          customMqttBroker: messageObject.mqttBroker,
          email: messageObject.email,
          // use: messageObject.use
        });
        console.log('After update:', this.wifi);
        break;
      case 2:
        // 'order'가 2일 때의 처리 로직 from Ble
        //this.updateReceivedMessage(jsonString);
        break;
      case 3:
          // 글로벌 서비스의 수신된 메시지를 업데이트합니다
          this.updateReceivedMessage(jsonString);
          break;
      case 101:
          // 실행에 대한 리턴 메시지를 표시합니다
          this.receivedMessage = messageObject.message;
          break;
      default:
        console.log('알 수 없는 order:', messageObject.order);
    }
  } catch (error) {
    console.error('JSON 메시지 처리 중 오류 발생:', error);
  }

  //const newIn = /* 변환된 in 배열 */;
  //this.updateDevIn(newIn);
}

async checkAndReconnectBluetooth() {
  if (!this.ble.connectedDevice) {
    console.log("No device");
    return;
  }

  try {
    await BleClient.getServices(this.ble.connectedDevice.deviceId);
    this.ble.isConnected = true;
  } catch (error) {
    console.error("Error checking Bluetooth connection:", error);
    this.ble.isConnected = false;

    try {
      await BleClient.connect(this.ble.connectedDevice.deviceId);
      console.log('Bluetooth reconnected:', this.ble.connectedDevice.deviceId);
      this.ble.isConnected = true;

      this.postConnectionSetup(this.ble.connectedDevice);
    } catch (reconnectError) {
      console.error("Error reconnecting Bluetooth:", reconnectError);
    }
  }
}

  updateReceivedMessage(message: string) {
    console.log(message)
    //this.receivedMessage = message;
    this.processMessage(message);
  }

  private processMessage(jsonString: string) {
    try {
      console.log("------------------");
      console.log(jsonString);
      const messageObject = JSON.parse(jsonString);
      const inString: string = messageObject.in;
      const outString: string = messageObject.out;
      console.log(messageObject.in);
      console.log(inString);

      // "in" 문자열의 각 글자를 숫자로 변환하여 in 배열에 저장합니다.
      for (let i = 0; i < inString.length; i++) {
        this.dev.in[i] = parseInt(inString.charAt(i), 10);
      }
      this.updateDevIn(this.dev.in);


      // "out" 문자열의 각 글자를 숫자로 변환하여 out 배열에 저장합니다.
      for (let i = 0; i < outString.length; i++) {
        this.dev.out[i] = parseInt(outString.charAt(i), 10);
      }

      if (messageObject.humidity) {
        this.dev.humidity = parseFloat(messageObject.humidity);
      }
      if (messageObject.temperature) {
        this.dev.temperature = parseFloat(messageObject.temperature);
      }

      // dev.in 배열을 복사하여 로그로 출력
      const devInCopy = [...this.dev.in];
      console.log('dev.in 배열 복사본:', devInCopy);

      // 여기에서 dev 객체의 현재 상태를 출력합니다.
      console.log('Current state of dev:', this.dev);

    } catch (error) {
      console.error('Error processing message:', error);
    }
  }
  private humiSubject = new BehaviorSubject<number>(this.dev.humidity);
  humidity$ = this.humiSubject.asObservable();

  private tempSubject = new BehaviorSubject<number>(this.dev.temperature);
  temperature$ = this.tempSubject.asObservable();
  // Update these values and emit the changes
  updateVbat(humidity: number) {
    this.dev.humidity = humidity;
    this.humiSubject.next(humidity);
  }

  updateAdc(temperature: number) {
    this.dev.temperature = temperature;
    this.tempSubject.next(temperature);
  }
  async sendData(order: number) {
    console.log(order);
    let data;
    if (order === 0) {
      data = {
        order: 0,
        fileName: "i2r-03-ai.ino.bin",
        message: "download firmware"
      };
    }
    else if (order==1) {
      data = {
        order: 1,
        ssid: this.wifi.ssid,
        password: this.wifi.password,
        mqttBroker: this.wifi.mqttBroker,
        email: this.wifi.email,
        message: "board config"
      };
    }
    else if (order === 2) {
      // 첫 번째 연결에서 보드 설정 값을 요청하는 메시지
      data = {
        order: 2,
        no: this.dev.no,
        value: this.dev.value,
        message: "board ouput"
      };
    }
    else if (order === 3) {
      // 와이파이 사용하는지?
      data = {
        order: 3,
        value: this.wifi.selectMqtt,
        message: "select Mqtt"
      };
    }
    else {
      // 다른 'order' 값에 따른 데이터 처리를 구현할 수 있습니다.
    }

    console.log("Sending data : ", data);
    if (!this.wifi.selectMqtt) {
      // selectMqtt가 false일 때 블루투스를 통한 데이터 송신
      if (data && this.ble.connectedDevice) {
        console.log("via Bluetooth:");
        const uint8Array = new TextEncoder().encode(JSON.stringify(data));
        const dataView = new DataView(uint8Array.buffer);
        try {
          await BleClient.write(
            this.ble.connectedDevice.deviceId,
            this.ble.SERVICE_UUID,
            this.ble.CHARACTERISTIC_UUID,
            dataView
          );
          console.log('Data sent successfully via Bluetooth');
        } catch (error) {
          console.error('Failed to send data via Bluetooth:', error);
        }
      }
    } else {
      console.log("via Mqtt:");
      // selectMqtt가 true일 때 MQTT를 통한 데이터 송신
      this.publishMessage(JSON.stringify(data));
    }

  }

  //=========================================================================================
  // MQTT 연결 로직
connectToMQTT() {
  cordova.plugins.CordovaMqTTPlugin.connect({
    url: `tcp://${this.wifi.mqttBroker}`,
    port: 1883,
    clientId: "YOUR_USER_ID_LESS_THAN_24_CHARS",
    connectionTimeout: 3000,
    willTopicConfig: {
      qos: 0,
      retain: true,
      topic: this.wifi.outTopic,
      payload: "<will topic message>" //연결됐을때 기기한테 보내는값으로 이걸로 로직구현가능

    },
    keepAlive: 60,
    isBinaryPayload: false,
    success: (s: any) => {
        console.log("connect success MQTT");
        console.log("mqtt broker",this.wifi.mqttBroker);
        console.log("outpic",this.wifi.outTopic);
        console.log("inpic",this.wifi.inTopic);
        console.log("email",this.wifi.email);
        this.wifi.isConnectedMqtt = true;

        // MQTT에 연결된 후에 구독 및 메시지 수신 리스너 설정
        this.subscribeToTopic();
        this.setMessageListener();
      },
      error: function(e: any) {
        console.log("connect error MQTT");
        this.isConnectedMqtt = false;
    },
    onConnectionLost: function() {
        console.log("disconnect MQTT");
        this.isConnectedMqtt = false;
        this.reconnectMQTT(); // 재연결 로직 호출
    },
  });
}


  reconnectMQTT() {
    if (!this.wifi.isConnectedMqtt) {
      console.log("MQTT Reconnecting...");
      this.connectToMQTT();
    }
  }

  ngOnDestroy(): void {
    cordova.plugins.CordovaMqTTPlugin.disconnect({
      success: () => {
        console.log("MQTT Disconnected");
      },
      error: (error: any) => {
        console.error("MQTT Disconnect Error", error);
      }
    });
  }

  setMessageListener() {
    cordova.plugins.CordovaMqTTPlugin.listen(this.wifi.inTopic, (payload: any, params: any) => {
      // 수신된 메시지를 처리합니다.
      try {
        this.processReceivedMessage(payload);
      } catch (e) {
        console.error('Error parsing received message:', e);
      }
      console.log(payload);
    });
  }

  subscribeToTopic() {
    cordova.plugins.CordovaMqTTPlugin.subscribe({
      topic: this.wifi.inTopic,
      qos: 0,
      success: function(s: any) {
        console.log("subscription success");
      },
      error: function(e: any) {
        console.log("subscription error");
      }
    });
  }

   // MQTT 메시지 전송
   sendMassage(message: String) {
    if(this.wifi.selectMqtt == true)
      this.publishMessage(message);
    else
      this.sendData(2);

  }

  publishMessage(message: String) {
    cordova.plugins.CordovaMqTTPlugin.publish({
      topic: this.wifi.outTopic,
      payload: message,
      qos: 0,
      retain: false,
      success: function(s: any) {
        console.log("publish success");
      },
      error: function(e: any) {
        console.log("publish error");
      }
    });
  }

  // MQTT 연결 상태를 주기적으로 확인하고 재연결을 시도하는 메서드
  startMQTTReconnect() {
    this.intervalId = setInterval(async () => {
      if (this.wifi.selectMqtt && !this.wifi.isConnectedMqtt) {
        try {
          await this.connectToMQTT();
          console.log('MQTT 재연결 시도');
        } catch (error) {
          console.error('MQTT 재연결 실패:', error);
        }
      }
    }, 2000); // 예: 2초마다 실행
  }

  // MQTT 재연결 로직을 중지하는 메서드
  stopMQTTReconnect() {
    if (this.intervalId) {
      clearInterval(this.intervalId);
      this.intervalId = null;
    }
  }

  // local storage 관리 프로그램 ================================================
  isLocalStorageAvailable(): boolean {
    try {
      const test = '__test__';
      localStorage.setItem(test, test);
      localStorage.removeItem(test);
      return true;
    } catch (e) {
      return false;
    }
  }

  saveMqttBrokerToLocalStorage() {
    if (this.isLocalStorageAvailable()) {
      localStorage.setItem('mqttBroker', this.wifi.mqttBroker);
      localStorage.setItem('email', this.wifi.email);
      localStorage.setItem('outTopic', this.wifi.outTopic);
      localStorage.setItem('inTopic', this.wifi.inTopic);
    } else {
      console.error('Local Storage is not available');
    }
  }

  loadMqttBrokerFromLocalStorage() {
    if (this.isLocalStorageAvailable()) {
      const mqttBroker = localStorage.getItem('mqttBroker');
      const email = localStorage.getItem('email');
      const outTopic = localStorage.getItem('outTopic');
      const inTopic = localStorage.getItem('inTopic');

      this.wifi.mqttBroker = mqttBroker ?? this.wifi.mqttBroker;
      this.wifi.email = email ?? this.wifi.email;
      this.wifi.outTopic = outTopic ?? this.wifi.outTopic;
      this.wifi.inTopic = inTopic ?? this.wifi.inTopic;
    } else {
      console.error('Local Storage is not available');
    }
  }

  loadWifiSettingsFromLocalStorage() {
    if (this.isLocalStorageAvailable()) {
      const mqttBroker = localStorage.getItem('mqttBroker');
      const email = localStorage.getItem('email');
      const outTopic = localStorage.getItem('outTopic');
      const inTopic = localStorage.getItem('inTopic');

      if (mqttBroker) this.wifi.mqttBroker = mqttBroker;
      if (email) this.wifi.email = email;
      if (outTopic) this.wifi.outTopic = outTopic;
      if (inTopic) this.wifi.inTopic = inTopic;
    } else {
      console.error('Local Storage is not available');
    }
  }
  saveMessageLogToLocalStorage(messageLog: string[]) {
    localStorage.setItem('messageLog', JSON.stringify(messageLog));
  }
  saveWifiSettingsToLocalStorage() {
    if (this.isLocalStorageAvailable()) {
      localStorage.setItem('mqttBroker', this.wifi.mqttBroker);
      localStorage.setItem('email', this.wifi.email);
      localStorage.setItem('outTopic', this.wifi.outTopic);
      localStorage.setItem('inTopic', this.wifi.inTopic);
    } else {
      console.error('Local Storage is not available');
    }
  }

// GlobalService 클래스 내부
loadMessageLogFromLocalStorage(): string[] {
  const storedLog = localStorage.getItem('messageLog');
  return storedLog ? JSON.parse(storedLog) : [];
}
loadSelectMqttFromLocalStorage() {
  if (this.isLocalStorageAvailable()) {
    const selectMqtt = localStorage.getItem('selectMqtt');
    this.wifi.selectMqtt = selectMqtt === 'true';
  } else {
    console.error('Local Storage is not available');
  }
}

saveSelectMqttToLocalStorage() {
  if (this.isLocalStorageAvailable()) {
    localStorage.setItem('selectMqtt', this.wifi.selectMqtt.toString());
  } else {
    console.error('Local Storage is not available');
  }
}



}
