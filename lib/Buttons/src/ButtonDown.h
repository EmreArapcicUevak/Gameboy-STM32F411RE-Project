#ifndef BUTTON_DOWN_H
#define BUTTON_DOWN_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonDown_triggered(void) {
    // Handle Button Down pressed event
    // This function will be called when Button Down is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Down pressed"); // Example
}

#endif
