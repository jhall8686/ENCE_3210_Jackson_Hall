#define RLED 4
#define GLED 5
#define BLED 6
#define BUTTON 2

volatile int flag_buttonChange = 0;
void reset() {
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, LOW);
  digitalWrite(BLED, LOW);
}

void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON),ISR_buttonChange, CHANGE);
  Serial.begin(9600);

}
void loop() {
  Serial.println(digitalRead(BUTTON));
  if(flag_buttonChange) {
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

void ISR_buttonChange() {
  if(digitalRead(BUTTON)) {
    flag_buttonChange = 1;
  } else {
    flag_buttonChange = 0;
  }
}
