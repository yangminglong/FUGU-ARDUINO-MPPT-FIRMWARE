#ifndef _MPPT_LCD_16x2_H
#define _MPPT_LCD_16x2_H
#include <Arduino.h>

class LCD_16x2 {
  public:
  LCD_16x2() {}
  void begin() {}
  
  void setCursor(uint8_t charIndex, uint8_t lineIndex) {

  }

  void clear() {}

  void print(char ch) { }
  void print(const String& str) { }

  void print(float value, uint8_t sp) { }

  void setBacklight(bool onOff) { }
};

extern LCD_16x2 lcd;

#endif //_MPPT_LCD_16x2_H