#include <Arduino.h>
#include <Arduino_ST7789_Fast.h>

#ifndef PAGEBUILDER
#define PAGEBUILDER

#define COUNT_PAGE_ELEMENT  1
enum TPageEl 
{ 
  StringElement=65,
  TimeElement,
  ClearCode = 127
};
struct Point
{
  byte X;
  byte Y;
};

byte CompileSingleString(byte*str, byte c_pos, Arduino_ST7789 tft, bool erase=false);
byte CompileTime(byte*str, byte c_pos, Arduino_ST7789 tft, bool erase=false);



#endif