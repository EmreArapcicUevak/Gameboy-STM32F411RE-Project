#ifndef SD_CARD_INFO_H
#define SD_CARD_INFO_H

#include "stm32f411xe.h"

typedef enum {
  Ver1X_SDSC = 0,
  SDSC,
  SDHCorSDXC,
  SDUC,
  Invalid
} 

SD_CardType;typedef struct {
  SD_CardType type;
  uint64_t capacity; // Capacity in KBytes
  float transfer_speed; // In Mbits/s
} SD_CardInfo;

static void copy_SD_CardInfo(SD_CardInfo *dest, const SD_CardInfo *src) {
  dest->type = src->type;
  dest->capacity = src->capacity;
  dest->transfer_speed = src->transfer_speed;
}

#endif