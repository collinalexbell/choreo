#ifndef CB_ARDUINO_UTIL
#define CB_ARDUINO_UTIL

#include <Arduino.h>

void debug_print(char* label, String var){
  Serial.print(label);
  Serial.print(":");
  Serial.print(var);
  Serial.print("\n");
}

#endif
