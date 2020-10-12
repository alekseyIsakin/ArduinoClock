#include <Arduino.h>

#include "SPI.h"
#include "Wire.h"
#include <Arduino_ST7789_Fast.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>

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
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

uint32_t t_size = 7;
uint32_t x_size = 6*t_size;
uint32_t y_size = 8*t_size;
uint32_t last_msg_ln = 0;

String ols_str = ""; 

void redraw(String);

void setup() {
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(3);
  tft.clearScreen();

  delay(100);
}

void loop() {
  String s = "";

  if (Serial.available()){
    s = Serial.readString();

  };

  if (s != ""){
    tft.setTextColor(RED);
    tft.setTextSize(t_size);
    // tft.clearScreen();

    tft.setCursor(0, 0);

    if (ols_str != s)
    {
      ols_str = s;
      redraw(s);
    }
  }
}

void redraw(String str)
{
  for (uint32_t i=0; i<last_msg_ln; i++){
    tft.fillRect(x_size*i, 0, x_size, y_size, BLACK);
    delay(75);
  }
  for (uint32_t i=0; i<str.length(); i++){
    tft.print(str[i]);
    delay(75);
  }
  last_msg_ln = str.length();
}