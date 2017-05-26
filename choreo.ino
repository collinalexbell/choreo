#include <Servo.h>
#include "choreo.h"

void setup(){
  SERIAL.begin(9600);
  choreo_setup();
  serialFlush();
  delay(2000);
  SERIAL.println("OK");
}

void loop(){
  choreo_loop();
}

