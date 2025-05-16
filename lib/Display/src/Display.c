#include "Display.h"

// Display PC2[SPI2_MISO], PC3[SPI2_MOSI], PC7[SPI2_SCK] <---- AF05
// Display reset PC0, Display command or data PC1
// PWM for the display's LED controll PB0 [TIM3_CH3]

void init_display(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN; // Enable the GPIOC clock
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN | RCC_APB1ENR_TIM3EN; // Enable the clock for the SPI2

  GPIOC->MODER |= (0b01 << GPIO_MODER_MODE0_Pos) | // Output
  (0b01 << GPIO_MODER_MODE1_Pos) | // Output
  (0b10 << GPIO_MODER_MODE2_Pos) | // Alt func
  (0b10 << GPIO_MODER_MODE3_Pos) | // Alt func
  (0b10 << GPIO_MODER_MODE7_Pos); // Alt func
  GPIOC->AFR[0] |= (5 << GPIO_AFRL_AFSEL2_Pos) |  // AF05
  (5 << GPIO_AFRL_AFSEL3_Pos) | // AF05
  (5 << GPIO_AFRL_AFSEL7_Pos); // AF05

  // Set PB0 to alternative function AF2 which is the TIM3_CH3
  GPIOB->MODER |= (0b10 << GPIO_MODER_MODE0_Pos);
  GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL0_Pos); 
  GPIOB->OSPEEDR |= (0b10 << GPIO_OSPEEDR_OSPEED0_Pos); // PB0 Fast Speed output

  GPIOC->ODR &= ~GPIO_ODR_OD0; // Keep display reset pin on low
  wait(10);
  GPIOC->ODR |= GPIO_ODR_OD0; // Keep display reset pin on low
  wait(10);
  
  SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_MSTR | SPI_CR1_SSI; // Manager slave or master mode behaviour via software
  // Leave clock polarity 0 and clock phase 0 

  //NVIC->ISER[1] |= (0b1 << 4); // enable interrupt for SPI2
  //SPI2->CR2 |= SPI_CR2_RXNEIE;


  TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS;
  TIM3->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE;
  TIM3->CCER |= TIM_CCER_CC3E; // enable CC3
  TIM3->PSC = 15; // Set prescaler (assuming 16 MHz clock -> 1 MHz Clock)
  TIM3->ARR = 999;   // Auto-reload value (period = 1000 counts --> 1s/1000 = 1ms --> 1kHz PWM)
  TIM3->CCR3 = 1000;  // 100% duty cycle
  TIM3->EGR |= TIM_EGR_UG;
  TIM3->CR1 |= TIM_CR1_CEN; // Turn on the timer

  SPI2->CR1 |= SPI_CR1_SPE; // Turn on the SPI

  write_command(0x01); // Software reset
  wait(5);

  write_command(0x28); // Display OFF

  write_command(0xCF); send_data(0x00); send_data(0x83); send_data(0x30);
  write_command(0xED); send_data(0x64); send_data(0x03); send_data(0x12); send_data(0x81);
  write_command(0xE8); send_data(0x85); send_data(0x01); send_data(0x79);

  write_command(0xCB); send_data(0x39); send_data(0x2C); send_data(0x00); send_data(0x34); send_data(0x02);
  write_command(0xF7); send_data(0x20);
  write_command(0xEA); send_data(0x00); send_data(0x00);

  write_command(0xC0); send_data(0x26);         // Power control 1
  write_command(0xC1); send_data(0x11);         // Power control 2
  write_command(0xC5); send_data(0x35); send_data(0x3E); // VCOM control
  write_command(0xC7); send_data(0xBE);

  write_command(0x36); send_data(0b00101000);         // Memory access control
  write_command(0x3A); send_data(0x55);         // Pixel format = 16 bit

  write_command(0xB1); send_data(0x00); send_data(0x1B); // Frame rate
  write_command(0xF2); send_data(0x08);         // 3Gamma Function Disable
  write_command(0x26); send_data(0x01);         // Gamma curve selected
  write_command(0xE0); // Positive gamma correction
  send_data(0x1F); send_data(0x1A); send_data(0x18); send_data(0x0A);
  send_data(0x0F); send_data(0x06); send_data(0x45); send_data(0x87);
  send_data(0x32); send_data(0x0A); send_data(0x07); send_data(0x02);
  send_data(0x07); send_data(0x05); send_data(0x00);

  write_command(0xE1); // Negative gamma correction
  send_data(0x00); send_data(0x25); send_data(0x27); send_data(0x05);
  send_data(0x10); send_data(0x09); send_data(0x3A); send_data(0x78);
  send_data(0x4D); send_data(0x05); send_data(0x18); send_data(0x0D);
  send_data(0x38); send_data(0x3A); send_data(0x1F);

  write_command(0x11); // Sleep OUT
  wait(100);

  write_command(0x29); // Display ON
}


void write_command(uint8_t command) {
  GPIOC->ODR &= ~(GPIO_ODR_OD1);
  while (!(SPI2->SR & SPI_SR_TXE)); // Wait until transmit buffer empty
  SPI2->DR = command;
  while (!(SPI2->SR & SPI_SR_TXE)); // Wait until transmit register is empty
  while (!(SPI2->SR & SPI_SR_RXNE)); // Wait until something has been received
  volatile uint8_t dummy = SPI2->DR; // Read and discard received byte
}

void send_data(uint8_t data) {
  GPIOC->ODR |= (GPIO_ODR_OD1);
  while (!(SPI2->SR & SPI_SR_TXE));
  SPI2->DR = data;
  while (!(SPI2->SR & SPI_SR_TXE)); // Wait until transmit register is empty
  while (!(SPI2->SR & SPI_SR_RXNE)); // Wait until something has been received
}

void send_multiple_data(uint8_t *data_arr, const unsigned int size) {
  for (uint8_t *data = data_arr; data < data_arr + size; data++)
    send_data(*data);
}


void SPI2_IRQHandler(void) {
  if (SPI2->SR & SPI_SR_TXE) {
    uart2_println("can send more data!");
  }

  if ((SPI2->SR) & SPI_SR_RXNE) {
    uart2_println("Recieved data:");
    uart2_send_bin_num(SPI2->DR, 8);
    uart2_println("");
  }
}