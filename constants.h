#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <SoftwareSerial.h>

#define PROC_PER_MOTOR 12
#define MOTOR_SWITCH_PIN 8
#define NUMBER_OF_MOTORS 2

#define USE_BTSERIAL false

#if USE_BTSERIAL
SoftwareSerial BTSerial(5, 6);
#define MY_SERIAL BTSerial

#else
#define MY_SERIAL Serial

#endif

void serialFlush(){
  while(MY_SERIAL.available() > 0) {
    char t = MY_SERIAL.read();
  }
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif
