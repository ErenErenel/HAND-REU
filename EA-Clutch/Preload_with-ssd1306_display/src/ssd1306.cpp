#include "ssd1306.h"
#include "font.h"

#define COMMAND 0x00
#define DATA    0x40

uint8_t ssd1306_buffer[SSD1306_BUFFERSIZE];
uint8_t ssd1306_i2c_buffer[SSD1306_BUFFERSIZE + 1];

void ssd1306_command(uint8_t c) {
  SSD1306_I2C.beginTransmission(SSD1306_ADDR);
  SSD1306_I2C.write(COMMAND);
  SSD1306_I2C.write(c);
  SSD1306_I2C.endTransmission();
}

void ssd1306_setup() {
  SSD1306_I2C.begin();
  delay(100);

  ssd1306_command(0xAE); // DISPLAYOFF
  ssd1306_command(0xD5); // SETDISPLAYCLOCKDIV
  ssd1306_command(0x80);
  ssd1306_command(0xA8); // SETMULTIPLEX
  ssd1306_command(SSD1306_HEIGHT - 1);
  ssd1306_command(0xD3); // SETDISPLAYOFFSET
  ssd1306_command(0x00);
  ssd1306_command(0x40 | 0x0); // SETSTARTLINE
  ssd1306_command(0x8D); // CHARGEPUMP
  ssd1306_command(0x14);
  ssd1306_command(0x20); // MEMORYMODE
  ssd1306_command(0x00);
  ssd1306_command(0xA1); // SEGREMAP
  ssd1306_command(0xC8); // COMSCANDEC
  ssd1306_command(0xDA); // SETCOMPINS
  ssd1306_command(0x02);
  ssd1306_command(0x81); // SETCONTRAST
  ssd1306_command(0x8F);
  ssd1306_command(0xD9); // SETPRECHARGE
  ssd1306_command(0xF1);
  ssd1306_command(0xDB); // SETVCOMDETECT
  ssd1306_command(0x40);
  ssd1306_command(0xA4); // DISPLAYALLON_RESUME
  ssd1306_command(0xA6); // NORMALDISPLAY
  ssd1306_command(0x2E); // DEACTIVATE_SCROLL
  ssd1306_command(0xAF); // DISPLAYON
}

void ssd1306_update() {
  ssd1306_command(0x21); // COLUMNADDR
  ssd1306_command(0); 
  ssd1306_command(SSD1306_WIDTH - 1);
  ssd1306_command(0x22); // PAGEADDR
  ssd1306_command(0);
  ssd1306_command((SSD1306_HEIGHT / 8) - 1);

  ssd1306_i2c_buffer[0] = DATA; // first byte is control byte
  memcpy(&ssd1306_i2c_buffer[1], ssd1306_buffer, SSD1306_BUFFERSIZE);

  // Send buffer in chunks (Teensy I2C has a 256-byte limit)
  for (uint16_t i = 0; i < SSD1306_BUFFERSIZE; i += 16) {
    SSD1306_I2C.beginTransmission(SSD1306_ADDR);
    SSD1306_I2C.write(DATA);
    for (uint8_t j = 0; j < 16 && (i + j) < SSD1306_BUFFERSIZE; j++) {
      SSD1306_I2C.write(ssd1306_buffer[i + j]);
    }
    SSD1306_I2C.endTransmission();
  }
}

void ssd1306_clear() {
  memset(ssd1306_buffer, 0, SSD1306_BUFFERSIZE);
}

void ssd1306_drawPixel(int x, int y, int color) {
  if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;
  int byteIndex = x + (y / 8) * SSD1306_WIDTH;
  if (color)
    ssd1306_buffer[byteIndex] |= (1 << (y % 8));
  else
    ssd1306_buffer[byteIndex] &= ~(1 << (y % 8));
}

void drawChar(int x, int y, char c) {
  if (c < 0x20 || c > 0x7E) c = '?'; // fallback to '?' for unsupported
  for (int col = 0; col < 5; col++) {
    uint8_t line = ASCII[c - 0x20][col];
    for (int row = 0; row < 8; row++) {
      int pixel = (line >> row) & 0x01;
      ssd1306_drawPixel(x + col, y + row, pixel);
    }
  }
}

void drawMessage(int x, int y, const char* msg) {
  while (*msg) {
    drawChar(x, y, *msg++);
    x += 6; // 5px width + 1px space
    if (x + 5 >= SSD1306_WIDTH) break;
  }
}
