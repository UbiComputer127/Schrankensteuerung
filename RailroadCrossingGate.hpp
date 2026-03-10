#ifndef RAILROADCROSSING_CLASS_HPP
#define RAILROADCROSSING_CLASS_HPP

class RailroadCrossingGate_Class
{
  private:
  enum Switch_Enum
  {
      SETUP = 1,            // Setup beim Starten des Rechners, weisse LED blinkt
      IDLE,                 // Schranke offen und betriebsbereit, weisses Licht blinkt
      START_RED_LIGHT,      // weisses Licht geht aus, rote Leds blinken, Glocke läutet
      WAIT_TO_CLOSE_BARRIER,
      START_CLOSE_BARRIER,  // swarten, das Schranken zu gehen
      CLOSE_BARRIER,        // Schranke schliesst
      CLOSED_BARRIER,       // Schranke ist geschlossen
      OPEN_BARRIER,          // Schranke öffnet
      WAIT_RED_LEDS_OFF     // warten, dass rote Leds aus sind
  };

  Switch_Enum Switch = IDLE;
  unsigned long Timer;

  const static unsigned long TimeShutofWhiteLed = 2000;
  const static unsigned long TimeToCloseBatrrier = 2000;
  const static unsigned long TimeToWaitCloseBarrier = 3000;
  const static unsigned long TimeCloseBarriers = 2000;

  public:

  void Init();

  void process();

};

extern RailroadCrossingGate_Class RailroadCrossingGate_Object;

#endif // define RAILROADCROSSING_CLASS_HPP