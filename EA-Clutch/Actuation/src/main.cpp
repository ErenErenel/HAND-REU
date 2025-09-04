
#include <Arduino.h>
#define NUM_ACTUATORS 3  // Number of actuators
#define WINDOW_SIZE  2    // Number of actuators ON at any time

// Define arrays for actuator driver pins
// const int pos_HV_pins[NUM_ACTUATORS] = {37, 14, 0, 28, 24, 23};
// const int neg_HV_pins[NUM_ACTUATORS] = {36, 13, 1, 29, 25, 22};

// const int pos_HV_pins[NUM_ACTUATORS] = {23, 24, 28, 0, 14, 37};
// const int neg_HV_pins[NUM_ACTUATORS] = {22, 25, 29, 1, 13, 36};

// const int pos_HV_pins[NUM_ACTUATORS] = {23, 24, 28, 0, 14, 37};
// const int neg_HV_pins[NUM_ACTUATORS] = {22, 25, 29, 1, 13, 36};

// Define arrays for grounding pins
// const int pos_GND_pins[NUM_ACTUATORS] = {2, 3, 4, 5, 6, 7};
// const int neg_GND_pins[NUM_ACTUATORS] = {8, 9, 10, 11, 12, 33};

// ---------Use for when you are doing 6 fully -----------------
// const int pos_HV_pins[NUM_ACTUATORS] = {23, 24, 28, 0, 14, 37};
// const int neg_HV_pins[NUM_ACTUATORS] = {22, 25, 29, 1, 13, 36};

// const int neg_GND_pins[NUM_ACTUATORS] = {2, 4, 6, 8, 10, 12};
// const int pos_GND_pins[NUM_ACTUATORS] = {3, 5, 7, 9, 11, 33};

// --------Use for when you are doing 5 fully -----------------

// const int pos_HV_pins[NUM_ACTUATORS] = {24, 28, 0, 14, 37};
// const int neg_HV_pins[NUM_ACTUATORS] = {25, 29, 1, 13, 36};

// const int neg_GND_pins[NUM_ACTUATORS] = {4, 6, 8, 10, 12};
// const int pos_GND_pins[NUM_ACTUATORS] = {5, 7, 9, 11, 33};

// -------------------flipped-----------------

// const int pos_HV_pins[NUM_ACTUATORS] = {37, 14, 0, 28, 24};
// const int neg_HV_pins[NUM_ACTUATORS] = {36, 13, 1, 29, 25};

// const int neg_GND_pins[NUM_ACTUATORS] = {12, 10, 8, 6, 4};
// const int pos_GND_pins[NUM_ACTUATORS] = {33, 11, 9, 7, 5};


//-----------------doing 4 ---------------------

// const int pos_HV_pins[NUM_ACTUATORS] = {37, 14, 28, 24};
// const int neg_HV_pins[NUM_ACTUATORS] = {36, 13, 29, 25};

// const int neg_GND_pins[NUM_ACTUATORS] = {12, 10, 6, 4};
// const int pos_GND_pins[NUM_ACTUATORS] = {33, 11, 7, 5};

//-----------------doing 4 flipped ---------------------

// const int pos_HV_pins[NUM_ACTUATORS] = {24, 28, 14, 37};
// const int neg_HV_pins[NUM_ACTUATORS] = {25, 29, 13, 36};

// const int neg_GND_pins[NUM_ACTUATORS] = {4, 6, 10, 12};
// const int pos_GND_pins[NUM_ACTUATORS] = {5, 7, 11, 33};




//--------------doing 5 flipped ---------------------

// Define arrays for actuator driver pins
const int pos_HV_pins[NUM_ACTUATORS] = {36, 15, 25};
const int neg_HV_pins[NUM_ACTUATORS] = {33, 14, 24};

// Define arrays for grounding pins
const int pos_GND_pins[NUM_ACTUATORS] = {13, 23, 7};
const int neg_GND_pins[NUM_ACTUATORS] = {37, 22, 6};

#define BUTTON_PIN 2 //


int freq = 200;

// Timing parameters
float speed = 450; // degrees per second
float actuatorSpacing_m = 45; // degrees
float delay_time_s = actuatorSpacing_m / speed;

unsigned long delay_time_off = (unsigned long)(delay_time_s * 1000);
unsigned long delay_time_on  = delay_time_off; // same unless you want separate logic
// unsigned long delay_time_off = 0;

