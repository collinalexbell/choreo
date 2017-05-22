#include <Arduino.h>
#include "motor.h"
#include "command.h"


Motor::Motor
(int servo_pin, int motor_switch_pin ,Motor_Command_Queue* command_queue, int start_pos):
  command_queue(command_queue), pos(start_pos), motor_switch_pin(motor_switch_pin), servo_pin(servo_pin){

  servo.attach(servo_pin);
  servo.write(start_pos);
  pinMode(motor_switch_pin, OUTPUT);
};

void Motor::set_bounds(int upper, int lower){
  upper_bound = upper;
  lower_bound = lower;
}

void Motor::time_step(){
  //Fetch the command and check if it is ready to be sent
  cur_command = command_queue->front();

  if(millis() >= cur_command->time){
    send_command_to_physical_motor();
    command_queue->pop();
  }

  //If command was sent to the physical motor:
  //    Pop deletes the memory containing the command
  //    Therefore, set cur_command to NULL
  //Else the command is not actually a current command
  //    It will be sent in the future
  //    Therefore, also set cur_command to NULL
  cur_command = NULL;
};

void Motor::send_command_to_physical_motor(){
  if(cur_command->pos >= lower_bound && cur_command->pos <= upper_bound){
    servo.write(cur_command->pos);
  };
};
