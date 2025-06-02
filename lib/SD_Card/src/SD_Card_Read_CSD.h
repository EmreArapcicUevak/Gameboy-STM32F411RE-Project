#ifndef SD_CARD_READ_CSD_H
#define SD_CARD_READ_CSD_H

#include "stm32f411xe.h"
#include "SD_Card_SPI.h"
#include "SD_Card_Responses.h"
#include "SD_Card_Info.h"
#include "SD_Card_Write_Commands.h"
#include "SD_Card_Control_Tokens.h"

static uint32_t SD_READ_CSD_C_SIZE(const uint8_t* CSD,const uint8_t CSD_Version) {
  if (CSD_Version == 2) {
    uint32_t C_SIZE = ((CSD[7] & 0x3F) << 16) | (CSD[8] << 8) | CSD[9];
    return (C_SIZE + 1) * 512; // C_SIZE is in blocks of 512 bytes
  }

  return 0; // For other CSD version I will handle that later.
}

static float SD_READ_CSD_C_SPEED(uint8_t* CSD, uint8_t CSD_Version) {
  uint8_t TRAN_SPEED = CSD[3];
  float speed;
  switch (TRAN_SPEED & 0x07) {
    case 0: // 100kbits
      speed = 0.1;
      break;
    case 1: // 1Mbits
      speed = 1;
      break;
    case 2: // 10Mbits
      speed = 10;
      break;
    case 3: // 100Mbits
      speed = 100;
      break;
    default: // for reserved values, return 0
      return 0; // Reserved or unknown speed
  }

  switch ((TRAN_SPEED & 0x78) >> 3) { // bits 3 - 6
    case 0x1:
      return speed;
    case 0x2:
      return speed * 1.2;
    case 0x3:
      return speed * 1.3;
    case 0x4:
      return speed * 1.5;
    case 0x5:
      return speed * 2;
    case 0x6:
      return speed * 2.5;
    case 0x7:
      return speed * 3;
    case 0x8:
      return speed * 3.5;
    case 0x9:
      return speed * 4;
    case 0xA:
      return speed * 4.5;
    case 0xB:
      return speed * 5;
    case 0xC:
      return speed * 5.5;
    case 0xD:
      return speed * 6;
    case 0xE:
      return speed * 7;
    case 0xF:
      return speed * 8;
    default:
      return 0; // Reserved or unknown speed
  }
}

static int SD_GetCardInfo(SD_CardType type, SD_CardInfo *buffer) {
  buffer->type = Invalid;
  SD_WriteCommand(9,0,0x00); // Send CMD9 to read the CSD register
  uint8_t CSD_R1_Response = get_R1_Response();
  if (CSD_R1_Response != 0) 
    return SD_ERR_ILLEGAL_COMMAND;

  uint8_t CSD_Token;
  int token_response = SD_Get_Token(&CSD_Token);
  if (token_response != SD_OK)
    return token_response;

  uint8_t CSD[16];
  for (unsigned int i = 0; i < 16; i++)
    CSD[i] = send_byte(0xFF);

  const uint8_t CSD_Version = ((CSD[0] >> 6) & 0x03) + 1; // CSD Version is in bits 6 and 7 of the first byte
  buffer->type = type;
  buffer->capacity = SD_READ_CSD_C_SIZE(CSD, CSD_Version);
  buffer->transfer_speed = SD_READ_CSD_C_SPEED(CSD, CSD_Version);

  return SD_OK;
}

#endif