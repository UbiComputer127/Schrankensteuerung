#ifndef LEDS_CLASS_HPP
#define LEDS_CLADD_HPP

class Leds_Class
{
  private:

  static const unsigned long WhiteLedOn = 500;
  static const unsigned long WhiteLedOff = 800;
  static const unsigned long RedLedsswitch = 500;

  public:

  enum LedsState_Enum
  {
      OFF = 0,
      ON_WHITE = 1,
      ON_RED = 2
  };

  private:
  
  LedsState_Enum LedsState;
  bool LedOnWhite;
  bool LedOnRed;

  unsigned long TriggerTime;

  unsigned int PortLed1_;     // white Led
  unsigned int PortLed2_;     // red Led and white Led
  unsigned int PortLed3_;     // rerd Led

  bool WhiteLedSmartActive;
  bool RedLedSmartActive;
  public:

  Leds_Class(unsigned int PortLed1, unsigned int PortLed2, unsigned int PortLed3);

  void InitLeds();

  void process();

  void setWhiteLedActive();

  void setRedLedsActive();

  void setLedsOff();

  void setWhiteLedSmartOff();

  void setRedLedSmartOff();
  
  LedsState_Enum getLedsState();

};

extern Leds_Class Leds_Object1;
extern Leds_Class Leds_Object2;

#endif // define LEDS_CLASS_HPP