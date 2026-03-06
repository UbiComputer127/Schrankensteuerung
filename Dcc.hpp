#pragma once

#include <Arduino.h> 
#include "Queue.hpp"

class Dcc_Class
{
public:

      struct DccCommand_Struct
      {
            int Length;
            uint8_t Data[8];
      };

      Dcc_Class(unsigned int Input);
      
      void init();

      static void RisingDcc();

      enum DccCommand_Enum
      {
            CommandIdle = 0,
            CommandOpen,
            CommandClose
      };

      bool getDccCommand(DccCommand_Enum & Cmd); 

      enum Switch_Enum
      {
            IDLE = 1,
            SEARCH_PREAMBEL,
            READ_DATA,
            LOW_BYTE,
            XOR_TEST
      };

     

private:

      // Port DCC Signal
      unsigned int Input_;

      // Null-bit: 90 ... 10000 µs
      static const unsigned long NullBitMin = 150;
      static const unsigned long NullBitMax = 10000;

      // Eins-Bit: 50 ... 66 µs
      static const unsigned long EinsBitMin = 80;
      static const unsigned long EinsBitMax = 130; 
      
      static const unsigned long MaxDiff = 10; 
      static unsigned long CurrentTimer_;
      static unsigned long LastTimer_;
      static const uint8_t RawDataMax = 10;
      static const uint16_t MinSyncBits = 10;

      static uint8_t RawData[RawDataMax];
      static uint16_t PreambelBitCount;
      static uint16_t DataIndex;
      static uint16_t BitPosition;
      static Switch_Enum Switch;
      static uint8_t XorTest;
      
      static DccCommand_Enum DccCommand;
};

extern Dcc_Class Dcc_Object;