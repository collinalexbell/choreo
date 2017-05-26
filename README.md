# Choreo
## Android/AVR library to create complex motor choreographies using a serial protocol 


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

### Contributing

#### Please learn these contribution _rules_ so you can _break them like a pro_. 

1. Keep the repo **clutter free**. Use a **global gitignore for emacs** by following these [instructions](https://help.github.com/articles/ignoring-files/#create-a-global-gitignore) and using this [ignore template](https://github.com/github/gitignore/blob/master/Global/Emacs.gitignore)
2. Make modular commits. *Do not use `git add .` when staging, but rather use `git add <files_i_edited>`* unless your worktree is clean of irrelevant cruft not commented in the commit msg.

## License


Copyright © 2017, SlightlyCyborg

Distributed under the MIT License.
