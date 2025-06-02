#ifndef SD_CARD_READ_BLOCK_H
#define SD_CARD_READ_BLOCK_H

#include "SD_Card_SPI.h"
#include "SD_Card_Responses.h"
#include "SD_Card_Info.h"
#include "SD_Card_Write_Commands.h"
#include "SD_Card_Control_Tokens.h"

static int SD_Read_Block(SD_CardInfo *card, uint8_t *buffer, uint32_t address) {
    if (card->type == SDHCorSDXC) {
      SD_WriteCommand(17, address, 0x00);

      R1_Response writeCmdRes = get_R1_Response();
      if (writeCmdRes > 0)
        return SD_ERR_CMD17;
      print_R1(writeCmdRes);

      uint8_t token;
      int token_response = SD_Get_Token(&token);
      if (token_response != SD_OK)
        return token_response;

      for (unsigned int i = 0; i < 512; i++)
        buffer[i] = send_byte(0xFF);

      // Clear the CRC
      send_byte(0xFF);
      send_byte(0xFF);

      return SD_OK;
    }

    return SD_ERR_NO_RESPONSE; // Add support for other cards later
}


#endif