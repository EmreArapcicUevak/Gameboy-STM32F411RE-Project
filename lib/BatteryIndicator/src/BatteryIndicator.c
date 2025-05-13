#include "BatteryIndicator.h"

void initilize_battery_indicator(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; // Enable timer 4

  GPIOB->MODER |= (0b10 << GPIO_MODER_MODE6_Pos) | (0b10 << GPIO_MODER_MODE7_Pos) | (0b10 << GPIO_MODER_MODE8_Pos); // Set PB6,7,8 as alt functions
  GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos) | (2 << GPIO_AFRL_AFSEL7_Pos); // Set PB6,7 to alternative function AF2
  GPIOB->AFR[1] |= (2 << GPIO_AFRH_AFSEL8_Pos); // Set PB8 to alternative function AF2
  GPIOB->OSPEEDR |= (0b10 << GPIO_OSPEEDR_OSPEED6_Pos) | (0b10 << GPIO_OSPEEDR_OSPEED7_Pos) | (0b10 << GPIO_OSPEEDR_OSPEED8_Pos); // PB6,7,8 Fast Speed output

  TIM4->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS;
  
  // Enable capture compare 1,2,3 channels and make them work as output PWM mode 1
  TIM4->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos) | (0b110 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
  TIM4->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE;

  TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E; // enable CC1,2,3

  TIM4->PSC = 15; // Set prescaler (assuming 16 MHz clock -> 1MHz)
  TIM4->ARR =999;   // Auto-reload value
  TIM4->CCR1 = TIM4->CCR2 = TIM4->CCR3 = 1000; // 100% duty cycle by default

  TIM4->EGR |= TIM_EGR_UG; // Generate an update event just in case to reset the timer of previous settings
  TIM4->CR1 |= TIM_CR1_CEN; // Turn on the timer
}

void set_battery_indicator_color(const uint8_t red, const uint8_t green, const uint8_t blue) {
  TIM4->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E); // disalbe CC1,2,3
  
  TIM4->CCR1 = 1000 * red / 255;
  TIM4->CCR2 = 1000 * green / 255;
  TIM4->CCR3 = 1000 * blue / 255;

  TIM4->EGR |= TIM_EGR_UG; // Generate an update event just in case to reset the timer of previous settings
  TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E; // enable CC1,2,3
}