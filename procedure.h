#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "constants.h"

class Procedure{
 public:
  virtual void proceed(){
  };
};

class LinearProcedure{
public:
  LinearProcedure() {};
  LinearProcedure(byte id_, short amount_, short duration_)
  {
    amount = amount_;
    duration = duration_;
    id=id_;
  };
  short proceed(short time_elapsed)
  {
    if(time_elapsed < duration){
      return (float)amount/(float)duration * time_elapsed;
    }else{
      return amount;
    }
  };

  byte id;
  short amount;
  short duration;
};

#endif

