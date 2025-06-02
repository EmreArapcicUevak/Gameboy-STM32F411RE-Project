#ifndef BUTTON_B_H
#define BUTTON_B_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonB_triggered(void) {
    // Handle Button B pressed event
    // This function will be called when Button B is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button B pressed"); // Example
}

#endif