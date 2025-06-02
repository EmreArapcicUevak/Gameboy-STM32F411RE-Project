#ifndef BUTTON_RIGHT_H
#define BUTTON_RIGHT_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonRight_triggered(void) {
    // Handle Button Right pressed event
    // This function will be called when Button Right is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Right pressed"); // Example
}

#endif
