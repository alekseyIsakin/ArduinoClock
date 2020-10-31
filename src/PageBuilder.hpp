#include <Arduino.h>
#include <PageClass.hpp>

#ifndef PAGEBUILDER
#define PAGEBUILDER

#define COUNT_PAGE_ELEMENT  1
enum TPageEl { StringElement=1, };

#define COUNT_STR_ELEMENT_OPTIONS  4
enum StrPageEl
{
  Position=1,
  Color,
  Size,
  Data,
};

class PageBuilder
{
private:
  SingleString*CompileSingleString(byte*, uint32_t);  
  BaseUIelement*UI_arr;

  Arduino_ST7789* tft;
public:
  void ReadString(byte*, uint32_t);
  void DrawPage();

  PageBuilder(Arduino_ST7789*);
};

#endif