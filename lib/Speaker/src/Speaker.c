#include "Speaker.h"


// PWM PB10 Timer2_CH3 for the audio
void init_speaker(void){ // initilize speaker
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable timer 2

  GPIOB->MODER |= (0b10 << GPIO_MODER_MODE10_Pos); // Alt Func
  GPIOB->AFR[1] |= (0b0001 << GPIO_AFRH_AFSEL10_Pos); // Set PB10 to alternative function AF1 which is the TIM2_CH3
  GPIOB->OSPEEDR |= (0b10 << GPIO_OSPEEDR_OSPEED10_Pos); // PB10 Fast Speed output

  TIM2->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS;
  TIM2->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE;
  TIM2->CCER |= TIM_CCER_CC3E; // enable CC3
  TIM2->PSC = 0; // Set prescaler (assuming 16 MHz clock -> unchanged)
  TIM2->ARR =61115;   // Auto-reload value (period = 1000 counts --> 1s/1000 = 1ms --> 1kHz PWM)
  TIM2->CCR3 = (TIM2->ARR + 1) / 2;  // 50% duty cycle
  TIM2->EGR |= TIM_EGR_UG;
  TIM2->CR1 |= TIM_CR1_CEN; // Turn on the timer
}