#ifndef BUTTONS_H
#define BUTTONS_H

#include "stm32f411xe.h"
#include "emulator.h"

// PC 5 Button "A"
// PA 6 Button "B"
// PA 7 Button "Start"
// PB 3 Button "Select"
// PC 13 Button "Right"
// PC 12 Button "Up"
// PC 11 Button "Down"  
// PC 10 Button "Left"
static void init_buttons(void) { // call to init buttons
    // Enable GPIOA and GPIOC clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

    GPIOA->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); // Set PA5, PA6, and PA7 as input
    GPIOB->MODER &= ~(GPIO_MODER_MODER3); // Set PB3 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12 | GPIO_MODER_MODER13); // Set PC10 to PC13 as input and PC5 as input

    GPIOA->PUPDR |= (0b01 << GPIO_PUPDR_PUPD6_Pos) | (0b01 << GPIO_PUPDR_PUPD7_Pos); // Pull-up PA6, and PA7
    GPIOB->PUPDR |= (0b01 << GPIO_PUPDR_PUPD3_Pos); // Pull-up PB3
    GPIOC->PUPDR |= (0b01 << GPIO_PUPDR_PUPD5_Pos) | (0b01 << GPIO_PUPDR_PUPD10_Pos) | (0b01 << GPIO_PUPDR_PUPD11_Pos) | (0b01 << GPIO_PUPDR_PUPD12_Pos) | (0b01 << GPIO_PUPDR_PUPD13_Pos); // Pull-up PC5, PC10 to PC13

    // Configure SYSCFG for external interrupts
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI3_Msk); // Clear EXTI configuration for EXTI3
    SYSCFG->EXTICR[0] |= (0b0001 << SYSCFG_EXTICR1_EXTI3_Pos); // Set PB3 to EXTI3

    SYSCFG->EXTICR[1] |= (0b0010 << SYSCFG_EXTICR2_EXTI5_Pos) | // Set PC5 to EXTI5
                         (0b0000 << SYSCFG_EXTICR2_EXTI6_Pos) | // Set PA6 to EXTI6
                         (0b0000 << SYSCFG_EXTICR2_EXTI7_Pos);   // Set PA7 to EXTI7

    SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI10_Msk | SYSCFG_EXTICR3_EXTI11_Msk); // Clear EXTI configuration for PC10 to PC15
    SYSCFG->EXTICR[2] |= (0b0010 << SYSCFG_EXTICR3_EXTI10_Pos) | // Set PC10 to EXTI10
                         (0b0010 << SYSCFG_EXTICR3_EXTI11_Pos); // Set PC11 to EXTI11

    SYSCFG->EXTICR[3] &= ~(SYSCFG_EXTICR4_EXTI12_Msk |
                           SYSCFG_EXTICR4_EXTI13_Msk); // Clear EXTI configuration for PC12 to PC15
    SYSCFG->EXTICR[3] |= (0b0010 << SYSCFG_EXTICR4_EXTI12_Pos) | // Set PC12 to EXTI12
                         (0b0010 << SYSCFG_EXTICR4_EXTI13_Pos) | // Set PC13 to EXTI13
                         (0b0010 << SYSCFG_EXTICR4_EXTI14_Pos) | // Set PC14 to EXTI14
                         (0b0010 << SYSCFG_EXTICR4_EXTI15_Pos);   // Set PC15 to EXTI15

    EXTI->IMR |= (EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | 
                        EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | 
                        EXTI_IMR_MR13 | EXTI_IMR_MR3); // Enable interrupts for PA5, PA6, PA7, PC10 to PC13, and PB3

    EXTI->FTSR |= (EXTI_FTSR_TR5 | EXTI_FTSR_TR6 | EXTI_FTSR_TR7 | 
                        EXTI_FTSR_TR10 | EXTI_FTSR_TR11 | EXTI_FTSR_TR12 |
                        EXTI_FTSR_TR13 | EXTI_FTSR_TR3); // Falling edge trigger for PA5, PA6, PA7, PC10 to PC15, and PB3

    EXTI->RTSR |= (EXTI_RTSR_TR5 | EXTI_RTSR_TR6 | EXTI_RTSR_TR7 | 
                        EXTI_RTSR_TR10 | EXTI_RTSR_TR11 | EXTI_RTSR_TR12 |
                        EXTI_RTSR_TR13 | EXTI_RTSR_TR3); // Rising edge trigger for PA5, PA6, PA7, PC10 to PC15, and PB3

    NVIC_EnableIRQ(EXTI3_IRQn); // Enable EXTI3 interrupt
    NVIC_EnableIRQ(EXTI9_5_IRQn); // Enable EXTI5 to EXTI9 interrupts
    NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable EXTI10 interrupt
}

