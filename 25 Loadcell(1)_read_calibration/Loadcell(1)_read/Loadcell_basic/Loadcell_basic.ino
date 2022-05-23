#include "HX711.h"

const int LOADCELL_DOUT_PIN = D5; //로드셀 핀
const int LOADCELL_SCK_PIN = D6;  //로드셀 핀
long reading, counter = 0;

HX711 scale;

void measure(){
  reading = scale.read();
  Serial.println(reading);
}

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  measure();
  //delay(1000);
}
