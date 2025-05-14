#ifndef ComputerUART
#define ComputerUART

#define MAX_BINARY_BITS 32

#include "stm32f411xe.h"

void uart2_init(uint32_t baud_rate) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2))); // Clear mode
  GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));     // Set AF

  GPIOA->AFR[0] |= (7 << (4 * 2)) | (7 << (4 * 3));    // AF7 for PA2, PA3

  // Assume APB1 = 16 MHz (default)
  uint32_t usartdiv = 16000000 / baud_rate;
  USART2->BRR = usartdiv;

  USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;  // Enable Transmit/Receive
  USART2->CR1 |= USART_CR1_UE;                 // Enable USART
}

void uart2_send_char(char c) {
  while (!(USART2->SR & USART_SR_TXE));
  USART2->DR = c;
}

void uart2_send_string(const char *s) {
  while (*s) {
      uart2_send_char(*s++);
  }
}

void uint32_to_binary_str(uint32_t value, char *buffer, uint8_t bits) {
  for (int i = bits - 1; i >= 0; i--) {
      *buffer++ = (value & (1 << i)) ? '1' : '0';
  }
  *buffer = '\0'; // Null terminator
}

void uart2_send_bin_num(uint32_t number, uint8_t bits){
  if (bits > MAX_BINARY_BITS) bits = MAX_BINARY_BITS; // safety cap
  char buffer[bits + 1];
  uint32_to_binary_str(number, buffer, bits);
  uart2_send_string(buffer);
}

void uart2_println(const char *s) {
  uart2_send_string(s);
  uart2_send_string("\n\r");
}

#endif