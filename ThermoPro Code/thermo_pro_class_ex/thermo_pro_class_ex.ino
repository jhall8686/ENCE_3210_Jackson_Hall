#define BUTTON1 2
#define BUTTON2 3
#define GLED 4
#define RLED 5

#define TIMER1_COMPARE_VALUE 62500

volatile char gFlag0 = 0;
void setup() 
{
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  noInterrupts();

  //Timer 1 init (16bit)
  //Prescale 16MHz/256
  
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = TIMER1_COMPARE_VALUE;
  TCCR1B |= (1 << WGM12);//CTC Mode
  TCCR1B |= (1 << CS12);//Prescaler

  TIMSK1  |= (1 << OCIE1A); //Enables interrupt when TIMER = OCR1A
  interrupts();
}

void loop() 
{

}


/***********************
ISR FUNCTIONS
***********************/

ISR(TIMER1_COMPA_vect) 
{
  gFlag0 = !gFlag0;
  digitalWrite(LED_GREEN, gFlag0);
}