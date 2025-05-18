#ifndef my_SD_Card
#define my_SD_Card

#include "stm32f411xe.h"
#include "ComputerUART.h"

#define R1_in_idle_state 0x01
#define R1_erase_reset 0x02
#define R1_illegal_command 0x04
#define R1_communication_CRC_error 0x08
#define R1_erase_sequence_error 0x10
#define R1_address_error 0x20
#define R1_parameter_error 0x40

#define SD_Init_Error_Unusable_Card -1
#define SD_Init_Error_Timeout -2
#define SD_Init_Success 0

#define SD_ApplicationCommand_Success 0
#define SD_ApplicationCommand_Fail -1

typedef struct {
    uint8_t r1;         // R1 response (status byte)
    uint8_t commandVersion;
    uint8_t vca;        // Voltage Card Accept
    uint8_t check;      // Echoed check pattern
} R7_Response;

typedef struct {
  uint8_t r1;
  uint32_t OCR;
} R3_Response;

static void SD_CS_LOW(void){
  GPIOC->ODR &= ~(1 << GPIO_ODR_OD4);
}

static void SD_CS_HIGH(void){
  GPIOC->ODR |= (1 << GPIO_ODR_OD4);
}

static inline uint8_t send_byte(uint8_t byte) {
  while (!(SPI1->SR & SPI_SR_TXE)); // wait until the transmission buffer is empty
  SPI1->DR = byte; // Send the byte

  while (!(SPI1->SR & SPI_SR_RXNE)); // Wait until there is data in the buffer
  return SPI1->DR;
}

static void SD_WriteCommand(uint8_t commandIndex, uint32_t arguments, uint8_t CRC7) {
  if (commandIndex > 0x3F) {
    uart2_println("Invalid command index");
    return;
  } else if (CRC7 > 0x7F) {
    uart2_println("Invalid CRC value");
    return;
  }

  uint8_t response;
  
  // Start the command transmission
  response = send_byte(0x40 | commandIndex);

  // Now send the arguments
  for (unsigned int i = 0; i < 4; i++)
    response = send_byte((arguments >> 8 * (3 - i)) & 0xFF);

  // Now send the CRC7
  response = send_byte(CRC7 << 1 | 0b1);
}


static uint8_t get_R1_Response(void) {
  uint8_t response;
  do {
    response = send_byte(0xFF);
  } while (response & 0x80);
  
  return response;
}

static R3_Response get_R3_Response(void) {
  R3_Response returnValue;

  returnValue.r1 = get_R1_Response();
  if (returnValue.r1 & R1_illegal_command)
    return returnValue;

  for (unsigned int i = 0; i < 4; i++)
    returnValue.OCR = (returnValue.OCR << 8) | send_byte(0xFF);

  return returnValue;
}

static R7_Response get_R7_Response(void) {
  R7_Response returnValue;

  returnValue.r1 = get_R1_Response();
  if (returnValue.r1 & R1_illegal_command)
    return returnValue;

  returnValue.commandVersion = send_byte(0xFF) >> 4;

  uint8_t response;
  for (unsigned int i = 0; i < 2; i++)
    response = send_byte(0xFF);

  returnValue.vca = response & 0x0F;
  returnValue.check = send_byte(0xFF);
  
  return returnValue;
}

static void print_R1(uint8_t response) {
  uart2_println("R1 response:");
  uart2_send_bin_num(response, 8);
  uart2_println("");
}

static int SD_WriteApplicationCommand(uint8_t commandIndex, uint32_t arguments, uint8_t CRC7) {
  SD_WriteCommand(55,0,0x65 >> 1); // Let the card know that the next command will be an app command
  uint8_t APP_CMD_R1_Response = get_R1_Response();

  if (APP_CMD_R1_Response > 1) {
    uart2_println("Error happened at SD_WriteApp:");
    print_R1(APP_CMD_R1_Response);
    return SD_ApplicationCommand_Fail;
  }
  
  SD_WriteCommand(commandIndex, arguments, CRC7);

  return SD_ApplicationCommand_Success;
}


static void print_R3(R3_Response response) {
  uart2_println("R3 response:");
  uart2_send_bin_num(response.r1, 8);
  uart2_println("");
  uart2_send_bin_num(response.OCR, 32);
  uart2_println("");
}

static void print_R7(R7_Response response) {
  uart2_println("R7 response:");
  uart2_send_bin_num(response.r1, 8);
  uart2_println("");
  uart2_send_bin_num(response.commandVersion, 4);
  uart2_println("");
  uart2_send_bin_num(response.vca, 4);
  uart2_println("");
  uart2_send_bin_num(response.check, 8);
  uart2_println("");
}


static int init_SD_Card(void) {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Set PA5,6,7 to alternative function
  GPIOA->MODER |= (0b10 << GPIO_MODER_MODE5_Pos) | (0b10 << GPIO_MODER_MODE6_Pos) | (0b10 << GPIO_MODER_MODE7_Pos);

  // Set PA5,6,7 to Alt func 5
  GPIOA->AFR[0] |= (5 << GPIO_AFRL_AFSEL5_Pos) | (5 << GPIO_AFRL_AFSEL6_Pos) | (5 << GPIO_AFRL_AFSEL7_Pos);

  // Set PC4 as output
  GPIOC->MODER |= (0b01 << GPIO_MODER_MODE4_Pos);

  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_MSTR | SPI_CR1_SSI; // Manager slave or master mode behaviour via software
  SPI1->CR1 |= (0b110 << SPI_CR1_BR_Pos); // Dividie the clock by 128 [125kHz]

  SPI1->CR1 |= SPI_CR1_SPE; // Enable the SPI

  SD_CS_HIGH();
  for (unsigned int i = 0; i < 10; i++)
    send_byte(0xFF); // Give card time to wake up
  SD_CS_LOW();

  SD_WriteCommand(0,0,0x95 >> 1);
  print_R1(get_R1_Response());

  R7_Response CMD8_Res;
  do {
    SD_WriteCommand(8,0x1 << 8 | 0xAA, 0x87 >> 1);
    CMD8_Res = get_R7_Response();

    if (CMD8_Res.r1 & R1_illegal_command || CMD8_Res.vca != 0x1)
      return SD_Init_Error_Unusable_Card;
  } while(CMD8_Res.check != 0xAA);
  print_R7(CMD8_Res);

  SD_WriteCommand(58,0,0xFD>>1);
  print_R3(get_R3_Response());

  uint8_t ACMD41_R1_Response = R1_in_idle_state;

  SD_WriteCommand(59,1,0x4B); // Turn on CRC
  print_R1(get_R1_Response());

  do {
    if (SD_WriteApplicationCommand(41,1 << 30,0x77 >> 1) == SD_ApplicationCommand_Success) // HCS = 1
      ACMD41_R1_Response = get_R1_Response();    
  } while((ACMD41_R1_Response & R1_in_idle_state));


  SD_WriteCommand(59,0,0x48); // Turn off CRC
  print_R1(get_R1_Response());

  SD_WriteCommand(58,0,0);
  print_R3(get_R3_Response());

  return SD_Init_Success;
}


#endif