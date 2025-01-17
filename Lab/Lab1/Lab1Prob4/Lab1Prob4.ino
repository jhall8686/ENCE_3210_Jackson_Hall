//pin numbers for HAL
#define RLED 13
#define GLED 5
#define BLED 4
#define wait 200

//register addresses for direct bitwise
volatile unsigned int *portDout_addr = 0x80002B;
volatile unsigned int *portBout_addr = 0x800025;

// initialize random array of R,G,B
char arr[] = {'R','G','B','G','G','G','R','R','B','R','B','B','B','B','G','R','G','R','B','G'};

//LED flash function using HAL functions
void flashLED(char color) {
  switch(color) {
    case 'R':
      digitalWrite(RLED,HIGH);
      delay(wait);
      digitalWrite(RLED,LOW);
      break;
    case 'G':
      digitalWrite(GLED,HIGH);
      delay(wait);
      digitalWrite(GLED,LOW);
      break;
    case 'B':
      digitalWrite(BLED,HIGH);
      delay(wait);
      digitalWrite(BLED,LOW);
      break;
    default:
      break; 
  }
}


//LED flash function using bitwise operations in register memory addresses
void regFlashLED(char color) {
  switch(color) {
    case 'R':
      //red LED @ PD4
      *portDout_addr |= 0x10; // 0b00010000 (bit 4)
      delay(wait);
      *portDout_addr &= ~0x10;
    break;
    case 'G':
      //green LED @ PD5
      *portDout_addr |= 0x20; // 0b00100000 (bit 5)
      delay(wait);
      *portDout_addr &= ~0x20;
    break;
    case 'B':
      //blue LED @ PB5
      *portBout_addr |= 0x20; // 0b00100000 (bit 5)
      delay(wait);
      *portBout_addr &= ~0x20;
    break;
    default:
    break;
  }
}

void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  
  Serial.begin(9600);
  //define array pointer
  char* pArr = arr;
  
  //define rgb counts
  int rCount = 0;
  int gCount = 0;
  int bCount = 0;
  
  //iterate through array
  for(int i = 0; i < 20; i++) {
    
    //grabs letter at each index
    char currentLetter = *(pArr + i);
    
    //increments rgb counts (eventually turns on respective LED)
    if(currentLetter == 'R') {
      rCount++;
    } 
    else if(currentLetter == 'G') {
      gCount++;
    }
    else if(currentLetter == 'B') {
      bCount++;
    } else {}
    regFlashLED(currentLetter);
  }

  Serial.println(rCount); //Should be 6
  Serial.println(gCount); //Should be 7
  Serial.println(bCount); //Should be 7
  
}

void loop() {}
