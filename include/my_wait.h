#ifndef my_wait
#define my_wait

#include "stm32f411xe.h"

void wait(uint32_t miliseconds) {
  TIM4->CR1 &= ~TIM_CR1_CEN; // Stop the timer

  // Configure the timer
  TIM4->ARR = miliseconds - 1;

  TIM4->EGR |= TIM_EGR_UG; // Force an update event for the timer to reset and update the shadow registers
  TIM4->SR &= ~TIM_SR_UIF_Msk; // Clear the IF incase it wasn't cleared before

  TIM4->CR1 |= TIM_CR1_CEN; // Turn on the timer
  while (!(TIM4->SR & TIM_SR_UIF)); // wait for timer to finish
  TIM4->SR &= ~TIM_SR_UIF_Msk;
  TIM4->CR1 &= ~TIM_CR1_CEN; // Stop the timer
}

void init_wait_timer() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  TIM4->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS;
  TIM4->PSC = 15999; // Set timer to run at 1kHz
}

#endif