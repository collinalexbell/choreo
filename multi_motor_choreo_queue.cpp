#include "multi_motor_choreo_queue.h"
#include "command.h"


#define MMCQ Multi_Motor_Choreo_Queue



Motor_Command* MMCQ::front()
{
  if(commands_left){
    Motor_Command* rv;
    rv = buf[buf_index]->front();
    buf_index++;
    buf_index %= buf_size;

    //If the last SMCQ has no commands left, then this queue is exhausted
    if(!rv && buf_index == (buf_size-1)){
      commands_left = false;
    }
    return rv;
  }
};

void MMCQ::pop(){
  if(buf_index == 0){
    buf_index = buf_size-1;
  }else{
    buf_index--;
  }

  buf[buf_index]->pop();

  buf_index++;
  buf_index %= buf_size;
}

int MMCQ::size(){
  return 1;
}

void MMCQ::insert(Single_Motor_Choreo_Queue* q){
  buf[insert_index] = q;
  insert_index++; insert_index%=buf_size;
}

