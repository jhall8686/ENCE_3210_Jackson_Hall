#define bitsize 14

float Vref = 3;
float Vin = 0.83;
float thresh;
float Vquantized = 0;
int count;
int bitval;
int bits[bitsize];



void setup() {
  Serial.begin(9600);
  Vref /= 2;
  thresh = Vref;

  for(count = 0; count < bitsize; count++) {
    Vref/=2;
    if(Vin >= thresh) {
      bitval = 1;
      thresh += Vref;
    } else {
      bitval = 0;
      thresh -= Vref;
    }

    bits[count] = bitval;
    Vquantized += 2*Vref*bitval;
  }
  Serial.println(Vquantized);
}

void loop() {

}