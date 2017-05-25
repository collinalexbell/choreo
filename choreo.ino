#include <Servo.h>
#include <SoftwareSerial.h>

#include "command.h"
#include "motor.h"
#include "single_motor_choreo_queue.h"
#include "multi_motor_choreo_queue.h"
#include "protocol_handler.h"
#include "constants.h"

//Motor* neck_motor;  // create servo object to control a servo
//Motor* head_motor;
// twelve servo objects can be created on most boards

int neck_pos = 0;    // variable to store the servo position
int head_pos = 0;
bool g = true;
int i;
int instruction, amount, duration;

Linear_Procedural_Command_Queue tmp_command_q;
Linear_Procedural_Command_Queue tmp_command_q2;
Linear_Procedural_Command_Queue tmp_command_q3;
Linear_Procedural_Command_Queue tmp_command_q4;

int tmp_dest_pos;
Single_Motor_Choreo_Queue tmp_smcq;
Single_Motor_Choreo_Queue tmp_smcq2;

Multi_Motor_Choreo_Queue tmp_mmcq;


SoftwareSerial BTSerial(6, 5); // RX | TX


void setup() {
  Serial.begin(9600);
  Serial.println("Starting new code");
  motors[0] = new Motor(10, 90);
  motors[1] = new Motor(11, 75);
  motors[1]->set_bounds(70, 95);
  Serial.println("Successfully constructed motors");
  pinMode(MOTOR_SWITCH_PIN, OUTPUT);
  BTSerial.begin(9600);


  delay(1000);
  /*
  digitalWrite(MOTOR_SWITCH_PIN, HIGH);

  //Make some LPCQs and insert into MMCQ
  tmp_command_q = Linear_Procedural_Command_Queue(90, 140, 2000);
  tmp_smcq.insert(&tmp_command_q);

  tmp_command_q2 = Linear_Procedural_Command_Queue(140, 40, 4000);
  tmp_smcq.insert(&tmp_command_q2);

  tmp_mmcq.insert(&tmp_smcq);

  tmp_command_q3 = Linear_Procedural_Command_Queue(70, 100, 2000);
  tmp_smcq2.insert(&tmp_command_q3);

  tmp_command_q4 = Linear_Procedural_Command_Queue(100, 70, 4000);
  tmp_smcq2.insert(&tmp_command_q4);

  tmp_mmcq.insert(&tmp_smcq2);

  motors[0]->add_command_queue(&tmp_mmcq);
  motors[1]->add_command_queue(&tmp_mmcq);
  */
}

void loop() {
  //handle_commands();
  handle_serial_commands();
  motors[0]->take_time_step();
  motors[1]->take_time_step();
}
