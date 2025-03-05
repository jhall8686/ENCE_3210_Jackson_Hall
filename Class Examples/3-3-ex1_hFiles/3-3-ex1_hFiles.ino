#include "myLib.h"

void setup() {
  Serial.begin(9600);
  int result = add(4,3);
  Serial.println(result);
}

void loop() {


}