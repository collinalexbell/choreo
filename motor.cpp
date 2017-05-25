#include <Arduino.h>
#include "motor.h"
#include "command.h"


Motor::Motor
(int servo_pin, int start_pos):
   pos(start_pos), servo_pin(servo_pin){

  command_queue = NULL;
  servo.attach(servo_pin);
  servo.write(start_pos);
};

void Motor::add_command_queue(Motor_Command_Queue* q){
  if(command_queue){
    command_queue->deactivate();
  }
  command_queue = q;
}

int Motor::get_pos(){
  return pos;
}

void Motor::set_bounds(int lower, int upper){
  lower_bound = lower;
  upper_bound = upper;
}

void Motor::take_time_step(){
  //Fetch the command and check if it is ready to be sent
  if(command_queue){
    //delay(500);
    cur_command = command_queue->front();
    if(!cur_command){
      // The command_queue has finished
      command_queue = NULL;
    }
    Serial.print("Cur_command_time: ");
    Serial.print(cur_command->time);
    Serial.print("\n");
    if(cur_command && millis() >= cur_command->time){
      command_queue->pop();
      pos = cur_command->pos;
    }
    send_pos_to_physical_motor();

    //If command was sent to the physical motor:
    //    Pop deletes the memory containing the command
    //    Therefore, set cur_command to NULL
    //Else the command is not actually a current command
    //    It will be sent in the future
    //    Therefore, also set cur_command to NULL
    cur_command = NULL;
  }
};

void Motor::send_pos_to_physical_motor(){
  if(pos >= lower_bound && pos <= upper_bound){
    servo.write(pos);
  };
};
