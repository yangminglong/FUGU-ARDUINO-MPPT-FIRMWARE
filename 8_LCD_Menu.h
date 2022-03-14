#ifndef _POWERON_LCD_MENU_H
#define _POWERON_LCD_MENU_H

#include <string>
#include <Arduino.h>


enum KeyType {
  KeyNone,
  KeyPrev,
  KeyNext,
  KeyEnter,
};

void processKey(KeyType key);

void doOperation();



#endif