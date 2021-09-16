int S = A0; // Analog Input
int d2=D2;  //Digital Input

int sound=0,soundPre=0;

unsigned long previousMillis = 0;     
const long interval = 1000;  

void setup()
{
  Serial.begin(115200); // 시리얼 통신 초기화
  pinMode(S, INPUT);
  pinMode(d2, INPUT);
}

void loop()
{
  doTick();
}

//1초 마다 실행되는 시간함수
void doTick() {
  sound=digitalRead(d2);
  if(sound==0 && soundPre==0) {  
   Serial.println(analogRead(S));
   soundPre=1;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //tick();
    soundPre=0;
  }  
}
