//tab1.page.ts
import { Component, ChangeDetectorRef, OnDestroy  } from '@angular/core';
import { Subscription } from 'rxjs';
import { GlobalService, DataWifiMqtt } from '../global.service';
import { LoadingController, AlertController } from '@ionic/angular';

@Component({
  selector: 'app-tab1',
  templateUrl: 'tab1.page.html',
  styleUrls: ['tab1.page.scss']
})
export class Tab1Page implements OnDestroy  {
  loading: any;
  connectedDeviceName: string = 'i2r-03-IoT PLC'; // 초기값을 빈 문자열로 설정
  customMqttBroker:string="";
  showWifiForm: boolean = false; // WiFi 폼 표시 여부
  ssid: string = ''; // SSID 정보
  private isButtonClicked: boolean = false;
  private debounceTime: number = 5000; // 5초 동안 버튼 클릭 무시

  //constructor(public globalService: GlobalService) {}
  private subscription: Subscription;

  constructor(
    public loadingController: LoadingController,
    public alertController: AlertController,
    public globalService: GlobalService,
    private changeDetectorRef: ChangeDetectorRef,
    ) {
    this.subscription = this.globalService.wifiData$.subscribe(() => {
      this.changeDetectorRef.detectChanges();
    });
  // Attempt to automatically reconnect to the previously connected device
  this.globalService.attemptAutoReconnect();
  }
  ngOnDestroy() {
    this.subscription.unsubscribe();
  }
  ngOnInit() {
    this.checkForReconnection();
  }
  private checkForReconnection() {
    if (this.globalService.ble.isConnected) {
      this.connectedDeviceName = this.globalService.ble.connectedDevice?.name || 'Reconnecting...';
    }
  }
// 장치 이름에서 숫자를 추출하고 해당하는 이미지 URL을 반환하는 함수
getDeviceImageUrl(deviceName: string): string {
  const deviceIdMatch = deviceName.match(/i2r-(\d+)-IoT PLC/);
  if (deviceIdMatch && deviceIdMatch[1]) {
    const deviceId = deviceIdMatch[1];
    return `https://drive.google.com/uc?export=view&id=11F9LXP4eVqi7lHZiz1496k90ti5RBRxI`; //https://drive.google.com/uc?export=view&id=15BYu3Tmo_WqA4hw5iBGl1mtuoo82cY9t
  } else {
    return `https://drive.google.com/uc?export=view&id=11F9LXP4eVqi7lHZiz1496k90ti5RBRxI`; // 기본 이미지 URL을 입력하세요.

  }
}
async scanAndConnect() {
  await this.globalService.scanAndConnect();
  // 여기서 연결 결과에 따른 추가 로직을 구현할 수 있습니다.
  this.globalService.dev.isSliderOn = false;
  this.globalService.wifi.selectMqtt = false;
  // Update label classes

  this.globalService.wifi.bleLabelClass = 'text-blue';
  this.globalService.wifi.wifiLabelClass = 'text-gray';
}

  // GlobalService의 disconnect 메서드를 호출하는 래퍼 함수
  async disconnect() {
    await this.globalService.disconnect();
  }


  saveCustomBroker() {
    if (this.globalService.wifi.mqttBroker === 'custom') {
      this.globalService.wifi.mqttBroker = this.customMqttBroker;
    }
  }

  onMqttBrokerChange(event: any) {
    // MQTT Broker 선택이 변경될 때 호출될 함수
    // 필요한 로직을 추가할 수 있습니다.
    // 예를 들어, 'custom'을 선택했을 때 추가 로직을 수행합니다.
    if (this.globalService.wifi.mqttBroker === 'custom') {
      // 사용자가 직접 입력할 수 있도록 추가 필드를 표시합니다.
      // 이 부분은 자동으로 처리됩니다. HTML 템플릿의 *ngIf에 의해
    }
  }
  openNewTab() {
    const wifiInfoDiv = document.getElementById('wifiInfo');
    if (wifiInfoDiv) {
      wifiInfoDiv.classList.toggle('hidden');
    } else {
      console.error('Element with id "wifiInfo" not found');
    }
  }
  resetSavedDevice() {
    // 로컬 스토리지에서 connectedDeviceId 제거
    if (this.globalService.isLocalStorageAvailable()) {
      localStorage.removeItem('connectedDeviceId');
    }

    // 기타 초기화 로직 (예: 상태 초기화)
    // ...
  }
  saveAndSendWifiInfo() {
    this.presentLoading();
    // 직접 입력한 MQTT Broker 주소 처리
    // MQTT Broker가 'custom'일 경우, customMqttBroker 값을 mqttBroker에 할당합니다.
    if (this.globalService.wifi.mqttBroker === 'custom') {
      this.globalService.wifi.mqttBroker = this.customMqttBroker;
    }
    // 저장된 정보를 보드로 보내 저장하게 한다.
    this.globalService.sendData(1);
    // 로컬 스토리지에 mqttBroker 저장
    this.globalService.saveMqttBrokerToLocalStorage();
    this.subscribeToResponse();
  }

// IoT 장치로부터의 응답을 처리하는 메소드
subscribeToResponse() {
  // 저장 및 전송 버튼을 누른 시간 기록
  const startTime = new Date().getTime();

  // BLE 장치로부터 메시지 수신을 시작합니다.
  if (this.globalService.ble.connectedDevice) {
    this.globalService.startNotifications(this.globalService.ble.connectedDevice?.deviceId);
  }

  // 4초 후에 BLE 재연결 로직을 시작합니다.
  setTimeout(() => {
    // 7초 동안 BLE 재연결을 체크합니다.
    const intervalId = setInterval(() => {
      const currentTime = new Date().getTime();

      // 7초가 지났는지 확인합니다.
      if (currentTime - startTime > 15000) {
        clearInterval(intervalId); // 인터벌 중단
        if (!this.globalService.ble.isConnected) {
          // BLE가 여전히 연결되지 않았다면 경고 표시
          this.dismissLoading();
          this.presentAlert('BLE 재연결 실패. 설정을 확인하세요.');
        }
      } else if (this.globalService.ble.isConnected) {
        clearInterval(intervalId); // 인터벌 중단
        this.dismissLoading(); // 로딩창 닫기
        console.log('블루투스 재연결 성공:', this.globalService.ble.connectedDevice?.deviceId);
        this.presentSuccess('WiFi 연결 성공!');
      }
    }, 500); // 0.5초마다 체크
  }, 10000); // 4초 지연 후 시작
}

downloadFirmware() {
  if (this.isButtonClicked) {
    console.log("버튼 클릭 무시됨");
    return;
  }

  this.isButtonClicked = true;
  setTimeout(() => this.isButtonClicked = false, this.debounceTime);

  this.globalService.sendData(0);

  this.alertController.create({
    header: '펌웨어 다운로드',
    message: '펌웨어 다운로드가 시작되었습니다.',
    buttons: ['확인']
  }).then(alert => alert.present());
}


// 로딩 창 표시
async presentLoading() {
  this.loading = await this.loadingController.create({
    message: 'WiFi 정보 전송 중...'
  });
  await this.loading.present();
}

// 로딩 창 숨기기
async dismissLoading() {
  if (this.loading) {
    await this.loading.dismiss();
  }
}

// 에러 메시지 표시
async presentAlert(message: string) {
  const alert = await this.alertController.create({
    header: '에러',
    message: message,
    buttons: ['확인']
  });

  await alert.present();
}
// 에러 메시지 표시
async presentSuccess(message: string) {
  const alert = await this.alertController.create({
    header: '성공',
    message: message,
    buttons: ['확인']
  });

  await alert.present();
}
}
