#include <stm32f411xe.h>

// Custom Button PA0
// Built-in LED PA5
int main(){
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  GPIOA->MODER |= (0b01 << GPIO_MODER_MODER5_Pos); // Set PA5 as output, PA0 is input by default
  GPIOA->PUPDR |= (0b01 << GPIO_PUPDR_PUPD0_Pos); // Set the PA0 with a pull up resistor
  GPIOA->LCKR |= GPIO_LCKR_LCK0 | GPIO_LCKR_LCK5 | GPIO_LCKR_LCKK; // Lock pins PA0 and PA5

  while (1) {
    while ((GPIOA->IDR & GPIO_IDR_ID0) != 0);
    GPIOA->ODR |= GPIO_ODR_OD5;
    while ((GPIOA->IDR & GPIO_IDR_ID0) != GPIO_IDR_ID0);
    GPIOA->ODR &= ~GPIO_ODR_OD5_Msk;
  }
  return 0;
}