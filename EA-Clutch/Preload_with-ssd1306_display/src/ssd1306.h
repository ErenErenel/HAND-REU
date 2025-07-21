#ifndef SSD1306_H__
#define SSD1306_H__

#include <Arduino.h>
#include <Wire.h>

// Use Wire1 for Teensy 4.1 alternate I2C bus (e.g., SDA1/SCL1)
#define SSD1306_I2C       Wire1
#define SSD1306_ADDR      0x3C

#define SSD1306_WIDTH     128
#define SSD1306_HEIGHT    32
#define SSD1306_BUFFERSIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

// Display buffer and I2C transfer array
extern uint8_t ssd1306_buffer[SSD1306_BUFFERSIZE];
extern uint8_t ssd1306_i2c_buffer[SSD1306_BUFFERSIZE + 1];

// Initialize the display (sends setup commands)
void ssd1306_setup(void);

// Transfer the display buffer to the screen
void ssd1306_update(void);

// Clear the display buffer (set all pixels off)
void ssd1306_clear(void);

// Draw a single pixel (0 = off, 1 = on)
void ssd1306_drawPixel(int x, int y, int color);

// Draw a single 5x8 character at x, y
void drawChar(int x, int y, char c);

// Draw a null-terminated string at x, y
void drawMessage(int x, int y, const char* msg);

#endif
