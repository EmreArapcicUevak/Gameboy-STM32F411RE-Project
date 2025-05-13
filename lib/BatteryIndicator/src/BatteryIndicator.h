#ifndef BatteryIndicator
#define BatteryIndicator

#include "stm32f411xe.h"

void initilize_battery_indicator(void);
void set_battery_indicator_color(const uint8_t red, const uint8_t green, const uint8_t blue); // Change the battery indicator color, uses the RGB(255,255,255) notation

#endif