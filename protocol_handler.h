#ifndef PROTOCOL_H
#define PROTOCOL_H


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
 */



#endif
