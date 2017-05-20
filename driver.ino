#include <Servo.h>
#include <SoftwareSerial.h>

Servo neck;  // create servo object to control a servo
Servo head;
// twelve servo objects can be created on most boards

int foo = 0;
int neck_pos = 0;    // variable to store the servo position
int head_pos = 0;
int motor_switch = 8;
bool g = true;
int i;
int instruction, amount, thespeed;

SoftwareSerial BTSerial(6, 5); // RX | TX

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

void loop() {
  if(false){
    neck.write(90);
    neck_pos = 90;
    delay(5000);
    look_left(40, 10);
    delay(1000);
    look_right(80, 10);
    delay(1000);
    g = false;
  }

  if(BTSerial.available() > 0){
    //instruction = BTSerial.parseInt();
    instruction = BTSerial.parseInt();
    amount = BTSerial.parseInt();
    thespeed = BTSerial.parseInt();
    BTSerial.read();
    // BTSerial.print(instruction);
    //BTSerial.print(",");
    //BTSerial.print(amount);
    //BTSerial.print(",");
    //BTSerial.print(thespeed);
    //BTSerial.print("\n");

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

