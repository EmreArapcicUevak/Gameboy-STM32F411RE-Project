#ifndef BUTTON_START_H
#define BUTTON_START_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonStart_triggered(void) {
    // Handle Button Start pressed event
    // This function will be called when Button Start is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Start pressed"); // Example
}

#endif
