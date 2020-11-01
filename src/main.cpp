#include <Arduino.h>

#include "SPI.h"
#include "Wire.h"
#include <Arduino_ST7789_Fast.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include <PageBuilder.hpp>

#define TFT_DC    7
#define TFT_RST   8 
// #define TFT_CS    10 
// #define TFT_MOSI  11 
// #define TFT_SCLK  13
// #define TFT_BLK   7

/*
 ST7789 240x240 IPS (without CS pin) connections (only 6 wires required):

 #01 GND -> GND
 #02 VCC -> VCC (3.3V only!)
 #03 SCL -> D13/SCK
 #04 SDA -> D11/MOSI
 #05 RES -> D8 or any digital
 #06 DC  -> D7 or any digital
 #07 BLK -> NC
 */

void ReadString(byte*str, uint32_t lenght);

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
// SingleString *ss;
PageBuilder*pb;

void setup() {
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(3);
  tft.clearScreen();
  tft.setCursor(0,0);
  tft.setTextSize(7);
  tft.setTextColor(BLUE);
  tft.print("Ready");
  delay(500);

  tft.fillScreen(BLACK);
  delay(100);
}

void loop() {
  byte arr[] = {0x01,
                0x01,0x00,0x0,
                0x02,0x00,0x1f,
                0x03,0x03,
                0x04,'c','b','s','\0',
                0x01,
                0x01,0x00,0x48,
                0x02,0x00,0xf1,
                0x03,0x07,
                0x04,'2','2',':','1','1','\0'};
  
  ReadString(arr, 30);
  Serial.println("Loop");
  delay(5*1000);
  // pb->ReadString(arr, 16);
  // pb->ReadString(arr2, 14);
  // if (Serial.available()){
  //     ss->NewString(Serial.readString());
  //     ss->Draw();
  // };
}

uint32_t CompileSingleString(byte*str, uint32_t c_pos)
{
  byte options = 1;
  uint32_t cur_pos = c_pos;

  Point pos = {0,0};
  String str_dt = "";
  uint32_t SizeFont = -1;
  int strColor = BLUE;

  while (options <= COUNT_STR_ELEMENT_OPTIONS)
  {
    switch (options)
    {
      case Position:
        pos = {str[cur_pos+1], str[cur_pos+2]};
        cur_pos += 3;
        break;
      case Color:
        strColor += (int)(str[cur_pos+1]) << 8;
        strColor += (int)(str[cur_pos+2]) ;
        cur_pos += 3;
        break;
      case Size:
        SizeFont += str[cur_pos+1];
        cur_pos += 2;
        break;
      case Data:
        cur_pos += 1;
        while (str[cur_pos] != '\0')
        {
          str_dt += char(str[cur_pos]);
          cur_pos += 1;
        }
        break;
    }
    options++;
  }
  Serial.print("Complete SingleString ");
  Serial.println(str_dt);

  tft.setCursor(pos.X, pos.Y);
  tft.setTextSize(SizeFont);
  tft.setTextColor(strColor);
  tft.println(str_dt);

  return cur_pos;
}

void ReadString(byte*str, uint32_t lenght)
{
  uint64_t cur_byte = 0;

  Serial.print("3_Start reading: ");

  while (cur_byte != lenght)
  {
    switch (str[cur_byte])
    {
      case StringElement:
        cur_byte = CompileSingleString(str, cur_byte+1);
        cur_byte++;
        
        delay (75);
        break;
    }
  }
}