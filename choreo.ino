#include <Servo.h>
#include "choreo.h"
#include <SoftwareSerial.h>


void setup(){
  MY_SERIAL.begin(9600);
  choreo_setup();
  serialFlush();
  delay(2000);
  MY_SERIAL.println("OK");
}

void loop(){
  choreo_loop();
}

