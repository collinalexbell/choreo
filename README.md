# Choreo
## Android/AVR library to create complex servo motor choreographies using a serial protocol 

### How to use

If you have 2 servos connected to pins 10 and 11, then you can use the choreo.io demo as is.

Some modifications you might want to make:

1) Change the number of motors you have connected
2) Change pins of your motors
3) Change the Serial to BTSerial

change choreo.h
```
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
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define PROC_PER_MOTOR 10
#define MOTOR_SWITCH_PIN 8
#define NUMBER_OF_MOTORS 2
//CHANGE THIS PERHAPS   ^^

#define SERIAL Serial
//CHANGE this  ^^     to BTSerial if using BTSerial like me!
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

### Protocol: Written as raw bytes
```
  num_motors_in_command
    motor_id
      num_procedures_for_this_motor
        procedure_id
        amount
        duration
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

### Demo protocol send code

I wrote clojure code to test the protocol.

Make sure clojure is installed with [lein](https://github.com/technomancy/leiningen)

Make sure your Arduino is attached to /dev/ttyACM0 and has the choreo sketch loaded.

If the Arduino is not attached to /dev/ttyACM0...change this code...


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

You should see the two motors move

### Contributing

#### Please learn these contribution _rules_ so you can _break them like a pro_. 

1. Keep the repo **clutter free**. Use a **global gitignore for emacs** by following these [instructions](https://help.github.com/articles/ignoring-files/#create-a-global-gitignore) and using this [ignore template](https://github.com/github/gitignore/blob/master/Global/Emacs.gitignore)
2. Make modular commits. *Do not use `git add .` when staging, but rather use `git add <files_i_edited>`* unless your worktree is clean of irrelevant cruft not commented in the commit msg.

## License


Copyright © 2017, SlightlyCyborg

Distributed under the MIT License.
