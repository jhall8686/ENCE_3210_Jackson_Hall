#include <avr/wdt.h>

#define GLED 4
#define BUTTON 2
void setup() {
  pinMode(GLED, OUTPUT);
  pinMode(BUTTON, INPUT);
  wdt_disable();
  delay(3000);
  
  wdt_enable(WDTO_2S);
}

void loop() {
  //Blink LED
  digitalWrite(GLED, HIGH);
  delay(100);
  digitalWrite(GLED, LOW);
  delay(100);
  if(!digitalRead(BUTTON)) {
    wdt_reset();
  } 
}