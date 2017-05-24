#ifndef MULTI_MOTOR_CHOREO_Q_H
#define MULTI_MOTOR_CHOREO_Q_H

#include "single_motor_choreo_queue.h"
#include "command.h"

class Multi_Motor_Choreo_Queue: public Motor_Command_Queue
/*
  This Motor_Command_Queue is shared between multiple Motors.
  Each Motor is represented by a Single_Motor_Choreo_Queue.
  Each subsequent call of front() accesses commands from a list of SMCQ.
  Each motor is assigned an index into the list of SMCQ.
  The order in which Motors call front() must be the same every iteration.
  This is because the MMCQ keeps an index of the last SMCQ accessed.
  Index++ is the only sync between front() and which SMCQ is used.
*/
{
public:
  Multi_Motor_Choreo_Queue(){};
  //Motor_Command_Queue interface
  //------------------------
  Motor_Command* front();
  void pop();
  int size();
  //------------------------

  void insert(Single_Motor_Choreo_Queue* q);

private:
  int smcq_index = 0;
  Single_Motor_Choreo_Queue* buf[NUMBER_OF_MOTORS];
  int buf_index = 0;
  int insert_index = 0;
  int buf_size = NUMBER_OF_MOTORS;
  bool commands_left = true;
};

#endif

