#ifndef SD_CARD_SPI_H
#define SD_CARD_SPI_H

#include "stm32f411xe.h"
#include "SD_Result.h"

static inline void SD_Enable_SPI(void) {
  SPI1->CR1 |= SPI_CR1_SPE; // Enable the SPI
}

static inline void SD_Disable_SPI(void) {
  SPI1->CR1 &= ~SPI_CR1_SPE; // Disable the SPI
}

#define SPI_DIVIDER_2 0b000
#define SPI_DIVIDER_4 0b001
#define SPI_DIVIDER_8 0b010
#define SPI_DIVIDER_16 0b011
#define SPI_DIVIDER_32 0b100
#define SPI_DIVIDER_64 0b101
#define SPI_DIVIDER_128 0b110
#define SPI_DIVIDER_256 0b111

static inline int SD_Set_SPI_Speed(uint8_t speed_divider) {
  if (speed_divider > 0b111) 
    return SD_ERR_SPI_FAIL;
  
  while (SPI1->SR & SPI_SR_BSY); // Wait until the SPI is not busy
  SD_Disable_SPI(); // Disable the SPI to change the speed

  SPI1->CR1 &= ~(0b111 << SPI_CR1_BR_Pos);
  SPI1->CR1 |= (speed_divider << SPI_CR1_BR_Pos);

  SD_Enable_SPI();
  return SD_OK;
}

static inline uint8_t send_byte(uint8_t byte) {
  while (!(SPI1->SR & SPI_SR_TXE)); // wait until the transmission buffer is empty
  SPI1->DR = byte; // Send the byte

  while (!(SPI1->SR & SPI_SR_RXNE)); // Wait until there is data in the buffer
  return SPI1->DR;
}

static inline void SD_SPI_Init(void) {
  // Enable the clocks
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Set PA5,6,7 to alternative function
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODE5_Pos) | (0b10 << GPIO_MODER_MODE6_Pos) | (0b10 << GPIO_MODER_MODE7_Pos);
  
  // Set PA5,6,7 to Alt func 5
  GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL6_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos);
  
  // Set PC4 as output
  GPIOC->MODER |= (0b01 << GPIO_MODER_MODE4_Pos);
  
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_MSTR | SPI_CR1_SSI; // Manager slave or master mode behaviour via software
}

static inline void SD_CS_LOW(void){
  GPIOC->ODR &= ~(1 << GPIO_ODR_OD4);
}

static inline void SD_CS_HIGH(void){
  GPIOC->ODR |= (1 << GPIO_ODR_OD4);
}

static inline void SD_WakeUp(void) {
  SD_CS_HIGH();
  for (unsigned int i = 0; i < 10; i++)
    send_byte(0xFF); // Give card time to wake up
  SD_CS_LOW();
}

#endif