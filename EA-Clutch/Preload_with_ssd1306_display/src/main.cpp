#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "ssd1306.h"

#define FILTER_SIZE 20
int32_t readings[FILTER_SIZE] = {0};
uint8_t idx = 0;
int64_t sum = 0;

Adafruit_NAU7802 nau;

char message[32];
int32_t offset = 0;
float scale_factor = 100.0 / 20700.0;  // Based on 100g = 21000 units

void setup() {
  pinMode(13, OUTPUT);
  Wire1.begin();                 
  Wire1.setClock(400000);       
  Serial.begin(115200);

  // OLED setup
  ssd1306_setup();
  ssd1306_clear();
  ssd1306_update();

  // NAU7802 setup
  if (!nau.begin(&Wire)) {
    drawMessage(0, 0, "NAU7802 not found!");
    ssd1306_update();
    while (1);
  }

  nau.setLDO(NAU7802_3V0);
  nau.setGain(NAU7802_GAIN_128);
  nau.setRate(NAU7802_RATE_320SPS);

  // Discard initial noisy samples
  for (int i = 0; i < 10; i++) {
    while (!nau.available());
    nau.read();
  }

  // Compute offset
  int64_t offset_sum = 0;
  const int offset_samples = 3000;
  for (int i = 0; i < offset_samples; i++) {
    while (!nau.available());
    offset_sum += nau.read();
  }
  offset = offset_sum / offset_samples;

  Serial.print("Offset: ");
  Serial.println(offset);

  drawMessage(0, 0, "Offset Done");
  ssd1306_update();
}


void loop() {
  if (nau.available()) {
    int32_t raw = nau.read();
    int32_t corrected = raw - offset;

    // Moving average filter
    sum -= readings[idx];
    readings[idx] = corrected;
    sum += readings[idx];
    idx = (idx + 1) % FILTER_SIZE;
    int32_t filtered = sum / FILTER_SIZE;

    float grams = filtered * scale_factor;

    static unsigned long start_time = millis();  // run only once
    float t = (millis() - start_time) / 1000.0;  // time in seconds

    Serial.print(t, 3);
    Serial.print(", ");
    Serial.println(grams, 2);
    Serial.println(corrected);

    // --- OLED Display ---
    ssd1306_clear();

    sprintf(message, "Raw: %ld", raw);
    drawMessage(0, 0, message);

    sprintf(message, "Offset: %ld", corrected);
    drawMessage(0, 10, message);

    sprintf(message, "Preload: %.2f g", grams);
    drawMessage(0, 20, message);

    ssd1306_update();

    digitalWrite(13, !digitalRead(13));  // Blink LED

    delay(20); 
  }
}


