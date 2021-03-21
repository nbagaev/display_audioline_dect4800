//arduino test sketch for Audioline DECT 4800 Display

#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x3A

//icon addresses
#define ICON1 0x40 
#define ICON2 0x41 
#define ICON3 0x42 
#define ICON4 0x43 
#define ICON5 0x44
#define ICON6 0x46
#define ICON7 0x48
#define ICON8 0x4A
#define ICON9 0x4B
#define ICON10 0x4C
#define ICON11 0x4D
#define ICON12 0x4E
#define ICON13 0x4F

#define BATT_EMPTY 0x01
#define BATT_1 0x11
#define BATT_2 0x19
#define BATT_3 0x1D
#define BATT_FULL 0x1F
#define LEFT 0x18
#define RIGHT 0x1C
#define REG_LENGTH 23

void init_display()
{
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(0x00);
  Wire.write(0x31);
  Wire.write(0x9F);//contrast
  Wire.write(0xFF);
  Wire.write(0x30);
  Wire.write(0x0C);
  Wire.write(0x02);
  Wire.write(0x01);   
  Wire.endTransmission();
}

void write_string(String str)
{
  uint8_t str_length = str.length();
  if(str_length > 0)
  {  
    if(str_length > REG_LENGTH)
      str_length = REG_LENGTH;
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(0x00);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(0x40);
    for(uint8_t i = 0; i < str_length - 1; i++)
    {
      Wire.write(str[i]);
    }
    Wire.endTransmission();
  }
}

void all_icons_on(bool on)
{
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(0x80);
  Wire.write(0x40);
  Wire.write(0x40);
  for(uint8_t i = 0; i < REG_LENGTH - 1; i++)
  {
    Wire.write(on);
  }
  Wire.endTransmission();
}

void icon_on(uint8_t icon, bool on)
{
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(0x80);
  Wire.write(icon);
  Wire.write(0x40);
  Wire.write(on);
  Wire.endTransmission();
}

void batt(uint8_t level)
{
  if((level < 5) && (level > -1))
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(0x80);
  Wire.write(ICON2);
  Wire.write(0x40);
  switch(level)
  {
    case 0:
            Wire.write(BATT_EMPTY);
            break;
    case 1:
            Wire.write(BATT_1);
            break;
    case 2:
            Wire.write(BATT_2);
            break;
    case 3:
            Wire.write(BATT_3);
            break;
    case 4:
            Wire.write(BATT_FULL);
            break;    
  }
  Wire.endTransmission();
}

//memory is 23 digits long but the display only has 16 digits
//but you can 'shift' the display
void shift(uint8_t dir)
{
  if(dir == LEFT || dir == RIGHT)
  {
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(0x00);
    Wire.write(dir);
    Wire.endTransmission();
  }
}

void setup()
{
  Wire.begin();  
  init_display();
  delay(2000);  
  all_icons_on(false);
  write_string("0123456789abcdefghijklm");  
  batt(0);//empty battery
  icon_on(ICON13, true);//arrow right side
}

void loop()
{  
  delay(2000);
  icon_on(ICON2, false);//blink empty battery icon
  shift(LEFT);
  icon_on(ICON1, true);//arrow left side on after left shift
  delay(2000);
  icon_on(ICON2, true);//blink empty battery icon
  shift(RIGHT);
  icon_on(ICON1, false);//arrow left side off after right shift
}
