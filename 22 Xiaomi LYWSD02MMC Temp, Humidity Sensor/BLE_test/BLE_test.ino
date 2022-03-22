/*코드 수정 예정 (이재진)
 * 제가 아는 바로는 온도/습도/배터리에 대한 내용 중 하나만 랜덤으로 패킷을 보내는데
 * 이렇게 되면 셋 중 하나가 들어올 때까지 계속 블루투스 기기에 접속하면서 기다릴 수밖에 없습니다.
 * 그래서 데이터를 받아오는 데 짧으면 5~6초, 길면 2분 이상이 걸릴 수도 있습니다.
 * 온도, 습도는 급격하게 빨리 변하는 수치는 아닙니다.
 * 패킷 내용이 16진수로 들어오는데 인터넷에 관련 자료가 없어서 일부만 해석했고 나머지는 해석중입니다.
 */

//////////////////////////////////////////////////
#include "SPI.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_system.h"
#include <sstream>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SCAN_TIME  10 // seconds


float humid;
float temper;

////////////////////////////////////////////////////

BLEScan *pBLEScan;

void IRAM_ATTR resetModule(){
    ets_printf("reboot\n");
    esp_restart();
}
         
float  current_humidity = -100;
float  previous_humidity = -100;
float current_temperature = -100;
float previous_temperature = -100;
bool isCollected = false; //is temp collected?

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        if (advertisedDevice.getAddress().toString() == "17:75:bc:11:28:99") { 
          //17:75:bc:11:28:99은 제 기기의 mac address여서 nRF connect나 Xiaomi Home으로 먼저 기기에 연결하시고 가지고 계신 기기의 mac address로 바꿔주셔야 합니다.
            int serviceDataCount = advertisedDevice.getServiceDataCount();
            std::string strServiceData = advertisedDevice.getServiceData(0);
            //std::string serviceJaejin[3];
            //Serial.print("Number of ServiceData :");
            //Serial.println(serviceDataCount);
            //Serial.print("How Data looks like :");
            /*for(int i=0; i<strServiceData.length(); i++){
                Serial.print((int)strServiceData[i]);
                Serial.print(" ");
            }
            
            Serial.println();*/
            uint8_t cServiceData[100];
            char charServiceData[100];

            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);

            //Serial.printf("\n\nAdvertised Device: %s\n", advertisedDevice.toString().c_str());

            for (int i=0;i<strServiceData.length();i++) {
                sprintf(&charServiceData[i*2], "%02x", cServiceData[i]);
            }

            unsigned long value;
            char charValue[5] = {0,};
            float temp_stored, hum_stored;
            int sl = strServiceData.length();
            if(sl == 17){//패킷의 정보를 담고 있는 문자열의 길이가 17이면서 마지막 원소 값이 0이면 온도, 1이면 습도입니다.
              //추가로 cServiceData[14] == 1이면 배터리에 대한 정보가 cServiceData[15]에 저장됩니다.
              //cServiceData[14]== 2이면 온도 아니면 습도에 대한 정보입니다.
              sprintf(charValue, "%02X%02X", cServiceData[16], cServiceData[15]);
              value = strtol(charValue, 0, 16);
              Serial.print("Info of the packet incoming : ");
              for(int i=0; i<strServiceData.length(); i++){
                Serial.print((int)strServiceData[i]);
                Serial.print(" ");
              }
              Serial.println();
              if(isCollected == false && (int)strServiceData[sl-1] == 0){
                Serial.print("Temp : ");
                temper = (float)value/10;
                Serial.println(temper);
                isCollected = true;
              }
              if(isCollected == true && (int)strServiceData[sl-1] != 0){
                Serial.print("Humidity :");
                humid = (float)value/10;
                Serial.println(humid);
                Serial.println("-----------------end of sample------------------");
                isCollected = false;
              }
            }
            std::stringstream ss;
            ss << "fe95" << charServiceData;
            
            //Serial.print("Payload:");
            //Serial.println(ss.str().c_str());
          
        }
    }
};

void setup() {
  
  Serial.begin(115200);
  Serial.println("ESP32 XIAOMI DISPLAY");
  initBluetooth();
}

void loop() {
  
    char printLog[256];
    BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
    delay(100);
}


void initBluetooth()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(0x50);
    pBLEScan->setWindow(0x30);
}

String convertFloatToString(float f)
{
  String s = String(f,1);
  return s;
}

float CelciusToFahrenheit(float Celsius)
{
 float Fahrenheit=0;
 Fahrenheit = Celsius * 9/5 + 32;
 return Fahrenheit;
}
