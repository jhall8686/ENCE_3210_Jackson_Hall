#include <stdio.h>

double arr[512];
double double_square(int n) {
  return (double) 2*n*n;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i=0;i<512;i++) {
    arr[i] = double_square(i);
    Serial.println(arr[i]);
    Serial.println("\n");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
