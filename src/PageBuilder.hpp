#include <Arduino.h>
#include <PageClass.hpp>

#ifndef PAGEBUILDER
#define PAGEBUILDER

class PageBuilder
{
private:
    SingleString CompileSingleString(byte*, int);
public:
  void ReadString(byte*, int);
};

#endif