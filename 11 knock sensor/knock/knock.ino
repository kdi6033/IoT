int d2=D2;  //Digital Input
int d4=D4;  //Digital Output

int knock=0,knockPre=0;

unsigned long previousMillis = 0;     
const long interval = 1000;  

void setup()
{
  Serial.begin(115200); // 시리얼 통신 초기화
  pinMode(d2, INPUT);
  pinMode(d4, OUTPUT);
}

void loop()
{
  doTick();
}

//1초 마다 실행되는 시간함수
void doTick() {
  knock=digitalRead(d2);
  digitalWrite(d4,knock);
  if(knock==0 && knockPre==0) {
    Serial.println(knock);
    knockPre=1;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //tick();
    knockPre=0;
  }  
}
