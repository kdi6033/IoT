//tab2.page.ts
import { Component, ElementRef, ViewChild, NgZone, ChangeDetectorRef, OnDestroy, DoCheck, AfterViewInit } from '@angular/core';
import { Subscription } from 'rxjs';
import { GlobalService } from '../global.service';
import { LogMessagePage } from '../log-message/log-message.page';
import { Router } from '@angular/router';
import { ModalController,AlertController } from '@ionic/angular'; // Import ModalController here

interface Relay {
  isOn: boolean;
}
@Component({
  selector: 'app-tab2',
  templateUrl: 'tab2.page.html',
  styleUrls: ['tab2.page.scss']
})

export class Tab2Page implements OnDestroy, DoCheck, AfterViewInit {
  @ViewChild('knob') knobElement!: ElementRef; // '!'를 사용하여 definite assignment assertion 적용

  ngAfterViewInit() {
    this.setupKnob();
  }
  
  currentKnobAngle: number = 0; // 현재 노브의 각도를 저장할 변수
  private setupKnob() {
    let currentRotation = 0; // 현재 회전 각도
      
    const calculateRotation = (touchEvent: TouchEvent) => {
      const knobRect = this.knobElement.nativeElement.getBoundingClientRect();
      const centerX = knobRect.left + knobRect.width / 2;
      const centerY = knobRect.top + knobRect.height / 2;
      const touch = touchEvent.touches[0];
      const touchX = touch.clientX;
      const touchY = touch.clientY;
      const angle = Math.atan2(touchY - centerY, touchX - centerX) * (180 / Math.PI);
      return angle;
    };
  
    const updateKnobRotation = (angle: number) => {
      // 노브 요소의 스타일 업데이트
      this.knobElement.nativeElement.style.transform = `rotate(${angle}deg)`;
    };
  
    const getNearestValue = (angle: number) => {
      const normalizedAngle = (angle + 360) % 360;
      if (normalizedAngle >= 135 && normalizedAngle < 225) {
        return 2;
      } else if (normalizedAngle >= 45 && normalizedAngle < 135) {
        return 1;
      } else {
        return 0;
      }
    };
  
    this.knobElement.nativeElement.addEventListener('touchstart', (e: TouchEvent) => {
      e.preventDefault();
    });
  
    this.knobElement.nativeElement.addEventListener('touchmove', (e: TouchEvent) => {
      const angle = calculateRotation(e);
      currentRotation = angle;
      updateKnobRotation(currentRotation); // 여기에서 노브 회전 적용
      const value = getNearestValue(currentRotation);
      console.log('Selected Value:', value); // 선택된 값 출력
      e.preventDefault();
    });
  
    this.knobElement.nativeElement.addEventListener('touchmove', (e: TouchEvent) => {
      this.currentKnobAngle = currentRotation; // 현재 각도 저장
    });

  }
  
  

  bleLabelClass: string = 'text-blue';
  wifiLabelClass: string = 'text-green';
  relayStates = [true, true, true, true];
  relays: Relay[] = [
    // Initialize this with your default values
    { isOn: false },
    { isOn: false },
    { isOn: false },
    { isOn: false }
    // ... add more relays as needed
  ];

