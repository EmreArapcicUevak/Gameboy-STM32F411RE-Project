#include <stm32f411xe.h>
#include "ComputerUART.h"
#include "my_wait.h"
#include <Display.h>
#include <Speaker.h>
#include <SD_Card.h>
#include <Battery.h>
#include <Buttons.h>
#include <emulator.h>

// Built-in LED PA5
int main(){
  uint16_t batteryValues[2];

  uart2_init(115200);
  uart2_println("Programm started");

  init_wait_timer();
  init_display();
  init_speaker();
  init_buttons();
  
  init_battery_ADC(batteryValues, batteryValues + 1);

  write_command(0x29);
  // 320x240
  set_address_window(0,0,320, 240);
  start_memory_write();

  for (unsigned int i = 0; i < 240*320; i++)
    send_multiple_data((uint8_t []) {0xFF, 0xFF}, 2);

  /*set_address_window(50, 50, 210, 194);
  start_memory_write();
  for (unsigned int i = 0; i < 144 * 160; i++)
    send_multiple_data((uint8_t[]){0xFF, 0x00}, 2);*/
  uart2_println("Starting emulator");
  emulator_init();
  emulator_run();

  return 0;
}