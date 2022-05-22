#include "HX711.h" 
 
#define LOADCELL_DOUT_PIN D5
#define LOADCELL_SCK_PIN D6
 
HX711 scale;
 
float calibration_factor = 450000;//or 450

void measure(){
  scale.set_scale(calibration_factor);
  Serial.print("Reading: ");
  float reading = scale.get_units();
  Serial.print(reading,10);
  Serial.print(" kg"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
}

void setup() 
{
  Serial.begin(9600);
  Serial.println("Setting HX711 calibration factor");
  delay(500);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
}
 
void loop() 
{
  measure();
  if(Serial.available())
  {
    char temp = Serial.read();
 
    switch(temp)
    {
      case '1':
        calibration_factor += 10;
        break;
      case '2':
        calibration_factor += 50;
        break;
      case '3':
        calibration_factor += 100;
        break;
      case '4':
        calibration_factor += 1000;
        break;
 
      case 'a':
        calibration_factor -= 10;
        break;
      case 's':
        calibration_factor -= 50;
        break;
      case 'd':
        calibration_factor -= 100;
        break;
      case 'f':
        calibration_factor -= 1000;
        break;
    }
  }

}
