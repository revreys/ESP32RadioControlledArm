#include <Arduino.h>
#include <ESP32Servo.h>
#include "ServoCalibration.h"

// pins
static const int BASE_PIN     = 4;
static const int SHOULDER_PIN = 5;

// attach range (wide)
static const int ATTACH_MIN_US = 500;
static const int ATTACH_MAX_US = 2500;

// fine tuning
static const int START_HOLD_MS = 300;    // pause at middle before each run
static const int ENDPOINT_HOLD_MS = 80; // pause at each turnaround point
static const int SWEEP_DELAY_MS = 8;     // delay between microsecond steps
static const int SWEEP_STEP_US = 10;     // movement resolution within one swing
static const int WINDOW_STEP_US = 100;   // endpoint shrink each full oscillation

Servo baseServo;
Servo shoulderServo;

void oscillateTowardMid(Servo &s, int minUs, int midUs, int maxUs,
                        int sweepStepUs, int windowStepUs, const char* name)
{
  Serial.print("=== ");
  Serial.print(name);
  Serial.println(": MIN<->MAX oscillation shrinking toward MID ===");

  s.writeMicroseconds(midUs);
  delay(START_HOLD_MS);

  int lo = minUs;
  int hi = maxUs;

  while (true) {
    // sweep up from low endpoint to high endpoint
    for (int pos = lo; pos <= hi; pos += sweepStepUs) {
      s.writeMicroseconds(pos);
      delay(SWEEP_DELAY_MS);
    }
    s.writeMicroseconds(hi);
    delay(ENDPOINT_HOLD_MS);

    // sweep down from high endpoint to low endpoint
    for (int pos = hi; pos >= lo; pos -= sweepStepUs) {
      s.writeMicroseconds(pos);
      delay(SWEEP_DELAY_MS);
    }
    s.writeMicroseconds(lo);
    delay(ENDPOINT_HOLD_MS);

    int nextLo = lo + windowStepUs;
    int nextHi = hi - windowStepUs;

    if (nextLo >= midUs || nextHi <= midUs || nextLo >= nextHi) {
      s.writeMicroseconds(midUs);
      delay(START_HOLD_MS);
      break;
    }

    lo = nextLo;
    hi = nextHi;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  baseServo.setPeriodHertz(50);
  shoulderServo.setPeriodHertz(50);

  baseServo.attach(BASE_PIN, ATTACH_MIN_US, ATTACH_MAX_US);
  shoulderServo.attach(SHOULDER_PIN, ATTACH_MIN_US, ATTACH_MAX_US);

  //center servos
  baseServo.writeMicroseconds(BASE_MID_US);
  shoulderServo.writeMicroseconds(SHOULDER_MID_US);

  Serial.println("Starting: SHOULDER first, then BASE.");
  delay(800);
}

void loop() {
  // shoulder movement
  baseServo.writeMicroseconds(BASE_MID_US);
  oscillateTowardMid(shoulderServo,
                     SHOULDER_MIN_US, SHOULDER_MID_US, SHOULDER_MAX_US,
                     SWEEP_STEP_US, WINDOW_STEP_US, "SHOULDER");

  // base movement
  shoulderServo.writeMicroseconds(SHOULDER_MID_US);
  oscillateTowardMid(baseServo,
                     BASE_MIN_US, BASE_MID_US, BASE_MAX_US,
                     SWEEP_STEP_US, WINDOW_STEP_US, "BASE");

  delay(1500);
}
