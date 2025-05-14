#ifndef BATTERY_INDICATOR_H
#define BATTERY_INDICATOR_H

#include "stm32f411xe.h"
#include "Color.h"

void initilize_battery_indicator(void);
void set_battery_indicator_color_rgb(const uint8_t red, const uint8_t green, const uint8_t blue);
void set_battery_indicator_color(const Color color);

#endif