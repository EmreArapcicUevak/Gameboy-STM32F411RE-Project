#include <stm32f411xe.h>
#include "ComputerUART.h"
#include "my_wait.h"
#include <Display.h>
#include <Speaker.h>

// Built-in LED PA5
int main(){
  uart2_init(115200);
  init_wait_timer();
  init_display();
  init_speaker();

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= (0b01 << GPIO_MODER_MODER5_Pos); // Set PA5 as output

  uart2_println("Programm started");
  write_command(0x29);
  while (1) {
    GPIOA->ODR ^= GPIO_ODR_OD5;        // Toggle LED on PA5
    wait(500);
  }
  return 0;
}