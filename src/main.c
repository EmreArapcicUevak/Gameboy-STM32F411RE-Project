#include <stm32f411xe.h>

// Custom Buttons PA0 [left], PA1 [right], PC2[up], PC3[down]
// Custom Button PA0 connected to EXTI0
// Built-in LED PA5

void EXTI0_IRQHandler(void) {
  if (EXTI->PR & EXTI_PR_PR0) {      // Check if line 0 is pending
      EXTI->PR |= EXTI_PR_PR0;       // Clear the pending bit by writing 1

      GPIOA->ODR ^= GPIO_ODR_OD5;       // Toggle LED on PA5
  }
}

void EXTI1_IRQHandler(void) {
  if (EXTI->PR & EXTI_PR_PR1) {      // Check if line 1 is pending
      EXTI->PR |= EXTI_PR_PR1;       // Clear the pending bit by writing 1

      GPIOA->ODR ^= GPIO_ODR_OD5;        // Toggle LED on PA5
  }
}

void EXTI2_IRQHandler(void) {
  if (EXTI->PR & EXTI_PR_PR2) {      // Check if line 2 is pending
      EXTI->PR |= EXTI_PR_PR2;       // Clear the pending bit by writing 1

      GPIOA->ODR ^= GPIO_ODR_OD5;        // Toggle LED on PA5
  }
}

void EXTI3_IRQHandler(void) {
  if (EXTI->PR & EXTI_PR_PR3) {      // Check if line 3 is pending
      EXTI->PR |= EXTI_PR_PR3;       // Clear the pending bit by writing 1

      GPIOA->ODR ^= GPIO_ODR_OD5;        // Toggle LED on PA5
  }
}

int main(){
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  GPIOA->MODER |= (0b01 << GPIO_MODER_MODER5_Pos); // Set PA5 as output, PA0, PA1, PA2, and PA3 is input by default
  GPIOA->PUPDR |= (0b01 << GPIO_PUPDR_PUPD0_Pos) | (0b01 << GPIO_PUPDR_PUPD1_Pos); // Set the PA0, PA1  with a pull up resistor
  GPIOC->PUPDR |= (0b01 << GPIO_PUPDR_PUPD2_Pos) | (0b01 << GPIO_PUPDR_PUPD3_Pos); // Set the PC2, PC3 with a pull up resistor


  EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2 | EXTI_RTSR_TR3; // Enable rising trigger detection

  SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0_Msk | SYSCFG_EXTICR1_EXTI1_Msk | SYSCFG_EXTICR1_EXTI2_Msk | SYSCFG_EXTICR1_EXTI3_Msk); // Set EXTI0, EXTI1, EXTI2, and EXTI3 lines to read from port A (reset the register)
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC | SYSCFG_EXTICR1_EXTI3_PC; // Set  EXTI2, and EXTI3 lines to read from port C

  EXTI->IMR |= EXTI_IMR_IM0 | EXTI_IMR_IM1 | EXTI_IMR_IM2 | EXTI_IMR_IM3; // Unmask the EXTI0 input so it can be listened to.
  //GPIOA->LCKR |= GPIO_LCKR_LCK0 | GPIO_LCKR_LCK1 | GPIO_LCKR_LCK2 | GPIO_LCKR_LCK3 | GPIO_LCKR_LCK5 | GPIO_LCKR_LCKK; // Lock pins PA0 and PA5

  NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_EnableIRQ(EXTI2_IRQn);
  NVIC_EnableIRQ(EXTI3_IRQn);

  while (1) {
    // main logic here
  }
  return 0;
}