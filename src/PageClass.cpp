#include <PageClass.hpp>

//
//
//

CellMaker::CellMaker(Point pos, Arduino_ST7789*tft, uint32_t sizeCell=3, int cellColor=BLACK, uint16_t tDelay=75){
  this->pos      = pos;
  this->tft      = tft;
  this->sizeCell = sizeCell;
  this->cellColor= cellColor;
  this->tDelay   = tDelay;
}
void CellMaker::setLenght(uint32_t lng)
  { this->Lenght = lng; }
  void CellMaker::SetPos(uint8_t x, uint8_t y)
  { this->pos = {x, y}; }
void CellMaker::SetSizeCell(uint8_t size)
  { this->sizeCell = size; }

uint32_t CellMaker::getLenght()
  { return this->Lenght; }
void CellMaker::Draw()
{
  uint32_t sizeX = defCellSize[0]*sizeCell;
  uint32_t sizeY = defCellSize[1]*sizeCell;

  for(uint16_t i=0; i < Lenght; i++){
    tft->fillRect(pos.X + sizeX*i, pos.Y, sizeX, sizeY, cellColor);
    delay(tDelay);
  }
}

//
//
//

void SingleString::NewString(String str)
{
  if(String(this->str) != str)
    redraw = true;
  
  if (this->str != nullptr)
    delete this->str;
  
  if (this->strLenght < str.length())
    filler->setLenght(str.length());
  else 
    filler->setLenght(this->strLenght);
  
  this->strLenght = str.length();
  this->str = new char[this->strLenght];
  str.toCharArray(this->str, str.length()+1);
}

void SingleString::ClearLastString()
{
  uint32_t sizeX = defFontSize[0]*sizeFont;
  uint32_t sizeY = defFontSize[1]*sizeFont;

  for(int i=0; i < strLenght; i++){
    tft->fillRect(pos.X + sizeX*i, pos.Y, sizeX, sizeY, bgColor);
    delay(tDelay);
  }
}

void SingleString::Draw()
{
  if (redraw){
    ForceRedraw();
    redraw = false;
  }
}
void SingleString::ForceRedraw()
{
  filler->Draw();

  tft->setCursor(pos.X, pos.Y);
  tft->setTextSize(sizeFont);
  tft->setTextColor(strColor);

  for(uint16_t i=0; i <= strLenght; i++){
    tft->print(str[i]);
    delay(tDelay);
  }

  filler->setLenght(this->strLenght);
  filler->setLenght(this->strLenght);
}
void SingleString::SetPos(uint8_t x, uint8_t y)
{
  this->pos = {x, y}; 
  this->filler->SetPos(x, y);
}

SingleString::SingleString(String str, Point pos, Arduino_ST7789*tft, uint32_t sizeFont=3, int strColor=BLUE, int bgColor=BLACK, uint16_t tDelay=75)
{
  NewString(str);
  this->pos      = pos;
  this->tft      = tft;
  this->sizeFont = sizeFont;
  this->bgColor  = bgColor;
  this->strColor = strColor;
  this->tDelay   = tDelay;
  this->filler = new CellMaker(pos, tft, sizeFont, bgColor, tDelay);
}
SingleString::~SingleString()
{
  delete this->filler;
  delete this->str;
}