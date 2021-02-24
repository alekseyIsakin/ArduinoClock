#include <Arduino.h>

#include "SPI.h"
#include "Wire.h"
#include <Arduino_ST7789_Fast.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include <pageElementReader.hpp>

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

#define DEBUG 0

void ReadString(byte*str, byte lenght, bool erase=false);


Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);
// SingleString *ss;
byte**arr_strings = (byte**)malloc(64*sizeof(byte*));
byte*arr = (byte*)malloc(64*sizeof(byte));
byte*arr_old = (byte*)malloc(64*sizeof(byte));

byte arr_lenght = 0;
byte arr_old_lenght = 0;

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
    bool redraw = false;
    arr_lenght = 0;

    String s = Serial.readString();
    
    for (uint8_t i = 0; i<s.length(); i++){

      if((!redraw && arr[i] != ((byte)s[i])) || 
         (arr_old_lenght != s.length()))
        redraw = true;
      
      arr[i] = ((byte)s[i]);

      arr_lenght++;
    }
      
    #if DEBUG
    tft.clearScreen();
    tft.setCursor(0,120);
    tft.setTextSize(2);
    tft.setTextColor(GREEN);
    for (int i = 0; i<s.length(); i++)
    {
      tft.print(arr[i]);
      tft.print("_");
    }
    #endif
    
    if (redraw)
    {
      ReadString(arr_old, arr_old_lenght, true);

      ReadString(arr, arr_lenght);

      arr_old_lenght = arr_lenght;
      for (byte i=0; i<64; i++)
        { 
          arr_old[i] = arr[i]; 
        }

    }
  }

  delay(100);
}

void ReadString(byte*str, byte lenght, bool erase)
{
  byte cur_byte = 0;

  #if DEBUG
  Serial.println("3_Start reading: ");
  #endif
  
  while (cur_byte != lenght)
  {
    switch (str[cur_byte])
    {
      case TimeElement:
        cur_byte++;
        cur_byte = CompileTime(str, cur_byte, tft, erase);
        cur_byte++;
        break;
      case StringElement:
        cur_byte++;
        cur_byte = CompileSingleString(str, cur_byte, tft, erase);
        cur_byte++;
        
        #if DEBUG
        Serial.println("String succeful draw");
        #endif
        break;

        case ClearCode:
          
          #if !DEBUG
          tft.clearScreen();
          for (byte i=0; i<64; i++)
            arr[i] = 0; 
          #endif

          cur_byte++;
          break;
        default:
          cur_byte++;
          break;

    }
  }
}

byte CompileSingleString(byte*str, byte c_pos, bool erase)
{
  byte cur_pos = c_pos;

  Point pos = {0,0};
  String str_dt = "";
  byte SizeFont = 0;
  uint32_t strColor = BLACK;

  pos = {str[cur_pos]*0b10, str[cur_pos+1]*0b10};
  cur_pos += 2;

  if (!erase)
  {
    strColor += ((int)(str[cur_pos]) << 8)*2;
    strColor += ((int)(str[cur_pos+1]))*2 ;
  }
  cur_pos += 2;

  SizeFont += str[cur_pos];
  cur_pos += 1;

  #if DEBUG
  Serial.print("Complete SingleString ");
  Serial.println(str_dt);
  #endif

  tft.setCursor(pos.X, pos.Y);
  tft.setTextSize(SizeFont);
  tft.setTextColor(strColor);

  while (str[cur_pos] != '\0')
  {
    tft.print(char(str[cur_pos]));
    cur_pos += 1;
    // delay(10);
  }
  return cur_pos;
}
