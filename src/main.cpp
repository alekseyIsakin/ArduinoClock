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

uint32_t CompileSingleString(byte*str, uint32_t c_pos, bool erase=false);

Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
// SingleString *ss;

byte*arr = (byte*)malloc(64*sizeof(byte));
byte*arr_old = (byte*)malloc(64*sizeof(byte));

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
  tft.clearScreen();
  delay(100);
}

int cnt = 0;

void loop() {
  if ((millis() / 1000) % 2 == 0){
    tft.fillCircle(208, 208, 16, YELLOW);
  }
  if ((millis() / 1000) % 2 == 1){
    tft.fillCircle(208, 208, 16, GREEN);
  }

  if (Serial.available()){

    // tft.setTextSize(1);
    // tft.setCursor(0,0);

    String s = Serial.readString();
    for (int i = 0; i<s.length(); i++){
      arr[i] = ((byte)s[i]);
      // tft.println(arr[i]);
    }

    CompileSingleString(arr_old, 1, 1);
    ReadString(arr, s.length());

    for(int i=0; i<64; i++){
      arr_old[i] = arr[i];
    }
  }
}

uint32_t CompileSingleString(byte*str, uint32_t c_pos, bool erase=false)
{
  byte options = 1;
  uint32_t cur_pos = c_pos;

  Point pos = {0,0};
  String str_dt = "";
  uint32_t SizeFont = -1;
  int strColor = BLACK;

  while (options <= COUNT_STR_ELEMENT_OPTIONS)
  {
    switch (options)
    {
      case Position:
        pos = {str[cur_pos+1], str[cur_pos+2]};
        cur_pos += 3;
        break;
      case Color:
        if (!erase)
        {
          strColor += (int)(str[cur_pos+1]) << 8;
          strColor += (int)(str[cur_pos+2]) ;
        }
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

  Serial.println("3_Start reading: ");
  while (cur_byte != lenght)
  {
    switch (str[cur_byte])
    {
      case StringElement:
        cur_byte++;
        cur_byte = CompileSingleString(str, cur_byte);
        cur_byte++;

        // tft.setTextSize(5);
        // tft.setCursor(128,128);
        // tft.print((int)cur_byte);
        // tft.print((int)lenght);
        
        Serial.println("String succeful draw");
        delay (75);
        break;
    }
  }
}