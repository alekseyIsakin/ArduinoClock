#include <PageBuilder.hpp>

SingleString*PageBuilder::CompileSingleString(byte*str, uint32_t lenght)
{
  byte options = 1;
  uint32_t cur_pos = 0;

  Point pos = {0,0};
  String str_dt = "";
  uint32_t SizeFont = -1;
  int strColor = BLUE;

  while (options <= COUNT_STR_ELEMENT_OPTIONS)
  {
    switch (str[cur_pos])
    {
      case Position:
        pos = {str[cur_pos+1], str[cur_pos+2]};
        cur_pos += 3;
        break;
      case Color:
        strColor += str[cur_pos+1];
        strColor += ((uint32_t)str[cur_pos+2]) << 8;
        cur_pos += 3;
        break;
      case Size:
        SizeFont += str[cur_pos+1];
        cur_pos += 2;
        break;
      case Data:
        while (str[cur_pos] != 0)
        {
          str_dt += char(str[cur_pos]);
          cur_pos += 1;
        }
        break;
    }
    options++;
  }

  SingleString*ss =  new SingleString(str_dt, pos, tft, SizeFont);

  return ss;
}

void PageBuilder::ReadString(byte*str, uint32_t lenght)
{
  uint64_t cur_byte = 0;
  while (cur_byte != lenght)
  {

    switch (str[cur_byte])
    {
      case StringElement:
        SingleString*ss = CompileSingleString(str, cur_byte);
        ss->Draw();
        cur_byte += 10;
        cur_byte += ss->strLenght;
        delete ss;
        break;
    }
  }
}

PageBuilder::PageBuilder(Arduino_ST7789*tft)
  { this->tft = tft; }