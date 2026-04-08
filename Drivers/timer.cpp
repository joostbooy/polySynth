#include "timer.h"

Timer* Timer::timer_;

void Timer::init() {
  timer_ = this;
}

void Timer::start2(uint32_t freq, void (*callback)()) {
  callbackTimer2_ = callback;

  TIM_HandleTypeDef TimHandle;
  TimHandle.Instance = TIM2;
  TimHandle.Init.Period = (F_CPU / freq / 2) - 1;
  TimHandle.Init.Prescaler = 0;
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  HAL_TIM_Base_Init(&TimHandle);
  HAL_TIM_Base_Start_IT(&TimHandle);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void Timer::start3(uint32_t freq, void (*callback)()) {
  callbackTimer3_ = callback;

  TIM_HandleTypeDef TimHandle;
  TimHandle.Instance = TIM3;
  TimHandle.Init.Period = (F_CPU / freq / 2) - 1;
  TimHandle.Init.Prescaler = 0;
  TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  HAL_TIM_Base_Init(&TimHandle);
  HAL_TIM_Base_Start_IT(&TimHandle);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

extern "C" {
void TIM2_IRQHandler(void) {
  if (!(TIM2->SR & TIM_IT_UPDATE)) {
    return;
  }
  TIM2->SR = ~TIM_IT_UPDATE;
  Timer::timer_->callbackTimer2_();
}

void TIM3_IRQHandler(void) {
  if (!(TIM3->SR & TIM_IT_UPDATE)) {
    return;
  }
  TIM3->SR = ~TIM_IT_UPDATE;
  Timer::timer_->callbackTimer3_();
}
};