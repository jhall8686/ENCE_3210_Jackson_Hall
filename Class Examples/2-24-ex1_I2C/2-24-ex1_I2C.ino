#include <Wire.h>

#define DS3231_ADDR               0x68 //0b0 1101000 (p. 16 on datasheet)
#define DS3231_READ               0xD0 //0b1101000 0
#define DS3231_WRITE              0xD1 //0b1101000 1

#define DS3231_SECONDS            0x00
#define DS3231_MINUTES            0x01
#define DS3231_HOURS              0x02
#define DS3231_DAY                0x03
#define DS3231_DATE               0x04
#define DS3231_MONTH              0x05
#define DS3231_YEAR               0x06
#define DS3231_ALARM1_SECONDS     0x07
#define DS3231_ALARM1_MINUTES     0x08
#define DS3231_ALARM1_HOURS       0x09
#define DS3231_ALARM1_DAY_DATE    0x0A
#define DS3231_ALARM2_MINUTES     0x0B
#define DS3231_ALARM2_HOURS       0x0C
#define DS3231_ALARM2_DAY_DATE    0x0D
#define DS3231_CONTROL            0x0E
#define DS3231_CONTROL_STATUS     0x0F
#define DS3231_AGING_OFFSET       0x10
#define DS3231_TEMP_MSB           0x11
#define DS3231_TEMP_LSB           0x12

uint8_t seconds;

void setup() {
  Serial.begin(9600);

  //Start I2C
  Wire.begin();


}

void loop() {
  delay(1000);
  
  Serial.println(fromBCD(readRegister(DS3231_SECONDS)));

}

uint8_t readRegister(uint8_t reg) {
  Wire.beginTransmission(DS3231_ADDR);//Start bit
  Wire.write(reg);//Send command
  Wire.endTransmission();//Stop bit

  //Request 1 byte
  Wire.requestFrom(DS3231_ADDR, 1);
  return Wire.read();
  
}

uint8_t fromBCD(uint8_t bcd) {
  return (((bcd & 0xF0)>>4)*10) + (bcd & 0x0F);
}