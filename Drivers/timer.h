#ifndef Timer_h
#define Timer_h

#include "stm32f4xx.h"

class Timer {
 public:
  static Timer* timer_;
  void (*callbackTimer2_)();
  void (*callbackTimer3_)();

  void init();
  void start2(uint32_t freq, void (*callback)());
  void start3(uint32_t freq, void (*callback)());

 private:
};

#endif
