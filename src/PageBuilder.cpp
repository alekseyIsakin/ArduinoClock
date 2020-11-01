#include <PageBuilder.hpp>

SingleString PageBuilder::CompileSingleString(byte*str, uint32_t c_pos)
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
        strColor += str[cur_pos+1];
        strColor += ((uint32_t)str[cur_pos+2]) << 8;
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
  return *(new SingleString(str_dt, pos, tft, SizeFont));
}

void PageBuilder::ReadString(byte*str, uint32_t lenght)
{
  uint64_t cur_byte = 0;

  Serial.print("Start reading: ");
  Serial.println((int)str[cur_byte]);

  while (cur_byte != lenght)
  {

    switch (str[cur_byte])
    {
      case StringElement:
        Serial.print("cur_byte ");
        Serial.println((int)cur_byte);

        SingleString ss = CompileSingleString(str, cur_byte+1);
        Serial.println(ss.str);
        Serial.println(ss.sizeFont);
        ss.Draw();
        cur_byte += 11;
        cur_byte += ss.strLenght;

        break;
    }
  }
}

PageBuilder::PageBuilder(Arduino_ST7789*tft)
  { this->tft = tft; }