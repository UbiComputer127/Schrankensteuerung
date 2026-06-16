#pragma once

class Loudness_Class
{
 public:

    Loudness_Class(unsigned int I2cClk, unsigned int I2cData);

    void Init();

    void setLoudness(char Value);

private:

    void I2cStart();

    void I2cStop();

    void I2cWriteByte(char Value);

unsigned int I2cClk_;
unsigned int I2cData_;


};
