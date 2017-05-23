#ifndef MULTI_MOTOR_COMMAND_Q_H
#define MUTLI_MOTOR_COMMAND_Q_H

#include "command.h"

struct Node{
  Node(Motor_Command_Queue* q_){q = q_;};
  Node* next = NULL;
  Motor_Command_Queue* q;
};

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
  Node* root = NULL;
  Node* tmp_node;
};

#endif
