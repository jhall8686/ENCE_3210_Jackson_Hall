#define BUTTON1 2
#define BUTTON2 3
#define RLED 4
#define GLED 5
#define BLED 6

//defining counts
volatile unsigned int count1 = 0;
volatile unsigned int count2 = 0;

void setup() {
  //Setting pinModes
  pinMode(RLED,OUTPUT);
  pinMode(GLED,OUTPUT);
  pinMode(BLED,OUTPUT);

  //Mechanical buttons require a debouncer to avoid multiple rising triggers
  attachInterrupt(digitalPinToInterrupt(BUTTON1),ISR_button1Press,RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2),ISR_button2Press,RISING);
  Serial.begin(9600);
}23.

void loop() {
  //monitor count1 and count2 in Serial monitor
  Serial.println(count1);
  Serial.println(count2);
  //LED Setup
  if(count1 > count2) {
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, LOW);
  } else if (count1 < count2) {
    digitalWrite(GLED, HIGH);
    digitalWrite(RLED, LOW);
    digitalWrite(BLED, LOW);
  } else {
    digitalWrite(BLED, HIGH);
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
  }
}

//ISR Routines
void ISR_button1Press() {
  count1++;
}
void ISR_button2Press() {
  count2++;
}