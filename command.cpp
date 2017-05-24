#include <Arduino.h>
#include "command.h"
#include "util.h"

#define LPC Linear_Procedural_Command_Queue

LPC::LPC(int start_pos, int dest_pos, int duration)
  : start_pos(start_pos), dest_pos(dest_pos), duration(duration)
{
  dPos_dMillis = (dest_pos - start_pos)/((float)duration);
  dMillis_dPos = 1/dPos_dMillis;

  //Set start_time on initial front() call.
  //for now, flag
  start_time = -1;

  next_position_absolute = start_pos;
  next_position_delta = 0;
  next = NULL;
};


Motor_Command* LPC::front(){

  cur_time = millis();
  if(start_time == -1) start_time = cur_time;

  //After pop is called (or on initial front()),
  //next is NULL and must be instantiated
  if(next == NULL){

    //..but only if the position has not reached dest pos...
    if((next_position_absolute < dest_pos && dPos_dMillis > 0) ||
       (next_position_absolute > dest_pos && dPos_dMillis < 0)){
      //Generate the next (future) position given the current time
      next_position_delta = ((cur_time - start_time) * dPos_dMillis) + 1;
      next_position_absolute = start_pos + next_position_delta;

      //Guard against over shooting
      if(dPos_dMillis > 0 && next_position_absolute > dest_pos){
        next_position_absolute = dest_pos;
      }
      if(dPos_dMillis < 0 && next_position_absolute < dest_pos){
        next_position_absolute = dest_pos;
      }

      //Generate the next time given the next position
      next_time = (dMillis_dPos * next_position_delta) + start_time;
      next_obj = Motor_Command(next_time, next_position_absolute);

      //Return a pointer to the member variable
      //It is dirty, I know, but it is fast.
      //Also, this class much doesn't care if the command gets altered
      next = &next_obj;
    }
  }
  //Either:
  //   next has been set to a new Motor_Command
  //   next is set to the last Motor_Command generated
  //   next is null because this linear movement has finished
  return next;
};

void LPC::pop(){
  // Pop must only be called after one uses the Motor_Command.
  // Else dangling pointers will ensue
  next = NULL;

};

int LPC::size(){
  //Will return the number of commands until reaching destination
  //Returns 0 if at destination
  int rv;
  if(dest_pos == next_position_absolute){
    rv = 0;
  }else{
    rv = (dest_pos - start_pos) - next_position_delta;
  }
  return rv;
};
