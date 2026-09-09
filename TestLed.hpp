#pragma once

class TestLed_Class
{
public:

    TestLed_Class(int Port);

    void init();

    void process();

    void LedOff();

    void LedBlinking();

    void LedOn();

private:

  static const unsigned long Time_ = 500; 
  unsigned long Timer_;

  int Port_;            // Number of Output Port 
  bool LedIsBlinking_;  // If LED is blinking
  bool PortIsOn_;       // State of Port during LED is blinking 

};

extern TestLed_Class TestLed_Object;
