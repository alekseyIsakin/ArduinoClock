#include <Arduino.h>

#include "SPI.h"
#include "Wire.h"
#include <Arduino_ST7789_Fast.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include <PageBuilder.hpp>

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
// SingleString *ss;
PageBuilder*pb;

void setup() {
  Serial.begin(9600);

  tft.begin();
  tft.setRotation(3);
  tft.clearScreen();
  tft.setCursor(0,0);
  tft.setTextSize(7);
  tft.setTextColor(BLUE);
  tft.print("Ready");
  delay(500);
  tft.fillScreen(BLACK);

  pb = new PageBuilder(&tft);
  byte arr[] = {0x01,
                0x01,0x00,0x00,
                0x02,0x00,0xf1,
                0x03,0x07,
                0x04,'a','b','c',0x00};
  byte arr2[] ={0x01,
                0x01,0x00,0x50,
                0x02,0x00,0xf1,
                0x03,0x07,
                0x04,'a','b','c',0x00};
  pb->ReadString(arr, 14);
  pb->ReadString(arr2, 14);
  // ss = new SingleString("ready", {0,0}, &tft, 7U);
  // ss->Draw();
  // ss->SetPos(0,32);
  delay(100);
}

void loop() {
  // if (Serial.available()){
  //     ss->NewString(Serial.readString());
  //     ss->Draw();
  // };
}