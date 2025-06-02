#ifndef BUTTON_SELECT_H
#define BUTTON_SELECT_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonSelect_triggered(void) {
    // Handle Button Select pressed event
    // This function will be called when Button Select is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Select pressed"); // Example
}

#endif
