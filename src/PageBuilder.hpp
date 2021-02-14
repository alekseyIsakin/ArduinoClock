#include <Arduino.h>

#ifndef PAGEBUILDER
#define PAGEBUILDER

#define COUNT_PAGE_ELEMENT  1
enum TPageEl 
{ 
  StringElement=65,
  ClearCode = 127
};

#define COUNT_STR_ELEMENT_OPTIONS  4
enum StrPageEl
{
  Position=1,
  Color,
  Size,
  Data,
};

struct Point
{
  byte X;
  byte Y;
};
#endif