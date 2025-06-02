#ifndef BUTTON_A_H
#define BUTTON_A_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonA_triggered(void) {
    // Handle Button A pressed event
    // This function will be called when Button A is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button A pressed"); // Example
}

#endif