#pragma once

#include <Arduino.h> 

class S88_Class
{
    public:

    S88_Class(unsigned int InputClock, unsigned int InputLoad, unsigned int InputData, unsigned int OutputData);
    void init();
    
    static void RisingLoad();
    static void RisingClock();
    
    void setValue(bool Value, uint16_t Mask);

    private:

    enum Switch_Enum
    {
        Idle = 0,
        ReadOut
    };

    Switch_Enum  Switch;
    unsigned int InputClock_;
    unsigned int InputLoad_;
    static uint16_t InputData_;
    static uint16_t OutputData_;
    static uint16_t CurrentValue;
    static uint16_t ShiftValue;
    static uint16_t InputData;
};

extern S88_Class S88_Object;