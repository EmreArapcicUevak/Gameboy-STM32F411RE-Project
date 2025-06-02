#ifndef SD_CARD_RESPONSES_H
#define SD_CARD_RESPONSES_H

#include "stm32f411xe.h"
#include "ComputerUART.h"
#include "SD_Card_SPI.h"

#define R1_in_idle_state 0x01
#define R1_erase_reset 0x02
#define R1_illegal_command 0x04
#define R1_communication_CRC_error 0x08
#define R1_erase_sequence_error 0x10
#define R1_address_error 0x20
#define R1_parameter_error 0x40

#define R3_CCS 1 << 30 // Card Capacity Status
#define R3_Power_Up_Status 1 << 31 

typedef uint8_t R1_Response;

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
#endif