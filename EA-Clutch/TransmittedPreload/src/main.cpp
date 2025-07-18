#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"

NAU7802 myScale;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire1.begin();
  Wire1.setClock(400000);

  if (!myScale.begin(Wire1)) {
    Serial.println("Scale not detected.");
    while (1);
  }

  Serial.println("Scale detected.");
  myScale.setSampleRate(NAU7802_SPS_320);
  myScale.calibrateAFE();

  Serial.println("Press 't' to tare, 'c' to calibrate with known weight.");
}

void loop() {
  if (myScale.available()) {
    float weight = myScale.getWeight();
    Serial.print("Weight (g): ");
    Serial.println(weight, 2); // Show 2 decimal places
  }

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 't') {
      Serial.println("Taring... Make sure scale is empty.");
      myScale.calculateZeroOffset(64);
      Serial.println("Tare complete.");
    }
    else if (c == 'c') {
      Serial.println("Place known weight on scale, then type the weight in grams (e.g., 70.0):");
      while (Serial.available()) Serial.read(); // Clear buffer
      while (Serial.available() == 0); // Wait for input

      float knownWeight = Serial.parseFloat();
      Serial.print("Calibrating using weight: ");
      Serial.println(knownWeight);

      myScale.calculateCalibrationFactor(knownWeight, 64);

      Serial.print("Calibration factor set: ");
      Serial.println(myScale.getCalibrationFactor());
    }
  }

  delay(200); // Adjust refresh rate as needed
}
