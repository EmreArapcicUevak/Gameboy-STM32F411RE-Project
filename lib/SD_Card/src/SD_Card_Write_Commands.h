#ifndef SD_CARD_WRITE_COMMANDS_H
#define SD_CARD_WRITE_COMMANDS_H

#include "stm32f411xe.h"
#include "SD_Card_SPI.h"
#include "SD_Card_Responses.h"

static void SD_WriteCommand(uint8_t commandIndex, uint32_t arguments, uint8_t CRC7) {
  if (commandIndex > 0x3F) {
    uart2_println("Invalid command index");
    return;
  } else if (CRC7 > 0x7F) {
    uart2_println("Invalid CRC value");
    return;
  }

  // Start the command transmission
  send_byte(0x40 | commandIndex);

  // Now send the arguments
  for (unsigned int i = 0; i < 4; i++)
    send_byte((arguments >> 8 * (3 - i)) & 0xFF);

  // Now send the CRC7
  send_byte(CRC7 << 1 | 0b1);
}

#define SD_ApplicationCommand_Success 0
#define SD_ApplicationCommand_Fail -1
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

#endif