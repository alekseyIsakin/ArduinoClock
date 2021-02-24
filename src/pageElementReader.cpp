#include <pageElementReader.hpp>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>

Point printTimeSegment(byte dt, Arduino_ST7789 tft)
{
  if (dt < 10)
    tft.print("0");  
  if (dt == 0)
    tft.print("0");  
  else
    tft.print(dt);  

  Point cursor = {tft.getCursorX(),tft.getCursorY()};
  return cursor;
}

byte CompileSingleString(byte*str, byte c_pos, Arduino_ST7789 tft, bool erase)
{
  byte cur_pos = c_pos;

  Point pos = {0,0};
  String str_dt = "";
  byte SizeFont = 0;
  uint32_t strColor = BLACK;

  pos = {str[cur_pos]*2, str[cur_pos+1]*2};
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

byte CompileTime(byte*str, byte c_pos, Arduino_ST7789 tft, bool erase)
{
  byte cur_pos = c_pos;

  Point pos = {0,0};

  int hour=31;
  int minunte=63;
  int second=63;

  byte SizeFont = 0;
  uint32_t strColor = BLACK;

  pos = {str[cur_pos]*2, str[cur_pos+1]*2};
  cur_pos += 2;

  if (!erase)
  {
    strColor += ((int)(str[cur_pos]) << 8)*2;
    strColor += ((int)(str[cur_pos+1]))*2 ;
  }
  cur_pos += 2;
  
  hour    = (str[cur_pos]    & 0b00011111);
  minunte  = (str[cur_pos]    & 0b01100000) >> 5;
  minunte += (str[cur_pos+1]  & 0b00001111) << 2;
  second  = (str[cur_pos+1]  & 0b01110000) >> 4;
  second += (str[cur_pos+2]  & 0b01110000) >> 1;

  SizeFont = str[cur_pos+2]  & 0b00001111;
  cur_pos += 3;

  #if DEBUG
  Serial.print("Complete SingleString ");
  Serial.println(str_dt);
  #endif

  tft.setCursor(pos.X, pos.Y);
  tft.setTextSize(SizeFont);
  tft.setTextColor(strColor);

  
  if (hour < 24) {
    pos = printTimeSegment(hour, tft);
    tft.setCursor(pos.X, pos.Y);  
  }

  if (minunte < 60){
    if (hour < 24 )
      tft.print(":");
    pos = printTimeSegment(minunte, tft);
    tft.setCursor(pos.X, pos.Y);
    if (second < 60) 
      tft.print(":");
  }  
  if (second < 60){
    pos = printTimeSegment(second, tft);
  }
  return cur_pos;
}