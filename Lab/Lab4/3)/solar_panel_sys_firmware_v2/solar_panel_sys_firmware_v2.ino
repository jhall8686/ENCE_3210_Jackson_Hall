#define SOLAR_PANEL A0
#define BATTERY A1

float Vsolar;
float Vbattery;

//OLED Screen Macros
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  //OLED Screen Startup
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //If the test fails, loop forever
    for(;;); 
  }
  //Buttons acting as the voltage inputs
  pinMode(SOLAR_PANEL, INPUT);
  pinMode(BATTERY, INPUT);
  //display setup
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
}

void loop() {
  //grab readings from panel and battery
  Vsolar = analogRead(SOLAR_PANEL)*5/1023;
  Vbattery = analogRead(BATTERY)*5/1023;
  display.clearDisplay();
  

  // > 2.5 V from the Solar panel
  if(Vsolar > 2.5) {
    //uP supplied from panel
    display.print("uP <- Solar");
  }
  // < 2.5 V from the solar panel
  else {
    //uP supplied from battery
    
    display.print("uP <- Battery");
  }

  if(Vbattery < Vsolar) {
    //battery connected to solar
    display.setCursor(0,10);
    display.print("Battery Charging");
    display.setCursor(0,0);
    //Keeps the battery charging until it's charged
    while(Vbattery < 5) {
      Vbattery = analogRead(BATTERY)*5/1023;
    }
  }
  display.display();
}