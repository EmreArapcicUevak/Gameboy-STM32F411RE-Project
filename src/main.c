#include <stm32f411xe.h>
#include "ComputerUART.h"
#include "my_wait.h"
#include <Display.h>
#include <Speaker.h>
#include <SD_Card.h>
#include <Battery.h>
#include <Buttons.h>

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
    send_multiple_data((uint8_t []) {0xF8, 0x00}, 2);

  set_address_window(319/2 - 10,239/2 - 50,319/2 + 10, 239/2 + 50);
  start_memory_write();
  for (unsigned int i = 0; i < 320*20; i++)
    send_multiple_data((uint8_t []) {0xFF, 0xFF}, 2); 


  set_address_window(319/2 - 50,239/2 - 10,319/2 + 50,239/2 + 10);
  start_memory_write();

  for (unsigned int i = 0; i < 320*20; i++)
    send_multiple_data((uint8_t []) {0xFF, 0xFF}, 2); 
  
    
  uart2_println("Starting main loop");
  while (1) {
    //uart2_println("This is from the main loop");
    wait(500);
  }

  return 0;
}