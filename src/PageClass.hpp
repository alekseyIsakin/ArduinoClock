#include <Arduino.h>
#include <Arduino_ST7789_Fast.h>

#ifndef PAGECLASS
#define PAGECLASS

class BaseUIelement
{
  virtual void Draw();
};

struct Point
{
  uint8_t X;
  uint8_t Y;
};

class CellMaker : BaseUIelement
{
private:
    int           Lenght;
    Point         pos;
    
    uint8_t       sizeCell;
    const uint8_t defCellSize[2] = {6, 8};
    
    int           cellColor;

    uint16_t      tDelay;

    Arduino_ST7789* tft;

public:
    void Draw();

    void setLenght(uint32_t lng);
    void SetPos(uint8_t, uint8_t);
    void SetSizeCell(uint8_t);

    uint32_t getLenght();
    CellMaker(Point pos, Arduino_ST7789*tft, uint32_t sizeCell=3, 
              int cellColor=BLACK, uint16_t tDelay=75);
};

class SingleString : BaseUIelement
{
  private:
  public:
    char*         str = nullptr;
    uint32_t      strLenght;
    bool          redraw = true; 

    Point         pos;
    uint8_t       sizeFont;
    const uint8_t defFontSize[2] = {6, 8};

    int           bgColor;
    int           strColor;

    uint16_t      tDelay;

    Arduino_ST7789*tft;
    CellMaker     *filler;

    void NewString(String);
    void ClearLastString();

    void Draw();
    void ForceRedraw();

    void SetPos(uint8_t, uint8_t);
    SingleString(String str, Point pos, Arduino_ST7789*tft, uint32_t sizeFont=3, 
                 int strColor=BLUE, int bgColor=BLACK, 
                 uint16_t tDelay=75);
    ~SingleString();
};

class MultiString : SingleString
{
  public:
    void Draw(Arduino_ST7789*);
    MultiString();
};
#endif