  // 전압 리프레쉬
  TempUpdated = false;
  HumiUpdated = false;
  // 로그
  showMessageLog = false; // Flag to show/hide the message log
  messageLog: string[] = []; // Array to store the log of messages
  leds: boolean[] = new Array(4).fill(false);
  private subscription: Subscription;
  private lastLoggedMessage: string = '';
  private formatTimestamp(date: Date): string {
    const hours = date.getHours();
    const minutes = date.getMinutes();
    const seconds = date.getSeconds();
    const year = date.getFullYear();
    const month = date.getMonth() + 1; // Month is 0-indexed
    const day = date.getDate();
    const ampm = hours >= 12 ? '오후' : '오전';
    const formattedTime = `${year}-${month.toString().padStart(2, '0')}-${day.toString().padStart(2, '0')} ${ampm} ${hours % 12}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    return formattedTime;
  }
  isWiFi: boolean = false; // Assuming this tracks the toggle state
  constructor(
    public globalService: GlobalService,
    private changeDetectorRef: ChangeDetectorRef,
    private ngZone: NgZone,
    private router: Router,
    private modalController: ModalController, // Add this line
    public alertController: AlertController,
  ) {
    // Load message log from local storage
    this.messageLog = this.globalService.loadMessageLogFromLocalStorage();
    // MQTT 연결 상태 구독
    this.globalService.mqttConnection$.subscribe(isConnected => {
      this.onMQTTConnectionChange();
    });
    this.globalService.wifiData$.subscribe(() => {
      this.changeDetectorRef.detectChanges();
    });
    this.subscription = this.globalService.devIn$.subscribe((newIn) => {
      this.leds = newIn.map(value => value > 0);
      this.changeDetectorRef.detectChanges();
    });
    // RxJS 구독
    this.globalService.wifiData$.subscribe(data => {
      this.ngZone.run(() => {
        // 데이터 처리
        this.changeDetectorRef.detectChanges();
      });
    });
    // Subscribe to vbat$ and adc$ observables
    this.globalService.temperature$.subscribe(() => {
      this.TempUpdated = true;
      setTimeout(() => this.TempUpdated = false, 1000); // Reset flag after 1 second
    });

    this.globalService.humidity$.subscribe(() => {
      this.HumiUpdated = true;
      setTimeout(() => this.HumiUpdated = false, 1000); // Reset flag after 1 second
    });
    // 앱 시작 시 MQTT 연결 상태 확인 및 연결 시도
    this.globalService.checkMQTTConnection();
  }
  navigateToLogMessage() {
    this.router.navigateByUrl('/log-message');
  }
  toggleMessageLog(): void {
    this.showMessageLog = !this.showMessageLog;
  }
  async openLogModal() {
    const modal = await this.modalController.create({
      component: LogMessagePage,
      componentProps: {
        messageLog: this.messageLog
      }
    });
    return await modal.present();
  }
  ngOnInit() {
    // MQTT 토픽 정보가 설정되어 있지만 BLE가 연결되지 않은 경우
    if (this.globalService.wifi.outTopic && !this.globalService.ble.isConnected) {
      this.globalService.dev.isSliderOn = true; // 토글 위치를 WiFi로 설정
      this.globalService.wifi.selectMqtt = true; // 실제 WiFi 선택 상태를 true로 설정
      this.bleLabelClass = 'text-gray';
      this.wifiLabelClass = 'text-green';
    } else{
      this.globalService.dev.isSliderOn = false; // 토글 위치를 BLE로 설정
      this.globalService.wifi.selectMqtt = false; // 실제 BLE 선택 상태를 true로 설정
      this.bleLabelClass = 'text-blue';
      this.wifiLabelClass = 'text-gray';
    }
  }
  ngDoCheck(): void {
    const currentMessage = this.globalService.receivedMessage;
    if (currentMessage && currentMessage !== this.lastLoggedMessage) {
      const timestamp = this.formatTimestamp(new Date());
      const logEntry = `${timestamp}: ${currentMessage}`;
      this.messageLog.push(logEntry);
      this.lastLoggedMessage = currentMessage;
      this.globalService.saveMessageLogToLocalStorage(this.messageLog);
    }
    // BLE 연결 상태 변경 감지
    if (this.globalService.ble.isConnected !== this.globalService.dev.isSliderOn) {
      this.updateSwitchState();
    }
  }
  ngOnDestroy() {
    this.subscription.unsubscribe();
  }
// Tab2Page에서 MQTT 상태에 따라 스위치 상태를 업데이트하는 메소드
updateSwitchState() {
  this.globalService.dev.isSliderOn = this.globalService.wifi.selectMqtt;
  // BLE 및 WiFi 라벨 클래스 업데이트
  this.bleLabelClass = this.globalService.dev.isSliderOn ? 'text-gray' : 'text-blue';
  this.wifiLabelClass = this.globalService.dev.isSliderOn ? 'text-green' : 'text-gray';
}
// MQTT 연결 상태 변경 시 호출되는 메소드
onMQTTConnectionChange() {
  this.updateSwitchState();
  if (this.globalService.wifi.selectMqtt) {
    // MQTT 연결 상태가 활성화되면 연결 시도
    this.globalService.checkMQTTConnection();
  }
}
toggleSlider(): void {
  this.globalService.dev.isSliderOn = !this.globalService.dev.isSliderOn;

  // Synchronize the slider state with the global service property
  this.globalService.wifi.selectMqtt = this.globalService.dev.isSliderOn;

  // Update label classes
  if (this.globalService.dev.isSliderOn) {
    this.bleLabelClass = 'text-gray';
    this.wifiLabelClass = 'text-green';
  } else {
    this.bleLabelClass = 'text-blue';
    this.wifiLabelClass = 'text-gray';
  }

  // Invoke toggleMqttSelect to handle the change
  this.toggleMqttSelect();
}
toggleRelay(event: MouseEvent, index: number): void {
  const button = event.target as Element; // Cast the target to Element

  const isOn = button.classList.contains('relay-on');
  const signal = button.nextElementSibling as HTMLElement;

  if (isOn) {
    button.classList.remove('relay-on');
    button.classList.add('relay-off');
    signal.classList.remove('signal-on');
    signal.classList.add('signal-off');
  } else {
    button.classList.remove('relay-off');
    button.classList.add('relay-on');
    signal.classList.remove('signal-off');
    signal.classList.add('signal-on');
  }

  this.relays[index].isOn = !this.relays[index].isOn;
  // Call sendDataToTab2 with the updated state
  this.sendDataToTab2(index, this.relays[index].isOn);
}
sendDataToTab2(no: number, value: boolean) {
  // GlobalService를 통해 Tab1Page의 sendData 함수를 호출합니다.
  this.globalService.dev.no = no;
  this.globalService.dev.value = value;
  this.globalService.sendData(2);
}


toggleMqttSelect() {
  console.log('와이파이 선택 상태:', this.globalService.wifi.selectMqtt);
  // 로컬 스토리지에 Wi-Fi 설정 저장
  //this.globalService.saveWifiSettingsToLocalStorage();
  this.globalService.loadMqttBrokerFromLocalStorage();
  console.log(this.globalService.wifi.mqttBroker);
  console.log(this.globalService.wifi.outTopic);
  console.log(this.globalService.wifi.inTopic);

  if (this.globalService.wifi.selectMqtt) {
    // MQTT가 활성화될 때
    this.globalService.startMQTTReconnect(); // 재연결 시작
  } else {
    // MQTT가 비활성화될 때
    this.globalService.stopMQTTReconnect(); // 재연결 중지
  }

  if(this.globalService.wifi.isConnectedMqtt)
    this.globalService.sendData(3);
  // 변경된 상태를 로컬 스토리지에 저장
  this.globalService.saveSelectMqttToLocalStorage();
}

get receivedMessage() {
  return this.globalService.receivedMessage;
}

}
