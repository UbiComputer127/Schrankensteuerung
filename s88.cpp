#include "S88.hpp" 
#include <Arduino.h>

uint16_t S88_Class::InputData_(0);
uint16_t S88_Class::OutputData_(0);
uint16_t S88_Class::CurrentValue(0);
uint16_t S88_Class::ShiftValue(0);
uint16_t S88_Class::InputData(0);


S88_Class::S88_Class(unsigned int InputClock, unsigned int InputLoad, unsigned int InputData, unsigned int OutputData)
: InputClock_(InputClock)
, InputLoad_(InputLoad)
{
    InputData_ = InputData;
    OutputData_ = OutputData;
}

void S88_Class::init()
{
      pinMode(OutputData_, OUTPUT);
      pinMode(InputData_, INPUT);
      pinMode(InputLoad_, INPUT);
      attachInterrupt(digitalPinToInterrupt(InputLoad_), S88_Class::RisingLoad, RISING);
      pinMode(InputClock_, INPUT);
      attachInterrupt(digitalPinToInterrupt(InputClock_), S88_Class::RisingClock, RISING);
}

void S88_Class::setValue(bool Value, uint16_t Mask)
{
    if (Value)
    {
        CurrentValue |= Mask;
    }
    else
    {
        CurrentValue &= ~Mask;
    }
}

void S88_Class::RisingLoad()
{
    ShiftValue = CurrentValue;
}

void S88_Class::RisingClock()
{
    if (ShiftValue & 0x80)
    {
        digitalWrite(OutputData_, 1);
    }
    else
    {
        digitalWrite(OutputData_, 0);
    }
    ShiftValue <<= 1;
    
    // Daten einlesen
    InputData = digitalRead(InputData_);

    if (InputData > 0)
    {
        ShiftValue |= 0x01;
    } 
}

S88_Class S88_Object(D12, D13, D11, D4);

