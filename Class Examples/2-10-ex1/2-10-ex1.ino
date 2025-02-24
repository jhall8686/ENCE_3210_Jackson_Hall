#define POT_1 A0
#define TIME_DELAY 100 // ms
#define TIMER_COMPARE_VALUE 62500

volatile char gFlag = 0;
int gPot_1 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(POT_1,INPUT);

  //Configure the ADC
  //Use AVCC as the reference voltage | MUX3..0 = 0 for ADC0
  ADMUX |= (1<<REFS0);

  noInterrupts();
  //Trigger Source
  ADCSRB = |= (1<<ADTS2) | (1<<ADTS0); //Timer/Counter 1 compare match B

  //Configure Timer1
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1B = TIMER_COMPARE_VALUE;

  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS12);
  TIMSK1 |= (1<<OCIE1B);

  ADCSRA |= (1<<ADATE);

  ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADSC);

  interrupts();


  //Enable ADC ISR (ADIE) and Start Conversion (ADSC)
  ADCSRA |= (1<<ADIE) | (1<<ADSC);

}

void loop() {
  if(gFlag) {
    gFlag = 0;
    Serial.print("Val 1: ");
    Serial.println(gPot_1);
  }
}
ISR(ADC_vect) {
  gFlag = 1;
  gPot_1 = ADC;

}

EMPTY_INTERRUPT(TIMER1_COMPB_vect);