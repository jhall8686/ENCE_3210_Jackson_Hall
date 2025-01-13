#include <math.h>

float cosines[361];

void setup() {
  Serial.begin(9600);
  for(int i = 0; i <= 360; i++) {
    cosines[i] = cos(i*PI/180);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(cosines[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
