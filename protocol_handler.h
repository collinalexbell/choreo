#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>
#include "motor.h"
#include "constants.h"
#include "command.h"


Motor* motors[NUMBER_OF_MOTORS];
Linear_Procedural_Command_Queue lpcqs[NUM_LPCQ];
Single_Motor_Choreo_Queue smcqs[NUM_SMCQ];




/*
  There are several commands that can be issued:

  All values are in binary and represent single or double byte numbers.

  =============================
  |        MOTORS OFF         |
  =============================
  |  0                        |
  =============================


  =============================
  |        MOTORS ON          |
  =============================
  |  1                        |
  =============================


  =============================
  |            LPCQ           |
  =============================
  |  2                        |
  |  <motor>                  |
  |    <amount><duration>     |
  =============================


  =============================
  |           SMCQ            |
  =============================
  |  3                        |
  |    <motor>                |
  |      <num LPCQs>          |
  |      <amount><duration>   |
  |      <amount><duration>   |
  |      <amount><duration>   |
  =============================


  =============================
  |           MMCQ            |
  =============================
  |  4                        |
  |  <code>                   |
  |   <num_motors>            |
  |     <motor>               |
  |      <num LPCQs>          |
  |       <amount><duration>  |
  |       <amount><duration>  |
  |       <amount><duration>  |
  |     <motor>               |
  |      <num LPCQs>          |
  |       <amount><duration>  |
  |       <amount><duration>  |
  |       <amount><duration>  |
  =============================

  =============================
  |REMAINING_COMMANDS_ON_MOTOR|
  =============================
  |  5<motor>                 |
  =============================

  =============================
  |        POSITIONS          |
  =============================
  |  6                        |
  =============================
  |  <motor0 position>        |
  |  <motor1 position>        |
  |  <motor2 position>        |
  =============================
 */

void create_lpcq(){

  char char_buffer[5];
  int  int_buffer[3], i;
  Serial.readBytes(char_buffer, 5);

  int_buffer[0] = (int)char_buffer[0];

  //Amount
  int_buffer[1] = (unsigned char)char_buffer[1] << 8 | (unsigned char)char_buffer[2];

  //Duration
  int_buffer[2] = (unsigned char)char_buffer[3] << 8 | (unsigned char)char_buffer[4];

  Serial.println(int_buffer[0]);
  Serial.println(int_buffer[1]);
  Serial.println(int_buffer[2]);

  //Find an available LPCQ
  for(i=0;i<NUM_LPCQ;i++){
    if(!lpcqs[i].front()){
      break;
    }
    //If it didn't break, catch it on the way out
    //and increment to indicate that none are ok
    if(i == NUM_LPCQ -1){
      i++;
    }
  }

  //Are there any lpcqs available and is the command within bounds
  if(i != NUM_LPCQ
     &&
     motors[int_buffer[0]]->get_pos() + int_buffer[1] >=
     motors[int_buffer[0]]->get_lower_bound()
     &&
     motors[int_buffer[0]]->get_pos() + int_buffer[1] <=
     motors[int_buffer[0]]->get_upper_bound())
    {
      lpcqs[i] =
        Linear_Procedural_Command_Queue(motors[int_buffer[0]]->get_pos(),

                                        motors[int_buffer[0]]->get_pos()
                                        + int_buffer[1],

                                        int_buffer[2]);

      motors[int_buffer[0]]->add_command_queue(&lpcqs[0]);

  //Pick out the error
  }else if(i == NUM_LPCQ){
    Serial.println("No LPCQs available");
  }else{
    Serial.println("Dest position is out of bounds");
  }
}


void handle_serial_commands
()
{
  byte count;
  char char_buffer[1];
  int  i;

  if(Serial.available() >0){
    count = Serial.readBytes(char_buffer, 1);
    i = char_buffer[0];
    switch(i){
    case(0):
      Serial.println("motors_off");
      digitalWrite(MOTOR_SWITCH_PIN, LOW);
      break;
    case(1):
      Serial.println("motors_on");
      digitalWrite(MOTOR_SWITCH_PIN, HIGH);
      break;
    case(2):
      Serial.println("LPCQ");
      create_lpcq();
      break;
    case(3):
      Serial.println("SMCQ");

      break;
    case(4):
      Serial.println("MMCQ");
      break;
    case(5):
      Serial.println("Remaining commands");
      break;
    case(6):
      Serial.println("Positions");
      for(i=0; i<NUMBER_OF_MOTORS; i++){
        Serial.write(motors[i]->get_pos());
      }
      break;
    default:
      Serial.println("Unkown Command");
      break;

    }
  }
}


#endif
