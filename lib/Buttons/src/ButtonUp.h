#ifndef BUTTON_UP_H
#define BUTTON_UP_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonUp_triggered(void) {
    // Handle Button Up pressed event
    // This function will be called when Button Up is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Up pressed"); // Example
}

#endif
