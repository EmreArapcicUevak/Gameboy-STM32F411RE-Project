#ifndef EMULATOR_H
#define EMULATOR_H

#include <ComputerUART.h>
#include <Display.h>

#include "../boot_rom/dmg_boot_rom.h"
#include "../game/tetris_rom.h"
#include "peanut_gb.h"
#include "stm32f411xe.h"

struct gb_s gb;
uint8_t framebuffer[LCD_HEIGHT][LCD_WIDTH];

enum button { UP, DOWN, LEFT, RIGHT, SELECT, START, B, A };

uint8_t gb_rom_read(struct gb_s *gb, const uint_fast32_t addr) {
  return game_rom[addr];
}

uint8_t gb_bootrom_read(struct gb_s *gb, const uint_fast16_t addr) {
  return dmg_boot_bin[addr];
}

void gb_error(struct gb_s *gb, const enum gb_error_e gb_err,
              const uint16_t addr) {
  uart2_println("Emulator error");
}

void lcd_draw_line(struct gb_s *gb, const uint8_t pixels[160],
                   const uint_fast8_t line) {
  for (uint8_t x = 0; x < LCD_WIDTH; x++) {
    framebuffer[line][x] = pixels[x] & 3;
  }
}

void display_buffer() {
  set_address_window(50, 50, 50 + LCD_WIDTH, 50 + LCD_HEIGHT);
  start_memory_write();
  for (uint8_t i = 0; i < LCD_HEIGHT; i++)
    for (uint8_t j = 0; j < LCD_WIDTH; j++) switch (framebuffer[i][j] & 0x3) {
        case 0x0:
          send_multiple_data((uint8_t[]){0x08, 0x41}, 2);
          break;
        case 0x1:
          send_multiple_data((uint8_t[]){0x39, 0xE7}, 2);
          break;
        case 0x2:
          send_multiple_data((uint8_t[]){0x84, 0x10}, 2);
          break;
        case 0x3:
          send_multiple_data((uint8_t[]){0xFF, 0xFF}, 2);
          break;
      }
}

void emulator_init() {
  enum gb_init_error_e err = gb_init(&gb, &gb_rom_read, NULL, NULL, &gb_error, NULL);

  if (err != GB_INIT_NO_ERROR) {
    uart2_println("Failed to init GB emulator!");
    if (err == GB_INIT_CARTRIDGE_UNSUPPORTED)
      uart2_println("GB_INIT_CARTRIDGE_UNSUPPORTED");
    else if (err == GB_INIT_INVALID_CHECKSUM)
      uart2_println("GB_INIT_INVALID_CHECKSUM");
    else
      uart2_println("GB_INIT_INVALID_MAX");
    
    return;
  }
  gb_init_lcd(&gb, &lcd_draw_line);
}

void emulator_run() {
  while (true) {
    uart2_println("loop");
    gb_run_frame(&gb);
    display_buffer();
  }
}

void set_input(const enum button btn, const bool is_down) {
  if (is_down) {
    switch (btn) {
      case UP:
        gb.direct.joypad &= ~JOYPAD_UP;
        break;
      case DOWN:
        gb.direct.joypad &= ~JOYPAD_DOWN;
        break;
      case LEFT:
        gb.direct.joypad &= ~JOYPAD_LEFT;
        break;
      case RIGHT:
        gb.direct.joypad &= ~JOYPAD_RIGHT;
        break;
      case SELECT:
        gb.direct.joypad &= ~JOYPAD_SELECT;
        break;
      case START:
        gb.direct.joypad &= ~JOYPAD_START;
        break;
      case B:
        gb.direct.joypad &= ~JOYPAD_B;
        break;
      case A:
        gb.direct.joypad &= ~JOYPAD_A;
        break;
    }
  } else {
    switch (btn) {
      case UP:
        gb.direct.joypad |= JOYPAD_UP;
        break;
      case DOWN:
        gb.direct.joypad |= JOYPAD_DOWN;
        break;
      case LEFT:
        gb.direct.joypad |= JOYPAD_LEFT;
        break;
      case RIGHT:
        gb.direct.joypad |= JOYPAD_RIGHT;
        break;
      case SELECT:
        gb.direct.joypad |= JOYPAD_SELECT;
        break;
      case START:
        gb.direct.joypad |= JOYPAD_START;
        break;
      case B:
        gb.direct.joypad |= JOYPAD_B;
        break;
      case A:
        gb.direct.joypad |= JOYPAD_A;
        break;
    }
  }
}

#endif  // EMULATOR_H
