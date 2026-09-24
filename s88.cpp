#include "S88.hpp" 
#include <Arduino.h>

uint16_t S88_Class::OutputData_(0);
uint16_t S88_Class::CurrentValue(0);
uint16_t S88_Class::ShiftValue(0);
uint16_t S88_Class::InputData_(0);

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
    digitalWrite(OutputData_, 0);   // Pin ist negiert und so wird der Ausgang auf 1 gesetzt
    pinMode(InputData_, INPUT);
    pinMode(InputLoad_, INPUT);
    attachInterrupt(digitalPinToInterrupt(InputLoad_), S88_Class::RisingLoad, RISING);
    pinMode(InputClock_, INPUT);
    attachInterrupt(digitalPinToInterrupt(InputClock_), S88_Class::FallingClock, FALLING);
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

void S88_Class::FallingClock()
{
    if (ShiftValue & 0x8000)
    {
        digitalWrite(OutputData_, 0);
    }
    else
    {
        digitalWrite(OutputData_, 1);
    }
    ShiftValue <<= 1;
    
    // Daten einlesen
    bool InputData = digitalRead(InputData_);

    if (InputData > 0)
    {
        ShiftValue |= 0x01;
    } 
}

S88_Class S88_Object(D2, D1, D13, D12);