void EXTI3_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR3) { // Check if EXTI3 triggered
        if (GPIOB->IDR & GPIO_IDR_ID3) {
          uart2_println("Button Select released");
          set_input(SELECT, false);
        }
        else {
          uart2_println("Button Select pressed");
          set_input(SELECT, true);
        }
        EXTI->PR |= EXTI_PR_PR3; // Clear pending bit
    }
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR5) { // Check if EXTI5 triggered
        if (GPIOC->IDR & GPIO_IDR_ID5) {
          uart2_println("Button A released");
          set_input(A, false);
        } else {
          uart2_println("Button A pressed");
          set_input(A, true);
        }
        EXTI->PR |= EXTI_PR_PR5; // Clear pending bit
    }

    if (EXTI->PR & EXTI_PR_PR6) { // Check if EXTI6 triggered
        if (GPIOA->IDR & GPIO_IDR_ID6) {
            uart2_println("Button B released");
            set_input(B, false);
        } else {
            uart2_println("Button B pressed");
            set_input(B, true);
        }
        EXTI->PR |= EXTI_PR_PR6; // Clear pending bit
    }

    if (EXTI->PR & EXTI_PR_PR7) { // Check if EXTI7 triggered
        if (GPIOA->IDR & GPIO_IDR_ID7) {
            uart2_println("Button Start released");
            set_input(START, false);
        } else {
            uart2_println("Button Start pressed");
            set_input(START, true);
        }
        EXTI->PR |= EXTI_PR_PR7; // Clear pending bit
    }
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR10) { // Check if EXTI10 triggered
        if (GPIOC->IDR & GPIO_IDR_ID10) {
          uart2_println("Button Left released");
          set_input(LEFT, false);
        } else {
            uart2_println("Button Left pressed");
            set_input(LEFT, true);
        }
        EXTI->PR |= EXTI_PR_PR10; // Clear pending bit
    }

    if (EXTI->PR & EXTI_PR_PR11) { // Check if EXTI11 triggered
        if (GPIOC->IDR & GPIO_IDR_ID11) {
            uart2_println("Button Down released");
            set_input(DOWN, false);
        } else {
            uart2_println("Button Down pressed");
            set_input(DOWN, true);
        }
        EXTI->PR |= EXTI_PR_PR11; // Clear pending bit
    }

    if (EXTI->PR & EXTI_PR_PR12) { // Check if EXTI12 triggered
        if (GPIOC->IDR & GPIO_IDR_ID12) {
            uart2_println("Button Up released");
            set_input(UP, false);
        } else {
            uart2_println("Button Up pressed");
            set_input(UP, true);
        }
        EXTI->PR |= EXTI_PR_PR12; // Clear pending bit
    }

    if (EXTI->PR & EXTI_PR_PR13) { // Check if EXTI13 triggered
        if (GPIOC->IDR & GPIO_IDR_ID13) {
            uart2_println("Button Right released");
            set_input(RIGHT, false);
        } else {
            uart2_println("Button Right pressed");
            set_input(RIGHT, true);
        }
        EXTI->PR |= EXTI_PR_PR13; // Clear pending bit
    }
}
#endif