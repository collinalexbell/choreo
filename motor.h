#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "procedure.h"
#include "constants.h"
#include "Servo.h"

class Motor{
 private:
  Servo servo;
  LinearProcedure procedures[PROC_PER_MOTOR];
  unsigned long last_procedure_start_time;
  short procedure_index = -1;
  short last_procedure_index = -1;
  short position;
  short position_at_procedure_start;
  short position_upper_bound = 0;
  short position_lower_bound = 180;
  byte motor_num;

  void calculate_next_position(){
    short time_elapsed, next_position_delta;
    time_elapsed = millis() - last_procedure_start_time;
    next_position_delta = procedures[procedure_index].proceed(time_elapsed);
    position = position_at_procedure_start + next_position_delta;
  }

  void change_current_procedure_if_necessary(){
    short time_elapsed;
    time_elapsed = millis() - last_procedure_start_time;
    if(procedures[procedure_index].proceed(time_elapsed)
       ==
       procedures[procedure_index].amount)
      {
        deactivate_current_procedure();
      }
  }

  void deactivate_current_procedure(){
    last_procedure_start_time = millis();
    position_at_procedure_start = position;
    if(procedure_index >= -1)
    MY_SERIAL.println(procedures[procedure_index].id);
    if(procedure_index == last_procedure_index){
      procedure_index = -1;
      last_procedure_index = -1;
    }
    else{
      procedure_index += 1;
      procedure_index %= PROC_PER_MOTOR;
    }
  }

  bool procedure_exists() {

    if(procedure_index>=0){
      return true;
    }else{
      return false;
    }
  };

 public:
  Motor(byte n, short p, short position_){
    servo.attach(p);
    position = position_;
    motor_num = n;
  };

  void add_linear_procedure(byte id, short amount, short duration){
    short insert_index;
    insert_index = (last_procedure_index + 1) % PROC_PER_MOTOR;

    //Check if the buffer is full. If not: insert.
    if(insert_index!=procedure_index){
      digitalWrite(MOTOR_SWITCH_PIN, HIGH);
      procedures[insert_index] = LinearProcedure(id, amount, duration);
      last_procedure_index = insert_index;
      if(procedure_index == -1){
        procedure_index = insert_index;
        position_at_procedure_start = position;
        last_procedure_start_time = millis();
      }
    }
  }



  void step(){
    if(procedure_exists()){
      change_current_procedure_if_necessary();
    }
    //Procedure may have been deleted in the previous line;
    if(procedure_exists()){
      calculate_next_position();
    }
    servo.write(position);
  };

  bool is_active(){
    if(procedure_index == -1){
      return false;
    }else{
      return true;
    }
  }
};
#endif
