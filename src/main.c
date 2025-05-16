#include <stm32f411xe.h>
#include "ComputerUART.h"
#include "my_wait.h"
#include <Display.h>
#include <Speaker.h>
#include <SD_Card.h>
#include <Battery.h>

// Built-in LED PA5
int main(){
  uint16_t batteryValues[2];

  uart2_init(115200);
  init_wait_timer();
  init_display();
  init_speaker();
  init_SD_Card();
  init_battery_ADC(batteryValues, batteryValues + 1);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= (0b01 << GPIO_MODER_MODER5_Pos); // Set PA5 as output

  uart2_println("Programm started");
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
  uart2_println("--------");

  while (1) {
    GPIOA->ODR ^= GPIO_ODR_OD5;        // Toggle LED on PA5
    uart2_println("This is from the main loop");
    wait(500);
  }

  return 0;
}