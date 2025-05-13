#ifndef my_Color
#define my_Color

#include "stm32f411xe.h"

typedef struct {
  uint8_t red, green, blue;  
} Color;

Color lerp_color(const Color color1, const Color color2, float t) { // lerp from Color 1 to Color 2
  Color result;
  result.red = color1.red + (color2.red - color1.red) * t;
  result.green = color1.green + (color2.green - color1.green) * t;
  result.blue = color1.blue + (color2.blue - color1.blue) * t;

  return result;
}


#endif