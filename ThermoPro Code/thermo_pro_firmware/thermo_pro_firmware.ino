
//Final PCB pins

//buttons
#define POWER_BUTTON 2
#define LOCK_BUTTON 3
#define UNITS_BUTTON 3 //CHANGE TO 4 FOR FINAL PCB
#define CAL_BUTTON 7
#define BACKLIGHT_BUTTON 12


//#define LED_BACKLIGHT 10
#define LED_BACKLIGHT 4 //*** TESTING ONLY, WILL NOT WORK ON FINAL PCB! ***

//temperature inputs
#define POT_TEMP_INPUT A0
#define DIGITAL_TEMP_INPUT 13

//Timer Rollover Values
//Timer 1
#define TIMER_COMPARE_3S 46875
#define TIMER_COMPARE_1S 15625
//Timer 2
#define TIMER_COMPARE_250HZ 250

//unstored for storeCounterValue is defined as -2 (explained below)
#define UNSTORED -2

//global flags and counters for interrupt routines
volatile char gPowerOnFlag = 0;
volatile char gLockFlag = 0;
volatile int g3sCounter = 0;
volatile int g250HzCounter = 0;
volatile int g1sCounter = 0;
//Button states
volatile char gPowerOn = 0;
volatile char gBacklightOn = 0;
volatile char gUnitButtonPressed = 0;

//State machine variables
enum SM_STATES {
  IDLE_STATE,
  UNITS_PRESS_STATE,
  UNITS_CHANGE_STATE
};

enum SM_STATES SM_State = IDLE_STATE;
//0 -> F, 1 -> C
volatile int gUnits = 0;

//Counters
volatile int risingEdgeCount = 0;

int storeCounterValue = UNSTORED;


void setup() {
  Serial.begin(9600);

  pinMode(POWER_BUTTON, INPUT);
  pinMode(LOCK_BUTTON, INPUT);
  //pinMode(UNITS_BUTTON, INPUT);
  pinMode(CAL_BUTTON, INPUT);
  pinMode(BACKLIGHT_BUTTON,INPUT);
  pinMode(LED_BACKLIGHT, OUTPUT);
  pinMode(POT_TEMP_INPUT, INPUT);
  pinMode(DIGITAL_TEMP_INPUT,INPUT);

  pinMode(LED_BACKLIGHT, OUTPUT);

  //likely more pins to follow

  //attaching interrupts

  // Interrupts and Timers
  noInterrupts();
  
  //TIMERS

  //TIMER 1 (3s)
  TCCR1A = 0;
  TCCR1B = 0;
  
  //Timer rolls over at 46875 (happens every three seconds, since it counts 15625 every second)
  OCR1A = TIMER_COMPARE_3S;

  //Timer rolls over at 15625 (happens every second)
  //OCR1A = TIMER_COMPARE_1S;

  //CTC Mode (Compare to OCR1A)
  TCCR1B |= (1 << WGM12); 
  //Prescales by 1024 (now runs at 16M/1024 = 15,625 Hz)
  TCCR1B |= (1 << CS10) | (1 << CS12);
  
  //TIMER 2 (250 Hz --> 1s (handled in ISR))
  TCCR2A = 0;
  TCCR2B = 0;

  OCR2A = TIMER_COMPARE_250HZ;
  //CTC Mode (Compare to OCR2A)
  TCCR2A |= (1 << WGM21);
  //Prescales by 256 (now runs at 16M/256 = 62,500 Hz)
  //TCCR2B |= (1 << CS21) | (1 << CS22); ##NO NO NO THIS IS THE ONE SECOND TIMER DO THE OTHER ONE FOR THE UNITS BUTTON FIX THIS
  //Timer will start when units button is pressed
  
  //INTERRUPTS
  
  //Set interrupt when timer = OCR1A (aka when it rolls over)
  TIMSK1 |= (1 << OCIE1A);

  //Set external interrupts for buttons
  EICRA |= 0x15; //Rising edge for both INT0 and INT1, EICRA = 0b1111
  //Power button interrupt (pin 2)
  EIMSK |= (1 << INT0);
  //Lock button interrupt (pin 3)
  EIMSK |= (1 << INT1);

  interrupts();
}

