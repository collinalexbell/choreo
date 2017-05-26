#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define PROC_PER_MOTOR 12
#define MOTOR_SWITCH_PIN 8
#define NUMBER_OF_MOTORS 2

//Use a macro for use with BTSerial if desired
#define SERIAL Serial

void serialFlush(){
  while(SERIAL.available() > 0) {
    char t = SERIAL.read();
  }
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

#endif
