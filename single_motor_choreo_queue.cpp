#include "single_motor_choreo_queue.h"

#define SMCQ Single_Motor_Choreo_Queue


Motor_Command* SMCQ::front()
{

  //Uses recursion.
  //If root->q->front is null, then the next node should be used
  //This is done by setting root to root->next.

  Motor_Command* rv;
  rv = mcq_buf[root_index]->front();

  //Move on to the next node (hence the "multi" part of the name)
  if(!rv){
    //On a circular buffer, the root index == last_index if no next
    if(root_index == last_index){
      //SMCQ has been fully executed
      more_commands_to_execute = false;
      return NULL;
    }
    root_index = (root_index + 1) % mcq_buf_size;
    rv = front();
  }
  Serial.println(rv->pos);
  return rv;
}

void SMCQ::pop()
{
  mcq_buf[root_index]->pop();
}

int SMCQ::size()
{
  return 1;
}

void SMCQ::insert(Motor_Command_Queue* q)
{
  //If there are more commands to execute
  //then that means there is a valid root
  //which means the new q gets inserted after the last index
  if(more_commands_to_execute){
    last_index = (last_index + 1) % mcq_buf_size;
    mcq_buf[last_index] = q;
  }else{
    //Otherwise, there is no valid data,
    //and everything can be over written, start at 0
    mcq_buf[0] = q;
    last_index = 0;
    root_index = 0;
    more_commands_to_execute = true;
  }
}
