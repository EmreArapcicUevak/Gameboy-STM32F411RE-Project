#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f411xe.h"

// PA3 Button "A"
// PA2 Button "B"
// PC 15 Button "Start"
// PC 14 Button "Select"
// PC 13 Button "Right"
// PC 12 Button "Up"
// PC 11 Button "Down"  
// PC 10 Button "Left"
static void Button_init(void) { // call to init buttons
    // Enable GPIOA and GPIOC clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3); // Set PA2 and PA3 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 | GPIO_MODER_MODER13 | GPIO_MODER_MODER14 | GPIO_MODER_MODER15); // Set PC10 to PC15 as input

    GPIOA->PUPDR |= (0b01 << GPIO_PUPDR_PUPD2_Pos) | (0b01 << GPIO_PUPDR_PUPD3_Pos); // Pull-up PA2 and PA3
    GPIOC->PUPDR |= (0b01 << GPIO_PUPDR_PUPD10_Pos) | (0b01 << GPIO_PUPDR_PUPD11_Pos) | 
                  (0b01 << GPIO_PUPDR_PUPD12_Pos) | (0b01 << GPIO_PUPDR_PUPD13_Pos) | 
                  (0b01 << GPIO_PUPDR_PUPD14_Pos) | (0b01 << GPIO_PUPDR_PUPD15_Pos); // Pull-up PC10 to PC15

    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI2 | SYSCFG_EXTICR1_EXTI3); // Clear EXTI configuration for PA2 and PA3

    SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI10_Msk | SYSCFG_EXTICR3_EXTI11_Msk); // Clear EXTI configuration for PC10 to PC15
    SYSCFG->EXTICR[2] |= (0b0010 << SYSCFG_EXTICR3_EXTI10_Pos) | // Set PC10 to EXTI10
                         (0b0010 << SYSCFG_EXTICR3_EXTI11_Pos); // Set PC11 to EXTI11

    SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI12_Msk |
                           SYSCFG_EXTICR4_EXTI13_Msk | 
                           SYSCFG_EXTICR4_EXTI14_Msk |
                           SYSCFG_EXTICR4_EXTI15_Msk); // Clear EXTI configuration for PC12 to PC15

    SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI12_Pos) | // Set PC12 to EXTI12
                         (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos) | // Set PC13 to EXTI13
                         (0b0010 << SYSCFG_EXTICR4_EXTI14_Pos) | // Set PC14 to EXTI14
                         (0b0010 << SYSCFG_EXTICR4_EXTI15_Pos);   // Set PC15 to EXTI15

      
    NVIC_EnableIRQ(EXTI2_IRQn); // Enable EXTI2 interrupt
    NVIC_EnableIRQ(EXTI3_IRQn); // Enable EXTI3 interrupt
    NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable EXTI10 interrupt

    EXTI->IMR |= (EXTI_IMR_MR2 | EXTI_IMR_MR3 | EXTI_IMR_MR10 | 
                        EXTI_IMR_MR11 | EXTI_IMR_MR12 | 
                        EXTI_IMR_MR13 | EXTI_IMR_MR14 | 
                        EXTI_IMR_MR15); // Enable interrupts for PA2, PA3, PC10 to PC15

    EXTI->FTSR |= (EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | 
                        EXTI_FTSR_TR10 | EXTI_FTSR_TR11 | 
                        EXTI_FTSR_TR12 | EXTI_FTSR_TR13 | 
                        EXTI_FTSR_TR14 | EXTI_FTSR_TR15); // Falling edge trigger for PA2, PA3, PC10 to PC15
}
#endif