#include <Servo.h>
#include "choreo.h"

void setup(){
  Serial.begin(9600);
  choreo_setup();
  serialFlush();
  delay(2000);
  Serial.println("OK");
}

void loop(){
  choreo_loop();
}

