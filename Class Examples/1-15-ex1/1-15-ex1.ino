#define GLED 4
#define RLED 5

#define TIMER_COMPARE_VALUE 62500
#define COUNTER_TIME 1000
#define LED_TIME 100
unsigned long gCounter = 0;
volatile unsigned char gISRFlag1 = 0;
void setup() {
  Serial.begin(9600);

  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  //Initialize Timer1 (16bit)
  //
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = TIMER_COMPARE_VALUE; //compare match register 16 MHz/256
  TCCR1B |= (1<<WGM12); //CTC Mode
  //Start timer by setting the prescaler
  TCCR1B |= (1<<CS12); //256 prescaler
  TIMSK1 |= (1<<OCIE1A); // enable timer compare interrupt
  interrupts();
}

void loop() {
  
  digitalWrite(GLED, HIGH);
  digitalWrite(RLED, LOW);
  delay(LED_TIME);
    
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, HIGH);
  if(gISRFlag1) {
    gISRFlag1 = 0;
    
    gCounter++;
    Serial.print(F("Counter: "));
    Serial.println(gCounter);
    delay(COUNTER_TIME);
  }
}

ISR(TIMER1_COMPA_vect){
    gISRFlag1 = 1;
  }
