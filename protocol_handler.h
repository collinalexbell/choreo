#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>
#include "motor.h"
#include "constants.h"
#include "command.h"


#define MOTOR_INDEX int_buffer[0]
#define AMOUNT int_buffer[1]
#define DURATION int_buffer[2]

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

int create_lpcq(int *int_buffer){

  char char_buffer[5];
  int  i;
  Serial.readBytes(char_buffer, 4);


  //Amount
   AMOUNT = (unsigned char)char_buffer[0] << 8 | (unsigned char)char_buffer[1];

  //Duration
  DURATION = (unsigned char)char_buffer[2] << 8 | (unsigned char)char_buffer[3];

  Serial.println(AMOUNT);
  Serial.println(DURATION);

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
     motors[MOTOR_INDEX]->get_pos() + AMOUNT >=
     motors[MOTOR_INDEX]->get_lower_bound()
     &&
     motors[MOTOR_INDEX]->get_pos() + AMOUNT <=
     motors[MOTOR_INDEX]->get_upper_bound())
    {
      return i;
  //Pick out the error
  }else if(i == NUM_LPCQ){
    Serial.println("No LPCQs available");
    return -1;
  }else{
    Serial.println("Dest position is out of bounds");
    return -2;
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
      int int_buffer[5];
      Serial.println("LPCQ");
      Serial.readBytes(char_buffer, 1);
      MOTOR_INDEX = (int)char_buffer[0];
      i = create_lpcq(int_buffer);
      //If successfully created an lpcq:
      if(i>=0){
        lpcqs[i] = Linear_Procedural_Command_Queue(
                      motors[MOTOR_INDEX]->get_pos(),
                      motors[MOTOR_INDEX]->get_pos() + AMOUNT,
                      DURATION);

        motors[MOTOR_INDEX]->add_command_queue(&lpcqs[0]);
      }
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
