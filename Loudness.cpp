#include "Loudness.hpp"
#include <Arduino.h>

Loudness_Class::Loudness_Class(unsigned int I2cClk, unsigned int I2cData)
: I2cClk_(I2cClk)
, I2cData_(I2cData)
{

}

void Loudness_Class::Init()
{
    pinMode(I2cClk_, OUTPUT);   // hier die rote und die Weisse
    pinMode(I2cData_, OUTPUT);   // hier nur die rote
    digitalWrite(I2cClk_, HIGH);
    digitalWrite(I2cData_, HIGH);
}

void Loudness_Class::I2cStart()
{
    digitalWrite(I2cData_, LOW);
    // evtl. warten


    digitalWrite(I2cClk_, LOW);
}

void Loudness_Class::I2cStop()
{

}

void Loudness_Class::I2cWriteByte(char Value)
{
    const char BitMask = 0x80; 
    for (int i=0; i<8; i++)
    {
        // Clk must be low, adjust data
        if (Value & BitMask)
        {
            digitalWrite(I2cData_, HIGH);
        }
        else
        {
            digitalWrite(I2cData_, LOW);
        }
        Value <<= 1;

        // Clk set high
        digitalWrite(I2cClk_, HIGH);
        // wait and Clk set low ...
        digitalWrite(I2cClk_, LOW);
    }
    // here check acknoledge bit
    pinMode(I2cData_, INPUT);
    // Clk set high
    digitalWrite(I2cClk_, HIGH);
    // wait and Clk set low ...
    char Acknoledge = digitalRead(I2cData_);
    // wait 
    digitalWrite(I2cClk_, HIGH);
    // Acknoledge should be low

    pinMode(I2cData_, OUTPUT); 
    digitalWrite(I2cData_, LOW);
}

void Loudness_Class::setLoudness(char Value)
{
    digitalWrite(I2cClk_, HIGH);
    // wait 
    digitalWrite(I2cData_, HIGH);
}