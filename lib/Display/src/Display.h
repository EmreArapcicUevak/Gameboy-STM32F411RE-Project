#ifndef my_Display_ILI9341
#define my_Display_ILI9341

#include "stm32f411xe.h"

void init_display(void); // Initilize pins for the display
void write_command(uint8_t command); // Writes a command to the display
void send_data(uint8_t data); // sends data to the display
void send_multiple_data(uint8_t *data_arr, const unsigned int size); // send multiple data to the display


#endif