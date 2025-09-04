#include <Arduino.h>
#include <Encoder.h>

// Motor control pins
const int IN1 = 29;
const int IN2 = 28;

// Encoder pins
Encoder enc(21, 22);

// Button pin
const int BUTTON_PIN = 33;

// Encoder and target parameters
const int COUNTS_PER_REV = 375000;  // Adjust based on encoder
const int TARGET_REVS = 5;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  analogWriteFrequency(IN1, 1000);
  analogWriteFrequency(IN2, 1000);

  enc.write(0);  // Reset encoder
}

void loop() {
  static bool wasPressed = false;
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  if (buttonPressed && !wasPressed) {
    long targetCounts = TARGET_REVS * COUNTS_PER_REV;
    enc.write(0);  // Reset before move

    analogWrite(IN1, 180);  // Forward speed
    analogWrite(IN2, 0);

    while (enc.read() < targetCounts) {
      delay(10);  // Light delay for polling
    }

    analogWrite(IN1, 0);  // Stop motor
    analogWrite(IN2, 0);
  }

  wasPressed = buttonPressed;
}
