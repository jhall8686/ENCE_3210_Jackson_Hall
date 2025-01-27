#define BUTTON 11

// Length 10 Array (Digital Signal)
int x[10] = {1,5,1,5,1,5,1,5,1,5};
int y[10] = {};
//Interrupt Flag for Button Press
static volatile int gButtonPressFlag = 0;
void setup() {
  pinMode(BUTTON, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON), ISR_button1, RISING);
}

void loop() {
  // populates y w/ y[n] = 2*x[n]-x[n-1]
  if(gButtonPressFlag) {
    //y[0] automatically defined to avoid a definition based on x[-1]
    y[0] = 0;
    for(int n = 1; n < 10; n++) {
      y[n] = 2*x[n]-x[n-1];
    }
    gButtonPressFlag = 0;
  }
}

void ISR_button1() {
  gButtonPressFlag = 1;
}