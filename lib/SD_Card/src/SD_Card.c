#include "SD_Card.h"

// PA5 [SPI1_SCK], PA6 [SPI1_MISO], PA7 [SPI1_MOSI] <-- AF05
// PC4[Chip Select]

void init_SD_Card(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Set PA5,6,7 to alternative function
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODE5_Pos) | (0b10 << GPIO_MODER_MODE6_Pos) | (0b10 << GPIO_MODER_MODE7_Pos);

  // Set PA5,6,7 to Alt func 5
  GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL6_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos);

  // Set PC4 as output
  GPIOC->MODER |= (0b01 << GPIO_MODER_MODE4);

  SPI1->CR1 |= SPI_CR1_SPE;
  deselect_SD();
}

void select_SD(void){
  GPIOC->ODR &= ~(1 << GPIO_ODR_OD4);
}

void deselect_SD(void){
  GPIOC->ODR |= (1 << GPIO_ODR_OD4);
}