#include <Servo.h>
#include <Arduino.h>
#include "command.h"

#ifndef ROBOTMOTOR_H
#define ROBOTMOTOR_H

class Motor{
 private:
  Servo servo;
  Motor_Command_Queue* command_queue;
  int pos;
  int servo_pin;
  int motor_switch_pin;
  int upper_bound = 0;
  int lower_bound = 180;
  Motor_Command* cur_command;
  void send_command_to_physical_motor();
 public:
  Motor(int servo_pin, int motor_switch_pin,
        Motor_Command_Queue* command_queue, int start_pos);
  void time_step();
  void set_bounds(int upper, int lower);
};

#endif
