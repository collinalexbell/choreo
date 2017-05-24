#ifndef MULTI_MOTOR_COMMAND_Q_H
#define MUTLI_MOTOR_COMMAND_Q_H

#include "command.h"
#define CHOREO_SIZE_ALLOCATED 5

/*
  Single_Motor_Choreo_Queue

  An implementation of the Motor_Command_Queue that contains within it a list of command queues used to perform choreography (in robotics, even walking is choreography!!!)
 */

class Single_Motor_Choreo_Queue: public Motor_Command_Queue
{
 public:
  Single_Motor_Choreo_Queue(){};
  Motor_Command* front();
  void pop();
  int size();
  void insert(Motor_Command_Queue* q);
 private:
  int root_index = 0;
  Motor_Command_Queue* mcq_buf[CHOREO_SIZE_ALLOCATED];
  int last_index = 0;
  int mcq_buf_size = CHOREO_SIZE_ALLOCATED;
  bool more_commands_to_execute = false;
};

#endif
