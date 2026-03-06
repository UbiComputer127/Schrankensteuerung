#pragma once

#include <Servo.h>
#include <Arduino.h> 

class Barrier_Class
{

public:

enum Barrier_Enum
{
    BARRIER_UNKNOWN = 1,
    BARRIER_OPEN,
    BARRIER_CLOSED,
    BARRIER_TO_OPEN,
    BARRIER_TO_CLOSE,
    BARRIER_ROCK
};

  Barrier_Class(unsigned int Port, const int EepromAdress);

  void Init();

  void process();

  Barrier_Enum getState(); 

  void Open();

  void Close();

private:

unsigned int Port_;
Barrier_Enum CurrentState;
Servo Servo_;
unsigned int Position;
static const unsigned int PositionMax = 90;
static const unsigned int PositionMin = 0;
unsigned long Timer;
static const unsigned long Intervall = 30;
static const unsigned int RockIndexMax = 36; 
const uint8_t RockValues[RockIndexMax]={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 1, 2, 4, 2, 1, 0};
unsigned int RockIndex;
const int EepromAddress_;
};

extern Barrier_Class Barrier1_Object;
extern Barrier_Class Barrier2_Object;
