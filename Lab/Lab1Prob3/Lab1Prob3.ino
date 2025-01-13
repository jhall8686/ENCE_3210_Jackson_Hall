int arr[50];
int* mem = arr + 50;

void populateArray(int *arr, int size) {
  for(int i = 0; i < size; i++) {
    arr[i] = i+1;
  }
}
void setup() {
  // populate array with 1-50
  populateArray(arr, 50);
  Serial.begin(9600);

  // defining initial 2 maximums
  int max1 = arr[0];
  int max2 = -1;

  //iterate through the array, looking form maximums 
  for(int i = 1; i < 50; i++) {
     if(arr[i] > max1) {
        max2 = max1;
        max1 = arr[i];
     } else if(arr[i] > max2 && arr[i] != max1) {
        max2 = arr[i];
     }
  }
  *mem = max2;
  Serial.print(*mem);
}

void loop() {
  // put your main code here, to run repeatedly:

}