bool actuatorState[NUM_ACTUATORS];
bool startCommutation = false;

void controlHV(int actuatorIndex, bool state) {
  if (actuatorIndex < 0 || actuatorIndex >= NUM_ACTUATORS) return;

  if (state) {
    digitalWriteFast(pos_GND_pins[actuatorIndex], LOW);
    digitalWriteFast(neg_GND_pins[actuatorIndex], HIGH);

    analogWrite(pos_HV_pins[actuatorIndex], 127);
    analogWrite(neg_HV_pins[actuatorIndex], 127);
  } else {
    analogWrite(neg_HV_pins[actuatorIndex], 0);
    analogWrite(pos_HV_pins[actuatorIndex], 255);

    pinMode(pos_HV_pins[actuatorIndex], OUTPUT);
    pinMode(neg_HV_pins[actuatorIndex], OUTPUT);

    digitalWriteFast(pos_HV_pins[actuatorIndex], HIGH);
    digitalWriteFast(neg_HV_pins[actuatorIndex], LOW);

    digitalWriteFast(pos_GND_pins[actuatorIndex], HIGH);
    digitalWriteFast(neg_GND_pins[actuatorIndex], LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor


  for (int i = 0; i < NUM_ACTUATORS; i++) {
    pinMode(pos_HV_pins[i], OUTPUT);
    pinMode(neg_HV_pins[i], OUTPUT);

    analogWriteResolution(8);
    analogWriteFrequency(pos_HV_pins[i], freq);
    analogWriteFrequency(neg_HV_pins[i], freq);

    actuatorState[i] = false;
    controlHV(i, false);
  }

//  uncomment this if we want the actuator to turn on off rip
  // actuatorState[1] = true;    // Turn second actuator ON
  // controlHV(1, true);
  // Serial.println("Actuator 2 ON (initial)");

  // Calculate delay_time_off correctly
  // delay_time_off = time_ms / NUM_ACTUATORS;

  Serial.println("Commands:");
  Serial.println("'1' to '3' - Activate individual actuators");
  Serial.println("'0' - Turn OFF all actuators");
  Serial.println("'c' - Start commutation");
  Serial.println("'s' - Stop commutation...");
}

void loop() {
  // ---------- BUTTON CHECK (outside Serial) ----------
  static bool lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState == LOW && lastButtonState == HIGH) {
      actuatorState[1] = !actuatorState[1]; // Toggle actuator 2
      controlHV(1, actuatorState[1]);

      Serial.print("Actuator 2 ");
      Serial.println(actuatorState[1] ? "ON" : "OFF");
    }
  }

  lastButtonState = currentButtonState;

  // ---------- SERIAL COMMANDS ----------
  if (Serial.available() > 0) {
    char received = Serial.read();

    if (received >= '1' && received <= '6') {
      int actuatorIndex = received - '1';
      actuatorState[actuatorIndex] = !actuatorState[actuatorIndex]; // Toggle state
      controlHV(actuatorIndex, actuatorState[actuatorIndex]);

      Serial.print("Actuator ");
      Serial.print(actuatorIndex + 1);
      Serial.println(actuatorState[actuatorIndex] ? " ON" : " OFF");
    }
    else if (received == '0') {
      Serial.println("Turning OFF all actuators...");
      for (int i = 0; i < NUM_ACTUATORS; i++) {
        actuatorState[i] = false;
        controlHV(i, false);
      }
    }
    else if (received == 'c') {
      startCommutation = true;
      Serial.println("Starting commutation...");
    }
    else if (received == 's') {
      startCommutation = false;
      Serial.println("Stopping commutation...");
    }
  }

  // ---------- COMMUTATION ----------
  if (!startCommutation) return;

  static unsigned long lastUpdate = 0;
  static int windowStartIndex = 0;

  if (millis() - lastUpdate >= delay_time_off) {
    lastUpdate = millis();

    for (int i = 0; i < NUM_ACTUATORS; i++) {
      actuatorState[i] = false;
      controlHV(i, false);
    }

    for (int j = 0; j < WINDOW_SIZE; j++) {
      int index = (windowStartIndex + j) % NUM_ACTUATORS;
      actuatorState[index] = true;
      controlHV(index, true);
    }

    windowStartIndex = (windowStartIndex + 1) % NUM_ACTUATORS;
  }
}