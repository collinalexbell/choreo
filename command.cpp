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
  cache = NULL;
};


Motor_Command* LPC::front(){
  //First call the RV will be used.
  //However, in subsequent calls, the rv will be calculated and returned, but not used by the motor.

  Motor_Command* rv;
  cur_time = millis();
  if(start_time == -1) start_time = cur_time;

  //After pop is called (or on initial front()),
  //next is NULL and must be instantiated
  if(next == NULL){

    //If the position has not reached dest pos...
    //... then set `next` to the new Motor Command
    if(next_position_absolute != dest_pos){
      //Generate the next (future) position given the current time
      next_position_delta = ((cur_time - start_time) * dPos_dMillis) + 1;
      next_position_absolute = start_pos + next_position_delta;

      //Generate the next time given the next position
      next_time = (dMillis_dPos * next_position_delta) + start_time;
      next = new Motor_Command(next_time, next_position_absolute);
    }
    //Else IMPLICIT If the position has reached dest pos
    // then set 'next' to the cache
  }
  //Either:
  //   next has been set to a new Motor_Command
  //   next is set to the last Motor_Command generated
  //   next is null because there are no more commands to give out
  return next;
};

void LPC::pop(){
  // Pop must only be called after one uses the Motor_Command.
  // Else dangling pointers will ensue
  delete next;
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
