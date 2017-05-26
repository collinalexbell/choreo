#include <Servo.h>
#include "motor.h"
#include "constants.h"



Motor *motors[NUMBER_OF_MOTORS];
byte i, num_motors;
bool active = false;

void setup(){
  Serial.begin(9600);
  motors[0] = new Motor(0,10,90); //Pin 10 is the servo signal pin
  motors[1] = new Motor(1,11,70); //Pin 11 is the servo signal pin
  pinMode(MOTOR_SWITCH_PIN, OUTPUT);
  digitalWrite(MOTOR_SWITCH_PIN, LOW);
  Serial.println("Starting");
}

/*
  num_motors
  motor_id
  num_procedures
  procedure_id
  amount
  duration
  .
  .
  motor_id
  num_procedures
  procedure_id
  amount
  duration
  .
  .

 */



void handle_command(){
  char char_buf[4];
  byte i, y, motor_id, num_motors, num_procedures, procedure_id;
  short amount, duration;
  if(Serial.available()>0){
    delay(1000);
    Serial.readBytes(char_buf, 1);
    num_motors = (byte)char_buf[0];

    for(i=0;i<num_motors;i++){
      Serial.readBytes(char_buf, 2);
      motor_id = (byte)char_buf[0];
      num_procedures = (byte)char_buf[1];

      for(y=0;y<num_procedures;y++){
        Serial.readBytes(char_buf, 1);
        procedure_id = (byte)char_buf[0];
        Serial.readBytes(char_buf, 4);
        amount = (unsigned char)char_buf[0] << 8 | (unsigned char)char_buf[1];

        duration = (unsigned char)char_buf[2] << 8 | (unsigned char)char_buf[3];

        motors[motor_id]->add_linear_procedure(procedure_id, amount, duration);
      }
    }
  }
}

void loop(){
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

