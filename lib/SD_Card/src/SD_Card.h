#ifndef my_SD_Card
#define my_SD_Card

#include "stm32f411xe.h"
#include "ComputerUART.h"
#include "SD_Card_SPI.h"
#include "SD_Card_Responses.h"
#include "SD_Card_Info.h"
#include "SD_Card_Write_Commands.h"
#include "SD_Card_Read_CSD.h"
#include "SD_Card_Read_Block.h"

static int init_SD_Card(SD_CardInfo *SD_CardInfo) {
  // Set up the SPI interface
  SD_SPI_Init();
  SD_Set_SPI_Speed(SPI_DIVIDER_128); // Set the SPI speed to 125kHz
  SD_Enable_SPI();


  SD_WakeUp(); // Wake up the SD card


  SD_WriteCommand(0,0,0x95 >> 1);
  print_R1(get_R1_Response());

  R7_Response CMD8_Res;
  do {
    SD_WriteCommand(8,0x1 << 8 | 0xAA, 0x87 >> 1);
    CMD8_Res = get_R7_Response();

    if (CMD8_Res.r1 & R1_illegal_command || CMD8_Res.vca != 0x1)
      return SD_ERR_ILLEGAL_COMMAND; // Come back to this becasue it can just be an old car but add support later
  } while(CMD8_Res.check != 0xAA);
  print_R7(CMD8_Res);

  SD_WriteCommand(58,0,0xFD>>1);
  print_R3(get_R3_Response());

  SD_WriteCommand(59,1,0x4B); // Turn on CRC
  print_R1(get_R1_Response());

  R1_Response ACMD41_R1_Response = R1_in_idle_state;
  do {
    if (SD_WriteApplicationCommand(41,1 << 30,0x77 >> 1) == SD_ApplicationCommand_Success) // HCS = 1
      ACMD41_R1_Response = get_R1_Response();    
  } while((ACMD41_R1_Response & R1_in_idle_state));


  SD_WriteCommand(59,0,0x48); // Turn off CRC
  print_R1(get_R1_Response());

  SD_WriteCommand(58,0,0);
  R3_Response CMD58_Res = get_R3_Response();
  if (CMD58_Res.r1 > 0)
    return SD_ERR_CMD58;

  print_R3(CMD58_Res);

  if (CMD58_Res.OCR & R3_CCS) 
    return SD_GetCardInfo(SDHCorSDXC, SD_CardInfo);
  else 
    return SD_GetCardInfo(SDSC, SD_CardInfo);
}


#endif