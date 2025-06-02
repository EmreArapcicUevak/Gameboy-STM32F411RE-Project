#ifndef my_Display_ILI9341
#define my_Display_ILI9341

#include "stm32f411xe.h"
#include "ComputerUART.h"

void init_display(void); // Initilize pins for the display
void write_command(uint8_t command); // Writes a command to the display
void send_data(uint8_t data); // sends data to the display
void send_multiple_data(uint8_t *data_arr, const unsigned int size); // send multiple data to the display

static inline void set_column_address(uint16_t x0, uint16_t x1) {
  write_command(0x2A);
  uint8_t data[] = {
      (x0 >> 8) & 0xFF, x0 & 0xFF,
      (x1 >> 8) & 0xFF, x1 & 0xFF
  };
  send_multiple_data(data, 4);
}

static inline void set_page_address(uint16_t y0, uint16_t y1) {
  write_command(0x2B);
  uint8_t data[] = {
      (y0 >> 8) & 0xFF, y0 & 0xFF,
      (y1 >> 8) & 0xFF, y1 & 0xFF
  };
  send_multiple_data(data, 4);
}

static inline void start_memory_write(void) {
  write_command(0x2C);
}

static inline void set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  set_column_address(x0, x1);
  set_page_address(y0, y1);
  start_memory_write();
}
#endif