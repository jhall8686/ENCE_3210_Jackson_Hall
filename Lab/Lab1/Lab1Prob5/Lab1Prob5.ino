#define GLED 4
#define RLED 5

int arr[] = {1, 2, 3, 4, 3, 5, 6, 2, 8, 1, 3, 4, 1, 5, 6, 7, 6, 2,8,20}; //array of length 20
int pattern[] = {3,4,3}; //pattern the program looks for
int isPattern = 0;
void setup() {
  pinMode(GLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  digitalWrite(GLED, LOW);
  digitalWrite(RLED, LOW);
  for(int i = 0; i<18; i++) { 
    int sample[] = {arr[i], arr[i+1], arr[i+2]};
    int check = memcmp(pattern,sample,sizeof(pattern));

    if(!check) {
      digitalWrite(GLED, HIGH);
      isPattern = 1;
    }
  }
  if(isPattern == 0) {
    digitalWrite(RLED, HIGH);
  }
}

void loop() {



}