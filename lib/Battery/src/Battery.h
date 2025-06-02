#ifndef battery_module
#define battery_module

#include "stm32f411xe.h"
#include <Display.h>

void init_battery_ADC(uint16_t *ADCAddress, uint16_t *backupAddress); // Used to init the ADC for the battery
uint16_t read_battery_ADC(void); // Read the recorded battery
float get_battery_voltage(void);

#endif