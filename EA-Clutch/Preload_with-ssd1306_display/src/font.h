#ifndef FONT_H__
#define FONT_H__

#include <stdint.h>

// Each character is 5 pixels wide and 8 pixels tall
// Each entry represents 5 vertical columns (8 bits high) per character
extern const uint8_t ASCII[][5];

#endif
