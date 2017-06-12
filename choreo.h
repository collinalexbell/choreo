#ifndef _CHOREO_H_
#define _CHOREO_H_

#include "motor.h"
#include "constants.h"
#include "Servo.h"

Motor *motors[NUMBER_OF_MOTORS];
byte i, num_motors;
bool active = false;


void choreo_setup(){
  for(i=STARTING_PIN; i<STARTING_PIN+NUMBER_OF_MOTORS; i++){
  motors[i-STARTING_PIN] = new Motor(i-STARTING_PIN, i, 90); //Pin 10 is the servo signal pin
  }
  //ADD OR SUBTRACT MOTORS ^^^ OR CHANGE PINS
  // Motor(id, pin, starting position)

  pinMode(MOTOR_SWITCH_PIN, OUTPUT);
  digitalWrite(MOTOR_SWITCH_PIN, LOW);
}

void handle_command(){
  char char_buf[4];
  byte i, y, motor_id, num_motors, num_procedures, procedure_id;
  short amount, duration;
  if(MY_SERIAL.available()>0){
    MY_SERIAL.readBytes(char_buf, 1);
    num_motors = (byte)char_buf[0];

    for(i=0;i<num_motors;i++){
      MY_SERIAL.readBytes(char_buf, 2);
      motor_id = (byte)char_buf[0];
      num_procedures = (byte)char_buf[1];

      for(y=0;y<num_procedures;y++){
        MY_SERIAL.readBytes(char_buf, 1);
        procedure_id = (byte)char_buf[0];
        MY_SERIAL.readBytes(char_buf, 4);
        amount = (unsigned char)char_buf[0] << 8 | (unsigned char)char_buf[1];

        duration = (unsigned char)char_buf[2] << 8 | (unsigned char)char_buf[3];

        motors[motor_id]->add_linear_procedure(procedure_id, amount, duration);
	MY_SERIAL.print("Motor: ");
	MY_SERIAL.print(motor_id);
	MY_SERIAL.print("\n");
      }
    }
  }
}

void choreo_loop(){
  handle_command();
  active = false;
  for(i=0;i<NUMBER_OF_MOTORS;i++){
    motors[i]->step();
    active = active || motors[i]->is_active();
  }
  if(!active){
    digitalWrite(MOTOR_SWITCH_PIN, LOW);
  }
}


#endif
