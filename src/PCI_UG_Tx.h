#ifndef PCI_UG_Tx_H
#define PCI_UG_Tx_H

#include "Arduino.h"

class PCI_UG_Tx {

  public:

  PCI_UG_Tx(uint8_t t0, uint8_t t1);
  void begin();
  void runStep(uint8_t n, uint32_t steps, uint8_t velstep, boolean cwstep);
  void runDC(uint8_t n, uint32_t time, uint8_t veldc, boolean cwdc);
  void beep(int xbnum, int xbdur, int xbfreq, int xbinter);
  void led(int xlnum, int xldur, int xlinter);
  void setms(uint32_t yms);
  void stopStep(uint8_t n);
  void stopDC(uint8_t n);
  void stopBeep();
  void stopLed();

  uint32_t getms();
  uint32_t where(uint8_t n);

  int bdur=500, binter=500, bfreq=500;
  volatile int bnum=0;

  int ldur=500, linter=500;
  volatile int lnum=0;

  volatile uint32_t xms=0;

  volatile uint8_t xtipostep[2]={2,2};
  volatile uint32_t   xsteps[2]={0,0};
  volatile uint32_t xvelstep[2]={0,0};
  volatile uint32_t   xusnow[2]={0,0};
  volatile uint32_t   xusult[2]={0,0};
  volatile boolean   xcwstep[2]={1,1};
  volatile int         xfase[2]={0,0};
  volatile uint32_t  xvelnow[2]={0,0};

  volatile uint32_t    xtime[4]={0,0,0,0};
  volatile uint32_t   xveldc[4]={50,50,50,50};
  volatile boolean     xcwdc[4]={1,1,1,1};

  private:

  void onTimer100us();
  void go();
  void move0();
  void move1();
  void move2();
  void move3();
  void move4();
  void writ(uint8_t px1, uint8_t px2, uint8_t px3, uint8_t px4);
  static PCI_UG_Tx *isrTable[];
  using isrFunct = void (*)();
  template<uint8_t NUM_INTERRUPTS = SOC_TIMER_GROUP_TOTAL_TIMERS>
  static isrFunct getIsr(uint8_t timerNumber);

  //variaveis de controle do beep
  volatile bool bxpausa=false, bxpri=true;
  volatile int bxinter=0, bxdur=0;

  //variaveis de controle do led
  volatile bool lxpausa=false, lxpri=true;
  volatile int lxinter=0, lxdur=0;

  //variaveis de controle dos step motors
  uint16_t passos[5]={ 0, 2048, 2048, 4096, 200};
  int      ajuste[5]={ 0,   28,   28,   28,-806};

  //variaveis de controle dos motores DC
  uint8_t pinosdc[4][3]={ {18,19,15}, {16,17,14}, {33,23,25}, {13,27,26} };
  uint8_t k=0;
};


template<uint8_t NUM_INTERRUPTS>
PCI_UG_Tx::isrFunct PCI_UG_Tx::getIsr(uint8_t timerNumber) {
  if (timerNumber == (NUM_INTERRUPTS - 1)) {
    return [] {
      isrTable[NUM_INTERRUPTS - 1]->onTimer100us();
    };
  }
  return getIsr < NUM_INTERRUPTS - 1 > (timerNumber);
}

template<>
inline PCI_UG_Tx::isrFunct PCI_UG_Tx::getIsr<0>(uint8_t timerNumber) {
  (void) timerNumber;
  return nullptr;
}

#endif

