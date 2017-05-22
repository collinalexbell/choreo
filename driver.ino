#include <Servo.h>
#include "command.h"
#include "motor.h"
#include <SoftwareSerial.h>
#include "LinkedList.h"

Servo neck;  // create servo object to control a servo
Servo head;
// twelve servo objects can be created on most boards

int neck_pos = 0;    // variable to store the servo position
int head_pos = 0;
int motor_switch = 8;
bool g = true;
int i;
int instruction, amount, thespeed;

SoftwareSerial BTSerial(6, 5); // RX | TX


/* Motor Protocol

   All commands are terminated via \r
   All optional arguments are prefixed with a null byte
   All letters are in ASCII and all numbers are in unsigned binary

   go_to_position = <x or y><cmd_id><pos><optional-speed>
   move_relatively = <X or Y><cmd_id><optional-amount><optional-speed>
   stop = s<cmd_id><x or y>
   query_position  = p
   motors_off = 0
   motors_on = 1
*/

char cmd_buff[6];
void recieve_and_parse_command(){
  if(BTSerial.available() > 0){
    BTSerial.readBytesUntil('\r', cmd_buff, 5);
    switch(cmd_buff[0]){
    case 'x':
      break;
    case 'y':
      break;
    case 'X':
      break;
    case 'Y':
      break;
    case 's':
      break;
    case 'p':
      break;
    case '0':
      break;
    case '1':
      break;
    default:
      break;
    }
  }
}

void setup() {
  neck.attach(10);  // attaches the servo on pin 9 to the servo object
  neck.write(90);
  head.attach(11);
  head.write(85);
  pinMode(motor_switch, OUTPUT);
  neck_pos = 90;
  head_pos = 75;
  delay(1000);
  Serial.begin(9600);
  BTSerial.begin(9600);

}

int look_left(int amount, int speed){
  //Relative to a person looking at the camera
  digitalWrite(motor_switch, HIGH);
  int delay_time = 1000/speed;
  if(neck_pos-amount >= 0){
    for(i=0; i<amount; i++){
      neck_pos -= 1;
      neck.write(neck_pos);
      delay(delay_time);
    }
  }
  digitalWrite(motor_switch, LOW);
}

int look_right(int amount, int speed){
  //Relative to a person looking at the camera
  digitalWrite(motor_switch, HIGH);
  int delay_time = 1000/speed;
  if(neck_pos+amount <=180){
    for(i=0; i<amount; i++){
      neck_pos += 1;
      neck.write(neck_pos);
      delay(delay_time);
    }
  }
  digitalWrite(motor_switch, LOW);
}

int look_up(int amount, int speed){
  digitalWrite(motor_switch, HIGH);
  int delay_time = 1000/speed;
  if(head_pos+amount <=180){
    for(i=0; i<amount; i++){
      head_pos += 1;
      head.write(head_pos);
      delay(delay_time);
    }
  }
  digitalWrite(motor_switch, LOW);
}

int look_down(int amount, int speed){
  digitalWrite(motor_switch, HIGH);
  int delay_time = 1000/speed;
  if(head_pos+amount <=180){
    for(i=0; i<amount; i++){
      head_pos -= 1;
      head.write(head_pos);
      delay(delay_time);
    }
  }
  digitalWrite(motor_switch, LOW);
}

/*

  void loop(){
  time
  Motor neck_motor;
  Motor head_motor;

  //Possibly create a new command queue
  handle_commands();

  //Grab the next commands of the queue and if there are any, then execute
  neck_motor.step()
  head_motor.step()

  }
*/

void loop() {

  if(BTSerial.available() > 0){
    instruction = BTSerial.parseInt();
    amount = BTSerial.parseInt();
    thespeed = BTSerial.parseInt();
    BTSerial.read();

    switch(instruction){
    case 0:
      look_left(amount, thespeed);
      BTSerial.println(neck_pos);
      break;
    case 1:
      look_right(amount, thespeed);
      BTSerial.println(neck_pos);
      break;
    case 2:
      look_up(amount, thespeed);
      BTSerial.println(head_pos);
      break;
    case 3:
      look_down(amount, thespeed);
      BTSerial.println(head_pos);
      break;
    case 4:
      digitalWrite(motor_switch, HIGH);
      break;
    case 5:
      digitalWrite(motor_switch, LOW);
      break;
    default:
      break;
    }
  }
}

