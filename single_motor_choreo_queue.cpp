#include "single_motor_choreo_queue.h"

#define SMCQ Single_Motor_Choreo_Queue


Motor_Command* SMCQ::front()
{

  //Uses recursion.
  //If root->q->front is null, then the next node should be used
  //This is done by setting root to root->next.

  Motor_Command* rv;
  rv = root->q->front();

  //Move on to the next node (hence the "multi" part of the name)
  if(!rv){
    if(!root->next){
      //SMCQ has been fully executed
      return NULL;
    }
    tmp_node = root->next;
    delete root;
    root = tmp_node;
    rv = front();
  }

  Serial.println(rv->pos);
  return rv;
}

void SMCQ::pop()
{
  root->q->pop();
}

int SMCQ::size()
{
  int s;
  while(tmp_node){
    s + tmp_node->q->size();
    tmp_node = tmp_node->next;
  }
  return s;
}

void SMCQ::insert(Motor_Command_Queue* q)
{
  if(!root){
    root = new Node(q);
  }else{
    tmp_node = root;
    while(tmp_node->next) tmp_node = tmp_node->next;
    tmp_node->next = new Node(q);
  }
}
