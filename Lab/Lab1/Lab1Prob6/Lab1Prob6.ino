#define GLED 4
#define RLED 5
#define BLED 3
#define BUTTON 11

void reset() {
  digitalWrite(RLED,LOW);
  digitalWrite(GLED,LOW);
  digitalWrite(BLED,LOW);
  delay(1000);

}
void setup() {
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(BUTTON,INPUT);
  
}
void loop() {
  while(digitalRead(BUTTON)==HIGH) {
    digitalWrite(RLED,HIGH);
    reset();
    digitalWrite(RLED,HIGH);
    digitalWrite(GLED,HIGH);
    reset();
    digitalWrite(RLED,HIGH);
    digitalWrite(GLED,HIGH);
    digitalWrite(BLED,HIGH);
    reset();
    digitalWrite(GLED,HIGH);
    digitalWrite(BLED,HIGH);
    reset();
    digitalWrite(BLED,HIGH);
    reset();
    digitalWrite(RLED,HIGH);
    digitalWrite(BLED,HIGH);
    reset();
  }

}