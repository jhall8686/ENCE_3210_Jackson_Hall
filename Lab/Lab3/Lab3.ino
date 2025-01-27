//I/O pins
#define BUTTON 2
#define GLED 4
#define RLED 5

//Timer compare values
#define TIMER_COMPARE_1HZ 62500;
#define TIMER_COMPARE_250HZ 250;

//ISR counters at respective frequencies
static volatile uint32_t gCounter1 = 0;
static volatile uint32_t gCounter250 = 0;

//Flag for button press/main task
static volatile int buttonPressedFlag = 0;
static volatile int mainTaskFlag = 0;
void setup() {
  //I/O setup
  Serial.begin(9600);
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(BUTTON, INPUT);

  noInterrupts();
  // TIMERS

  // Set up Timer 0 (8 bit) to rollover at 250 Hz (scaled to 10 Hz in an interrupt later)
  TCCR0A = 0;
  TCCR0B = 0;

  OCR0A = TIMER_COMPARE_250HZ //Sets timer to rollover at 250 --> Rollover frequency 250 Hz
  TCCR0B |= (1 << CS02); //Prescales the clock by 256 (62500 Hz)
  TCCR0A |= (1 << WGM01); //sets timer to CTC (Resets after reaching OCR0A)

  // Set up Timer 1 (16 bit) to rollover at 1 Hz
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = TIMER_COMPARE_1HZ; //Sets timer to rollover at 62500 --> Rollover frequency 1 Hz
  TCCR1B |= (1 << WGM12); //sets timer to CTC (Resets after reaching OCR1A)
  TCCR1B |= (1 << CS12); //Prescales the clock by 256 (62500 Hz)

  //INTERRUPTS

  TIMSK0 |= (1 << OCIE0A); //Enables interrupt when Timer 0 == OCR0A
  TIMSK1 |= (1 << OCIE1A); //Enables interrupt when Timer 1 == OCR1A
  interrupts();
}

void loop() {
  //Turns on the red LED whenever the button is flagged, otherwise turns the LED off if it's on
  if(buttonPressedFlag) {
    digitalWrite(RLED, HIGH);
  } else if(digitalRead(RLED)) {
    digitalWrite(RLED, LOW);
  } else;
  
  //Main Task-- Prints "It's been three seconds!" Every three seconds
  if(mainTaskFlag) {
    Serial.println("It's been three seconds!");
    mainTaskFlag = 0;
  }
}

//Runs Task 2-- Sets flag every 100 ms if button is pressed
ISR(TIMER0_COMPA_vect) {
  gCounter250++;
  // mod changes frequency from 250 Hz to 10 Hz
  if(gCounter250 % 25 == 0) {
    //Every 100ms (10 Hz) checks if button is pressed and changes red LED accordingly
    if(digitalRead(BUTTON)) {
      buttonPressedFlag = 1;
    } else {
      buttonPressedFlag = 0;
    }
  }
}

//Runs Task 1-- Blinks green LED at 1 Hz (doesn't use a flag so that loop() isn't constantly setting the pin)
ISR(TIMER1_COMPA_vect) {
  gCounter1++;
  //On 1s, Off 1s, On 1s, ...
  if(gCounter1 % 2 == 1) {
    digitalWrite(GLED, HIGH);
  } else {
    digitalWrite(GLED, LOW);
  }
  if(gCounter1 % 3 == 0) {
    mainTaskFlag = 1;
  }
}