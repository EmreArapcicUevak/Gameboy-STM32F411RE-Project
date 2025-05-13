#include "Battery.h"
//#include "ComputerUART.h"

// PA0 is our ADC input ADC1_0
void init_battery_ADC(uint16_t *ADCAddress, uint16_t *backupAddress) {
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA2EN;

  ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT | ADC_CR2_DMA | ADC_CR2_DDS; // Turn on the ADC whilst we set everything else up to allow it to stabilize 
  ADC1->SMPR2 |= (0b111 << ADC_SMPR2_SMP0_Pos);

  // ADC1_0 is the default and 1 scan is also the default so no extra setting up needed
  GPIOA->MODER |= (0b11 << GPIO_MODER_MODE0_Pos); // Set PA0 as analog
  

  DMA2_Stream0->CR &= ~(DMA_SxCR_CHSEL_Msk); // Select channel 0 from stream 0 (ADC1)
  DMA2_Stream0->CR |= (0b01 << DMA_SxCR_PL_Pos) | (0b01 << DMA_SxCR_PSIZE_Pos) | (0b01 << DMA_SxCR_MSIZE_Pos) | DMA_SxCR_DBM | DMA_SxCR_TCIE; // Set priority to medium, and set the PSize to half a word (16 bits)
  DMA2_Stream0->M0AR = ADCAddress; // Set the memory address to write to
  DMA2_Stream0->M1AR = backupAddress; // This is for the double buffer mode
  DMA2_Stream0->PAR = (uint32_t)&(ADC1->DR); // Set the ADC1 peripheral's data register as the Peripheral address
  DMA2_Stream0->NDTR = 1; // Set the number of data items for transfer to 1

  NVIC_EnableIRQ(DMA2_Stream0_IRQn); // Enable the interrupts for the DMA2 Stream 0

  DMA2_Stream0->CR |= DMA_SxCR_EN; // Enable the stream
  ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t read_battery_ADC(void) {
  if (DMA2_Stream0->CR & DMA_SxCR_CT) // CT is 1 so read from M0AR
    return *(uint16_t *)(DMA2_Stream0->M0AR);
  else 
    return *(uint16_t *)(DMA2_Stream0->M1AR);
}

void DMA2_Stream0_IRQHandler(void) {
  if (DMA2->LISR & DMA_LISR_TCIF0_Msk){
    DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
    uart2_println("Battery ADC value is:");
    uart2_send_bin_num(read_battery_ADC, 16);
    uart2_println("");
  }

  // More code here
}