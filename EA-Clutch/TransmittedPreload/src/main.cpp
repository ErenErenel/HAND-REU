#include <Arduino.h>
#include <Encoder.h>
#include <Adafruit_NAU7802.h>
#include <Wire.h>

// ----------------------------
// DRV8835 Motor Driver Pins (PHASE/ENABLE mode)
// ----------------------------
const int PHASE_PIN  = 29; // AIN1/APHASE
const int ENABLE_PIN = 28; // AIN2/AENABLE
const int MODE_PIN   = 27; // MODE pin — HIGH for PHASE/ENABLE

// ----------------------------
// Encoder & Control
// ----------------------------
Encoder enc(21, 22);  // Adjust pin order if direction is wrong
const int BUTTON_PIN = 33;
const int COUNTS_PER_REV = 375000;
const int TARGET_REVS = 5;

// ----------------------------
// Load Cells
// ----------------------------
Adafruit_NAU7802 loadcell1;
Adafruit_NAU7802 loadcell2;
void configureLoadCell(Adafruit_NAU7802 &sensor, const char *label);

// ----------------------------
// Revolution Pulse Output (pin 3)
// ----------------------------
const int PULSE_PIN = 3;
bool pulse_active = false;
unsigned long pulse_start_time = 0;

// We'll trigger on every new 100% bucket crossed
long last_percent_bucket = -1;  // start at -1 so the first bucket (0) can trigger

// ----------------------------
// Runtime State
// ----------------------------
bool motorActive = false;
bool wasPressed = false;
long targetCounts = TARGET_REVS * COUNTS_PER_REV;

// ----------------------------
// Function Declarations
// ----------------------------
void driveMotor(bool forward, uint8_t duty);
void brakeMotor();

void setup() {
  Serial.begin(115200);
  delay(100);

  // Motor driver setup
  pinMode(PHASE_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(MODE_PIN, OUTPUT);
  digitalWrite(MODE_PIN, HIGH); // Enable PHASE/ENABLE mode

  // Button setup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // PWM setup
  analogWriteFrequency(ENABLE_PIN, 20000); // 20 kHz PWM
  analogWriteResolution(8);                // 0–255 duty range

  enc.write(0);

  // Pulse pin setup
  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW);

  // NEW: start safely braked so motor won't run until first press
  brakeMotor();

  // NEW: ignore a button that's already held down at boot
  wasPressed = (digitalRead(BUTTON_PIN) == LOW);

  // Load cell setup
  Serial.println("Initializing load cells...");
  Wire.begin();   // Load Cell 1 on pins 18/19
  Wire1.begin();  // Load Cell 2 on pins 17/16

  if (!loadcell1.begin(&Wire)) {
    Serial.println("Load Cell 1 not found!");
    while (1);
  }
  if (!loadcell2.begin(&Wire1)) {
    Serial.println("Load Cell 2 not found!");
    while (1);
  }

  configureLoadCell(loadcell1, "Load Cell 1");
  configureLoadCell(loadcell2, "Load Cell 2");

  Serial.println("System ready. Press button to start/stop motor.");
  Serial.println("Time (ms)\tEncoder\tTransmitted (g)\tPreload (g)\t% of Cycle (cumulative)");
}

void loop() {
  bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);

  if (buttonPressed && !wasPressed) {
    wasPressed = true;

    // Only allow starting if it was off
    if (!motorActive) {
      motorActive = true;
      Serial.println("Motor started.");
      enc.write(0);
      last_percent_bucket = -1;         // reset bucket tracking
      driveMotor(true, 90);             // Forward ~27% duty (8-bit)
    } else {
      // If already running, stop it
      motorActive = false;
      Serial.println("Motor stopped by user.");
      brakeMotor();
    }
  }

  if (!buttonPressed) {
    wasPressed = false;
  }

  long enc_val = enc.read();

  // ----- Cumulative percent (keeps increasing beyond 100) -----
  float percentCycle = (enc_val * 100.0f) / (float)COUNTS_PER_REV;
  long percentBucket = (long)floor(percentCycle / 100.0f);  // 0,1,2,...

  // NEW: Only generate pulses when the motor is actually running
  if (motorActive && (percentBucket > last_percent_bucket)) {
    digitalWrite(PULSE_PIN, HIGH);
    pulse_start_time = millis();
    pulse_active = true;

    Serial.print("PULSE at ");
    Serial.print(percentBucket * 100);
    Serial.println("% cumulative");
    last_percent_bucket = percentBucket;
  }

  // End HIGH pulse after 10 ms
  if (pulse_active && (millis() - pulse_start_time >= 10)) {
    digitalWrite(PULSE_PIN, LOW);
    pulse_active = false;
  }

  // Read and log data
  if (loadcell1.available() && loadcell2.available()) {
    float preload = loadcell1.read() * 0.0091f;
    float transmitted = loadcell2.read() * 0.00965f;

    Serial.print(millis());     Serial.print("\t");
    Serial.print(enc_val);      Serial.print("\t");
    Serial.print(transmitted);  Serial.print("\t");
    Serial.print(preload);      Serial.print("\t");
    Serial.println(percentCycle);
  }

  // Stop motor automatically after N revolutions
  if (motorActive && enc_val >= targetCounts) {
    Serial.println("Motor reached target revolutions.");
    brakeMotor();
    motorActive = false;
  }
}

void configureLoadCell(Adafruit_NAU7802 &sensor, const char *label) {
  sensor.setLDO(NAU7802_3V0);
  sensor.setGain(NAU7802_GAIN_128);
  sensor.setRate(NAU7802_RATE_320SPS);

  Serial.print(label); Serial.println(" LDO, gain, and rate set.");

  for (uint8_t i = 0; i < 10; i++) {
    while (!sensor.available()) delay(1);
    sensor.read();
  }

  while (!sensor.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.print(label); Serial.println(" internal offset calibration failed, retrying...");
    delay(1000);
  }
  Serial.print(label); Serial.println(" internal offset calibrated.");

  while (!sensor.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.print(label); Serial.println(" system offset calibration failed, retrying...");
    delay(1000);
  }
  Serial.print(label); Serial.println(" system offset calibrated.");
}

// ----------------------------
// DRV8835 motor control helpers
// ----------------------------
void driveMotor(bool forward, uint8_t duty) {
  digitalWrite(PHASE_PIN, forward ? HIGH : LOW);
  analogWrite(ENABLE_PIN, duty);
}

void brakeMotor() {
  // Make sure PHASE is driven; ENABLE=0 engages brake (drive/brake)
  digitalWrite(PHASE_PIN, LOW);
  analogWrite(ENABLE_PIN, 0);
}
