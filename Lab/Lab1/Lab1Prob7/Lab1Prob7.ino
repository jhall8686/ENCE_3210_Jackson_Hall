
#define RLED 3
#define GLED 4
#define BLED 5

#define BUTTON1 11
#define BUTTON2 12

int arr1[10] = {};
int arr2[10] = {};

int ans[10] = {};
int counter = 0;
//populates arrays with sequential ints 1-10
void populateArray10(int arr[10]) {
  for(int i = 0; i<10; i++) {
    arr[i] = i+1;
  }
}

void setup() {
  populateArray10(arr1);
  populateArray10(arr2);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
}

void loop() {
  // mode changer (counter = 1 --> addition, counter = 2 --> subtraction,
  // counter = 0 --> multiplication)
  if(digitalRead(BUTTON1)) {
    counter++;
    if(counter == 1) {
      digitalWrite(RLED, HIGH);
      digitalWrite(BLED, LOW);
    }
    if(counter == 2) {
      digitalWrite(GLED, HIGH);
      digitalWrite(RLED, LOW);
    }
    if(counter == 3) {
      digitalWrite(BLED, HIGH);
      digitalWrite(GLED, LOW);
      counter = 0;
    }
  }
  // calculator
  if(digitalRead(BUTTON2)) {
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
    digitalWrite(BLED, LOW);
    // performs and stores operations
    for(int i = 0; i < 10; i++) {
      switch(counter) {
        //add
        case 1:
          ans[i] = arr1[i] + arr2[i];
        break;
        //subtract
        case 2:
          ans[i] = arr1[i] - arr2[i];
        break;
        //multiply
        case 0:
          ans[i] = arr1[i] * arr2[i];
        break;
      }
    }
    digitalWrite(RLED, HIGH);
    digitalWrite(GLED, HIGH);
    digitalWrite(BLED, HIGH);
  }
  
}