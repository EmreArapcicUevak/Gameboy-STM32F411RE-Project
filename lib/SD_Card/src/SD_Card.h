#ifndef my_SD_Card
#define my_SD_Card

#include "stm32f411xe.h"

void init_SD_Card(void); // Initilizes the SD card
void select_SD(void); // Selects the card via CS
void deselect_SD(void); // Selects the card via CS

#endif