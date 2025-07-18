#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include <SoftwareSerial.h>

// --- Stepper Driver Settings ---
#define EN_PIN        6
#define DIR_PIN       5
#define STEP_PIN      11
#define DRIVER_ADDRESS 0b00
#define R_SENSE       0.11f

// --- UART to TMC2209 ---
SoftwareSerial TMCSerial(8, 9); // RX = 8, TX = 9
TMC2209Stepper driver(&TMCSerial, R_SENSE, DRIVER_ADDRESS);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// --- Button Pins (using external pull-ups) ---
#define CW_BUTTON     2
#define CCW_BUTTON    3
#define STOP_BUTTON   4

void setup() {
  // Enable pin
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);  // Enable driver (LOW = active)

  // Set button pins as inputs (external pull-ups in hardware)
  pinMode(CW_BUTTON, INPUT);
  pinMode(CCW_BUTTON, INPUT);
  pinMode(STOP_BUTTON, INPUT);

  // Initialize Serial (for optional debug)
  Serial.begin(115200);
  TMCSerial.begin(115200);

  // Initialize TMC2209 driver
  driver.begin();
  driver.toff(5);
  driver.rms_current(600);        // Motor current (mA)
  driver.microsteps(16);          // Set microstepping
  driver.en_spreadCycle(false);   // Enable StealthChop2 (quiet mode)
  driver.pwm_autoscale(true);     // Auto current control

  // Set motor speed/acceleration parameters
  stepper.setMaxSpeed(1000);      // Steps/sec
  stepper.setAcceleration(2000);  // Steps/sec^2
  stepper.setSpeed(0);            // Start stopped

  Serial.println("System ready. Use buttons to control motor.");
}

void loop() {
  // Read buttons (LOW = pressed, due to pull-ups)
  bool cwPressed = digitalRead(CW_BUTTON) == LOW;
  bool ccwPressed = digitalRead(CCW_BUTTON) == LOW;
  bool stopPressed = digitalRead(STOP_BUTTON) == LOW;

  if (cwPressed) {
    stepper.setSpeed(400);  // Clockwise
  }
  else if (ccwPressed) {
    stepper.setSpeed(-400); // Counterclockwise
  }
  else if (stopPressed) {
    stepper.setSpeed(0);    // Stop
  }

  stepper.runSpeed();  // Move motor at set speed (non-blocking)
}