void loop() 
{
  //Power on/off sequence
  if(gPowerOnFlag) 
  {
    gPowerOn = !gPowerOn;
    if(gPowerOn)
    {
      digitalWrite(4,HIGH);
    }
    else
    {
      digitalWrite(4,LOW);
    }
    Serial.println(risingEdgeCount);
    gPowerOnFlag = 0;
  }
  //Startup loop
  
  if(gPowerOn) 
  {
    switch(SM_State) {
      case IDLE_STATE:
        //display temperature

        //check buttons
        checkButtons(); 
        break;
      case UNITS_PRESS_STATE:
        //display temperature
        
        //start timer
        startTimer2();
        if(g1sCounter)
        {
          //When
          TCCR2B = 0;
        }
        break;
      case UNITS_CHANGE_STATE:
        //
        break;
    }
    //Start I2C Comms ## maybe this goes in setup


    //Enable OLED Screen

    //lock/unlock sequence ## i think this is handled in the ISR but i could be wrong

    
    //Enable backlight button

    /*if(digitalRead(BACKLIGHT_BUTTON)) 
    {
      gBacklightOn = !gBacklightOn;
    }
    if(!gBacklightOn) 
    {
      digitalWrite(LED_BACKLIGHT, HIGH);
    }
    else 
    {
      digitalWrite(LED_BACKLIGHT, LOW);
    }
    */
    //Enable Units button

    
    if(digitalRead(UNITS_BUTTON)) 
    {
      //if the units button is pressed, grabs current 3s counter value. Resets to UNSTORED if at any point the button stops being pressed
      if(storeCounterValue == UNSTORED)
      {
        storeCounterValue = g3sCounter;
      }
      //if the button is still pressed (storeCounterValue doesn't reset), checks if 3 seconds have passed
      if(g3sCounter+UNSTORED+1 == storeCounterValue)
      {
        gUnits = !gUnits;
        g3sCounter = 0;
        Serial.println(gUnits);
      }
    }  else
    {
      //unstored is defined as -2 to prevent false positive when g3sCounter = 0
      storeCounterValue = UNSTORED;
    }
    

  }
 delay(125); //Debounce attempt TMP
}

void checkButtons() {
  //Power button
  if(gPowerOnFlag) 
  {
    gPowerOn = !gPowerOn;
    digitalWrite(POWER_BUTTON, gPowerOn);
    Serial.println(risingEdgeCount);
    gPowerOnFlag = 0;
  }
  //Units button
  if(digitalRead(UNITS_BUTTON))
  {
    SM_State = UNITS_PRESS_STATE;
  }
}

void startTimer2() {
  TCCR2B |= (1 << CS21) | (1 << CS22);
}
/*******************
 Interrupt Routines
*******************/
//Power button
ISR(INT0_vect) {
  gPowerOnFlag = 1;
  risingEdgeCount++;
}
//Lock button (starts/stops TIMER2)
ISR(INT1_vect) {
  //Flips locked state
  gLockFlag = !gLockFlag;
  if(gLockFlag) 
  {
    //Stops timer
    TCCR2B = 0;
  } else {
    //Starts timer with 62500 Hz
    TCCR2B |= (1 << CS21);
    TCCR2B |= (1 << CS22);
  }
}
//3 second timer
ISR(TIMER1_COMPA_vect) {
  g3sCounter++;
}
//250Hz timer / 1 second timer
ISR(TIMER2_COMPA_vect) {
  g250HzCounter++;
  if(g250HzCounter == 250) {
    g1sCounter++;
    g250HzCounter = 0;
  }
}