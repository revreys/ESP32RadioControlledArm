#include <Arduino.h>
#include <ESP32Servo.h>

// pins
static const int BASE_PIN     = 4;
static const int SHOULDER_PIN = 5;

// servo pulse range
static const int SERVO_MIN_US = 500;
static const int SERVO_MAX_US = 2500;

//
static const int SHOULDER_MIN_US = 900;
static const int SHOULDER_MAX_US = 1600;
static const int SHOULDER_MID_US = 2300;

Servo baseServo;
Servo shoulderServo;

// helpers
void baseMoveDeg(int deg, int holdMs = 0) {
  baseServo.write(deg);
  if (holdMs > 0) delay(holdMs);
}

void shoulderMoveUs(int us, int holdMs = 0) {
  shoulderServo.writeMicroseconds(us);
  if (holdMs > 0) delay(holdMs);
}

void sweepBase(int fromDeg, int toDeg, int stepDeg, int stepDelayMs) {
  if (fromDeg < toDeg) {
    for (int d = fromDeg; d <= toDeg; d += stepDeg) {
      baseServo.write(d);
      delay(stepDelayMs);
    }
  } else {
    for (int d = fromDeg; d >= toDeg; d -= stepDeg) {
      baseServo.write(d);
      delay(stepDelayMs);
    }
  }
}

void rapidShoulderEndToEnd(int cycles, int holdEachEndMs) {
  for (int i = 0; i < cycles; i++) {
    shoulderServo.writeMicroseconds(SHOULDER_MIN_US);
    delay(holdEachEndMs);
    shoulderServo.writeMicroseconds(SHOULDER_MAX_US);
    delay(holdEachEndMs);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1200);

  baseServo.setPeriodHertz(50);
  shoulderServo.setPeriodHertz(50);

  baseServo.attach(BASE_PIN, SERVO_MIN_US, SERVO_MAX_US);
  shoulderServo.attach(SHOULDER_PIN, SERVO_MIN_US, SERVO_MAX_US);

  // Safe start
  baseServo.write(90);
  shoulderServo.writeMicroseconds(SHOULDER_MID_US);

  Serial.println("Test run starting...");
  delay(2000);
}

void loop() {

  
  // slow base rotation

  Serial.println("Phase 1: Base slow sweep");
  shoulderServo.writeMicroseconds(SHOULDER_MID_US); 

  sweepBase(60, 120, 1, 30); 
  delay(500);
  sweepBase(120, 60, 1, 30); 
  delay(1000);


  // rapid base rotation
 
  Serial.println("Phase 2: Base rapid sweep");
  shoulderServo.writeMicroseconds(SHOULDER_MID_US);

  sweepBase(60, 120, 3, 8);
  delay(250);
  sweepBase(120, 60, 3, 8);
  delay(1000);

  // rapid shoulder movement
  
  Serial.println("Phase 3: Shoulder rapid end-to-end");
  baseServo.write(90);         // lock base still

  rapidShoulderEndToEnd(6, 120);   // 6 cycles
  Serial.println("Phase 3: Shoulder -> middle");
  shoulderServo.writeMicroseconds(SHOULDER_MID_US);
  delay(1500);

  
  // move together slowly
  
  Serial.println("Phase 4: Slow synchronized move");

  // move base
  const int steps = 120;
  for (int i = 0; i <= steps; i++) {
    int baseDeg = 60 + (60 * i) / steps;

    int shoulderUs = SHOULDER_MIN_US +
      (int)((long)(SHOULDER_MAX_US - SHOULDER_MIN_US) * i / steps);

    baseServo.write(baseDeg);
    shoulderServo.writeMicroseconds(shoulderUs);
    delay(20);
  }

  delay(800);

  
  for (int i = 0; i <= steps; i++) {
    int baseDeg = 120 - (60 * i) / steps;

    int shoulderUs = SHOULDER_MAX_US -
      (int)((long)(SHOULDER_MAX_US - SHOULDER_MIN_US) * i / steps);

    baseServo.write(baseDeg);
    shoulderServo.writeMicroseconds(shoulderUs);
    delay(20);
  }

  delay(1500);

  // done
  Serial.println("Test run complete. Repeating in 5 seconds...");
  delay(5000);
}