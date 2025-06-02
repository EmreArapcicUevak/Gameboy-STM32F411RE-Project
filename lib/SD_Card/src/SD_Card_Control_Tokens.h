#ifndef SD_Card_Control_Tokens_h
#define SD_Card_Control_Tokens_h

#include "stm32f411xe.h"
#include "SD_Card_SPI.h"
#include "SD_Card_Responses.h"
#include "SD_Card_Info.h"
#include "SD_Card_Write_Commands.h"
#include "SD_Card_Read_CSD.h"
#include "ComputerUART.h"
#include "SD_Result.h"

#define SD_TIMEOUT 1000000 // Number of attempts to read a byte before timing out


#define SD_READ_BLOCK_SUCCESS 0
#define SD_READ_BLOCK_ERROR_Timeout 1
#define SD_READ_BLOCK_ERROR_TokenError 2
#define SD_READ_BLOCK_ERROR_OutOfRange 3

#define START_TRANSMISSION_SIGNLE_BLOCK_TOKEN 0xFE
#define START_TRANSMISSION_MULTI_BLOCK_TOKEN 0xFC // Start block token for reading data
#define STOP_TRANSMISSION_TOKEN 0xFD // Stop transmission token for multi-block read

static int SD_Get_Token(uint8_t *buffer) {
  uint8_t token;
  uint32_t timeout = SD_TIMEOUT;

  do {
    token = send_byte(0xFF);
    uart2_println("SD_Get_Token: Received token: ");
    uart2_send_bin_num(token, 8); // Print the received token in binary format
    uart2_println("");

    if (~token & 0xF0) {
      if (token & 0x09)
        return SD_ERR_OUT_OF_RANGE;
      else if (token & 0x01)
        return SD_ERR_UNKNOWN;
    } else if (timeout-- == 0)
      return SD_ERR_TIMEOUT;

  } while (token != START_TRANSMISSION_SIGNLE_BLOCK_TOKEN &&
           token != START_TRANSMISSION_MULTI_BLOCK_TOKEN &&
           token != STOP_TRANSMISSION_TOKEN);

  *buffer = token;
  return SD_OK; // Token received successfully
}

#endif