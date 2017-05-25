#include <Servo.h>
#include <Arduino.h>
#include "command.h"

#ifndef ROBOTMOTOR_H
#define ROBOTMOTOR_H

class Motor{
 private:
  Servo servo;
  Motor_Command_Queue* command_queue = NULL;
  int pos;
  int servo_pin;
  int upper_bound = 180;
  int lower_bound = 0;
  Motor_Command* cur_command;
  void send_pos_to_physical_motor();
 public:
  Motor(int servo_pin, int start_pos);
  void add_command_queue(Motor_Command_Queue* q);
  void take_time_step();
  int get_pos();
  int get_upper_bound(){return upper_bound;};
  int get_lower_bound(){return lower_bound;};
  void set_bounds(int lower, int upper);
};

#endif
