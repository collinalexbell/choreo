#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define PROC_PER_MOTOR 12
#define MOTOR_SWITCH_PIN 8
#define NUMBER_OF_MOTORS 2

void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif
