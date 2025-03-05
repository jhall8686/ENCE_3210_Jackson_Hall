#define LED_PIN 10
#define ON_OFF_BUTTON 2
#define SENSE_BUTTON 3
#define PWM_OUTPUT_PIN 11

#define COMPARE_250HZ 250
#define COMPARE_20HZ 3125

volatile uint8_t gOnOffFlag = 0;
volatile uint8_t gSenseFlag = 0;
volatile uint8_t gTimerCountFlag = 0;
volatile uint8_t gFullArrayFlag = 0;

volatile uint8_t sampleCount = 0;

uint8_t pwmCompare = 125;
uint8_t timer1Running = 0;
uint16_t avg;

volatile uint16_t digitalTemp[100];

void setup() {
  pinMode(ON_OFF_BUTTON, INPUT);
  pinMode(SENSE_BUTTON, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_OUTPUT_PIN, OUTPUT);

  noInterrupts();
  //ADC Setup
  //Automatically uses external Vref at AREF pin
  //Also automatically uses pin A0 as Vin
  ADMUX = 0;
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN); //Enable ADC
  //ADCSRA |= (1 << ADSC); to start the conversion; 
    //Triggers on Timer 2 Compare Interrupt
  
  ADCSRA |= (1 << ADIE); // Enable ADC interrupt

  //TIMERS
  //Timer 1 -- 20 Hz Interrupt Trigger
  TCCR1A = 0;
  TCCR1B = 0;
  
  OCR1A = COMPARE_20HZ;
  TCCR1B |= (1 << WGM12); //CTC Mode
  //TCCR1B |= (1 << CS12); // Prescale by 256
  
  //Timer 2 -- Fast PWM Generator at 250 Hz
  TCCR2A = 0;
  TCCR2B = 0;

  OCR2A = COMPARE_250HZ; //TOP Value for timer 2
  OCR2B = pwmCompare; //set trigger for PWM
  TCCR2A |= (1 << COM2B1) | (1 << COM2B0);//Generate inverted PWM Output; sets OC2B register when TCNT2 == OCR2B, clears when Timer 2 resets to 0x00
  TCCR2A |= (1 << WGM21) | (1 << WGM20); TCCR2B |= (1 << WGM22); //Set WGM22:0 to 7, enabling Fast PWM mode that TOPs at OCR2A
  TCCR2B |= (1 << CS22) | (1 << CS21); // Prescale by 256


  //Timer 1 Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);

  //Set external interrupts for buttons
  EICRA |= 0xF; //Rising edge for both INT0 and INT1, EICRA = 0b1111
  //Power button interrupt (pin 2)
  EIMSK |= (1 << INT0);
  //Lock button interrupt (pin 3)
  EIMSK |= (1 << INT1);
  interrupts();
}

void loop() {
  if(gOnOffFlag) {
    if(!timer1Running) startTimer1(); //starts ADC Sampling

    /*****
    Here, could add display functions, etc. 

    *****/

    //handles full array flag and computes the average, while resetting the array (should happen every 5 seconds)
    if(gFullArrayFlag) {
      uint16_t sum = 0;
      for(int i = 0; i < 100; i++) {
        sum += digitalTemp[i];
        digitalTemp[i] = 0;
      }

      //computes the average and stores it on pwmCompare, then scales based on sensitivity selected
      switch(gSenseFlag) {
      case 0: //full speed
        pwmCompare = sum / 100 / 4; //0-255
        break;
      case 1: //half speed
        pwmCompare = sum / 100 / 8; //0-127
        break;
      case 2: //quarter speed
        pwmCompare = sum / 100 / 16; //0-63
        break;
      default:
        break;
      }
      //sets new PWM compare value
      OCR2B = pwmCompare;
      //resets flags so ADC can start sampling and storing again
      sampleCount = 0;
      gFullArrayFlag = 0;
      
    }
  }
}

void startTimer1() {
  TCCR1B |= (1 << CS12);
  timer1Running = 1;
}
void stopTimer1() {
  TCCR1B &= ~(1 << CS12);
  timer1Running = 0;
}

ISR(INT0_vect) {
  gOnOffFlag = !gOnOffFlag;
}

ISR(INT1_vect) {
  gSenseFlag++;
  if(gSenseFlag > 2) gSenseFlag = 0;
}

ISR(TIMER1_COMPA_vect) {
  ADCSRA |= (1 << ADSC);  
}


ISR(ADC_vect) {
  if(!gFullArrayFlag) {
    digitalTemp[sampleCount] = ADCL + (ADCH << 8); // stores the 10 bit digital conversion
    sampleCount++;
    //prevents overwriting values or an index error w/ the above
    if(sampleCount == 100) {
      gFullArrayFlag = 1;
    }
  }
}
