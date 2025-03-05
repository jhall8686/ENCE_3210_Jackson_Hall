//MAX7219 - using SPI library to display 8x8 bitmap
#include <SPI.h>

#define CS 10

//MAX 7219 REGISTERS

#define MAX7219_DECODE_MODE   0x09
#define MAX7219_INTENSITY     0x0A
#define MAX7219_SCAN_LIMIT    0x0B
#define MAX7219_SHUTDOWN      0x0C
#define MAX7219_DISPLAY_TEST  0x0F

const uint8_t IMAGES[][8] = {
{
  0b00000000,
  0b00011000,
  0b00011000,
  0b00111000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b01111110
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b00000110,
  0b00001100,
  0b00110000,
  0b01100000,
  0b01111110
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b00000110,
  0b00011100,
  0b00000110,
  0b01100110,
  0b00111100
},{
  0b00000000,
  0b00001100,
  0b00011100,
  0b00101100,
  0b01001100,
  0b01111110,
  0b00001100,
  0b00001100
},{
  0b00000000,
  0b01111110,
  0b01100000,
  0b01111100,
  0b00000110,
  0b00000110,
  0b01100110,
  0b00111100
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b01100000,
  0b01111100,
  0b01100110,
  0b01100110,
  0b00111100
},{
  0b00000000,
  0b01111110,
  0b01100110,
  0b00001100,
  0b00001100,
  0b00011000,
  0b00011000,
  0b00011000
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111100
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111110,
  0b00000110,
  0b01100110,
  0b00111100
},{
  0b00000000,
  0b00111100,
  0b01100110,
  0b01101110,
  0b01110110,
  0b01100110,
  0b01100110,
  0b00111100
}};


void setup() {
  //Start SPI
  SPI.begin();
  writeRegister(MAX7219_DISPLAY_TEST,0x01);
  delay(1000);
  writeRegister(MAX7219_DISPLAY_TEST, 0x00);
  writeRegister(MAX7219_INTENSITY, 0x00);
  delay(1000);
  writeRegister(MAX7219_DECODE_MODE, 0x00);
  writeRegister(MAX7219_SCAN_LIMIT, 0x0F);
  writeRegister(MAX7219_SHUTDOWN, 0x01);
  writeRegister(MAX7219_INTENSITY,0x0F);


}

void loop() {
  for(int j = 0; j < 8; j++) {
    for(int i = 1; i < 9; i++) {
      writeRegister(i, IMAGES[j][i-1]);
    }
    delay(1000);
  }
}
  



void writeRegister(uint8_t reg, uint8_t value) {
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW); //select device
  SPI.transfer(reg); //send address
  SPI.transfer(value); //send value to reg
  digitalWrite(CS, HIGH); //release device
  SPI.endTransaction();
}
