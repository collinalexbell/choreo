#include <Arduino.h>
#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

/*
  Motor_Commands are simple data structs that contain a position the motor should be in as well as time to wait from the previous command before this command is sent to the motor.
 */

struct Motor_Command{
public:
  Motor_Command(unsigned long time, int pos) : time(time), pos(pos){};
  unsigned long  time; //Time to execute command (using millis())
  int  pos;  //Position in some position space (most likely degrees)
};



/*
  Motor Command Queue

  An abstract class that defines the interface for fetching motor commands.
  This interface is designed to support several types of command queues.
  For low memory resources, procedural command queues must be implemented.
  For higher memory resources, complex movements can be cached in command queues.
  For low latency serial connections, command queues could be implemented on top of Serial
 */

class Motor_Command_Queue{
public:
  virtual Motor_Command* front() =0;
  virtual void pop() =0;
  virtual int  size() =0;
};


/*
  Linear_Procedural_Command_Queue

  A Motor Command Queue that procedurally generates the commands to get from a start position to a dest position in a linear way. Make sure to ALWAYS call pop only after using the returned Motor_Command as pop deallocates the memory for the command
 */

class Linear_Procedural_Command_Queue :public Motor_Command_Queue {
public:
  Linear_Procedural_Command_Queue(int start_pos, int dest_pos, int duration);
  Motor_Command* front();
  void pop();
  int size();
private:
  int duration;;
  unsigned long start_time;;
  unsigned long next_time;;
  unsigned long cur_time;;
  int next_position_delta;;
  int next_position_absolute;;
  int start_pos;;
  int dest_pos;;
  float dPos_dMillis;;
  float dMillis_dPos;;
  Motor_Command* next;;
};


#endif
