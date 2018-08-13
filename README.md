# Choreo
## Android/AVR library to create complex servo motor choreographies using a serial protocol 

## Table of Contents
* Why this library?
* High Level Overview
* Protocol Written in Raw Bytes
* How to Run Demo
* How to Include into your own Project
* Possible Modifications

### Why this library?

The official Arduino Servo library is great for sending single positions to a servo. However, another level of abstraction is needed if you wish to control the velocity and acceleration of the movement and synchronize it across multiple motors. The Arduino's limited RAM must also be seriously considered

Choreo provides this abstraction by procedurally generating servo positions based on high level commands issued through its serial protocol

### High Level Overview

Choreo allows the programmer to efficiently say:

```
Move motor0 20 degrees with constant velocity, taking .5 seconds to do so
Then move motor0 20 degrees with constant velocity, taking 1.5 seconds to do so
Simultaneously, move motor 1 -45 degrees taking 2 seconds to do so
```
Each one of these 3 commands is called a *procedure*

Each motor's procedure buffer runs independently, so the above set of procedures would take 2 seconds total to run.

The machine readable protocol documented below and is written in raw bytes.

### Limitations:

* Right now the library only supports constant velocity, however I plan on writing other interpolation functions to ease the movement of the motors in and out. 

* Each motor is limited to a buffer of 12 procedures on the ArduinoUno due to its low SRAM limitations.

### Protocol: Written as Raw Bytes
```
  num_motors_in_command (1 byte)
    motor_id (1 byte)
      num_procedures_for_this_motor (1 byte)
        procedure_id (1 byte)
        amount in deg (2 bytes)
        duration in ms (2 bytes)
        .
        .
    motor_id
      num_procedures_for_this_motor
        procedure_id
        amount
        duration
        .
        .

```

### How to Run Demo 

Connect two servo motors. One to pin 10 and one to pin 11.
These pins can be changed. See *Possible Modifications* below.

```
cd <dir for all your scrap projects>
git clone https://github.com/SlightlyCyborg/choreo.git
```
Open up the Arduino Editor and upload the choreo/choreo.ino
Take note of the port the Arduino was attached on as you will need this later.

I wrote Clojure client code to test the serial protocol.

Make sure clojure is installed with [lein](https://github.com/technomancy/leiningen)

If the port was not /dev/ttyACM0...change this code...

```
;choreo-tester/src/choreo_tester/core.clj

(ns choreo-tester.core
  (:use [serial.core]
        [serial.util])
  (:import [java.nio ByteBuffer]))

(def s (open "/dev/ttyACM0" :baud-rate 9600))
;       CHANGE THIS ^^

```

Then...run..

```
$ cd choreo-tester
$ lein run
```

You should see the two motors move in sync.

### How to Include into your own Project

Make ~/sketchbook/libraries if it doesn't already exist
```
cd ~
mkdir sketchbook
mkdir libraries
```

CD into libraries & clone the choreo repo
```
cd ~/sketchbook/libraries
git clone https://github.com/SlightlyCyborg/choreo.git
```

Copy choreo.ino into your project folder and modify it, making sure to change the name to the name of the folder it is in

```
mkdir ~/<myproject>
cp ~/sketchbook/libraries/choreo/choreo.ino ~/<myproject>/<myproject>.ino
```

Make whatever edits you want to the file and upload it to your arduino.

Write the client code that will send the commands to the arduino.
Look at *Protocol: Written as Raw Bytes* above for details.


### Possible Modifications
If you have 2 servos connected to pins 10 and 11, then you can use the choreo.io demo as is.

Some modifications you might want to make:

1) Change the number of motors you have connected
2) Change pins of your motors
3) Change the Serial to BTSerial

change choreo.h
```
Motor *motors[NUMBER_OF_MOTORS];
  //ADD OR SUBTRACT MOTORS ^^^

void choreo_setup(){
  motors[0] = new Motor(0,10,90); //Pin 10 is the servo signal pin
  motors[1] = new Motor(1,11,70); //Pin 11 is the servo signal pin
  
  //ADD OR SUBTRACT MOTORS ^^^ OR CHANGE PINS
  // Motor(id, pin, starting position)

  pinMode(MOTOR_SWITCH_PIN, OUTPUT);
  digitalWrite(MOTOR_SWITCH_PIN, LOW);

}
```

change constants.h

```
#define PROC_PER_MOTOR 12
#define MOTOR_SWITCH_PIN 8
#define NUMBER_OF_MOTORS 2

#define USE_BTSERIAL true
//CHANGE this          ^^     to true if using BTSerial like me!

#if USE_BTSERIAL
SoftwareSerial BTSerial(5, 6);
#define MY_SERIAL BTSerial

#else
#define MY_SERIAL Serial

#endif
```

Also note, that I like to use a set of power transistors to turn power on/off to the servos.
I toggle off the power whenever the motors aren't moving. 
My application is fine with this, but yours might not be.
This wont work if you don't have power transistors wired in.
However, if you wish to change how this toogles, then feel free to mess with this code

```
//choreo.h

void choreo_loop(){
  handle_command();
  active = false;
  for(i=0;i<NUMBER_OF_MOTORS;i++){
    motors[i]->step();
    active = active || motors[i]->is_active();
  }
  if(!active){
    digitalWrite(MOTOR_SWITCH_PIN, LOW);
    //   COMMENT OUT THIS LINE ^^ TO STOP TOGGLING OF MOTOR POWER
  }
}
```



### Contributing

#### Please learn these contribution _rules_ so you can _break them like a pro_. 

1. Keep the repo **clutter free**. Use a **global gitignore for emacs** by following these [instructions](https://help.github.com/articles/ignoring-files/#create-a-global-gitignore) and using this [ignore template](https://github.com/github/gitignore/blob/master/Global/Emacs.gitignore)
2. Make modular commits. *Do not use `git add .` when staging, but rather use `git add <files_i_edited>`* unless your worktree is clean of irrelevant cruft not commented in the commit msg.

## License


Copyright © 2017, SlightlyCyborg

Distributed under the MIT License.
