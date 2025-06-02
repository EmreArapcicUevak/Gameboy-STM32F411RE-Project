#ifndef BUTTON_LEFT_H
#define BUTTON_LEFT_H

#include "stm32f411xe.h"
#include "ComputerUART.h"

static void ButtonLeft_triggered(void) {
    // Handle Button Left pressed event
    // This function will be called when Button Left is pressed
    // You can add your custom logic here, such as sending a signal or toggling an LED
    // Register clearing will be done automatically so you don't need to worry about it
    uart2_println("Button Left pressed"); // Example
}

#endif
