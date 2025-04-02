// Ultrasonic Sensor Pins
#define TRIG_PIN 9
#define ECHO_PIN 10

// Motor Control Pins (One L298N per motor)
// Motor 1 (Front Left)
#define M1_ENA 3   // PWM
#define M1_IN1 4
#define M1_IN2 5

// Motor 2 (Front Right)
#define M2_ENB 6   // PWM
#define M2_IN3 7
#define M2_IN4 8

// Motor 3 (Rear Left)
#define M3_ENC 11  // PWM
#define M3_IN5 12
#define M3_IN6 13

// Motor 4 (Rear Right)
#define M4_END A0  // PWM
#define M4_IN7 A1
#define M4_IN8 A2

// Speed Parameters (PWM values 0-255)
#define FULL_SPEED 180    // 78% power
#define MEDIUM_SPEED 100  // 59% power (NEW)
#define SLOW_SPEED 50    // 39% power
#define STOP 0

// Distance Thresholds (cm)
#define FAR_DIST 200       // Full speed
#define MID_DIST 140       // Medium speed (NEW)
#define WARNING_DIST 70   // Slow speed
#define DANGER_DIST 20    // Emergency stop

void setup() {
  Serial.begin(9600);
  
  // Initialize all motor control pins
  pinMode(M1_ENA, OUTPUT); pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_ENB, OUTPUT); pinMode(M2_IN3, OUTPUT); pinMode(M2_IN4, OUTPUT);
  pinMode(M3_ENC, OUTPUT); pinMode(M3_IN5, OUTPUT); pinMode(M3_IN6, OUTPUT);
  pinMode(M4_END, OUTPUT); pinMode(M4_IN7, OUTPUT); pinMode(M4_IN8, OUTPUT);

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  allStop();
  Serial.println("4WD System Ready - Medium Speed Added");
}

void loop() {
  float distance = getDistance();
  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");

  // Enhanced speed control with medium speed
  if (distance <= DANGER_DIST) {
    emergencyStop();
    Serial.println("! EMERGENCY STOP !");
  } 
  else if (distance <= WARNING_DIST) {
    setFrontMotors(SLOW_SPEED);
    setRearMotors(MEDIUM_SPEED);  // Rear motors slightly faster
    Serial.println("WARNING: Slow Speed");
  }
  else if (distance <= MID_DIST) {  // NEW MEDIUM SPEED RANGE
    setAllMotors(MEDIUM_SPEED);
    Serial.println("APPROACHING: Medium Speed");
  }
  else if (distance <= FAR_DIST) {  // NEW FAR RANGE
    setAllMotors(FULL_SPEED * 0.8);  // Slightly reduced full speed
    Serial.println("CLEAR: High Speed");
  }
  else {
    setAllMotors(FULL_SPEED);
    Serial.println("VERY FAR: Full Speed");
  }

  // Set all motors forward
  setAllDirections(HIGH, LOW);
  delay(100);
}

// === IMPROVED MOTOR CONTROL FUNCTIONS === //

void setFrontMotors(int speed) {
  analogWrite(M1_ENA, speed);
  analogWrite(M2_ENB, speed);
}

void setRearMotors(int speed) {
  analogWrite(M3_ENC, speed);
  analogWrite(M4_END, speed);
}

void setAllMotors(int speed) {
  setFrontMotors(speed);
  setRearMotors(speed);
}

void setAllDirections(bool dir1, bool dir2) {
  digitalWrite(M1_IN1, dir1); digitalWrite(M1_IN2, dir2);
  digitalWrite(M2_IN3, dir1); digitalWrite(M2_IN4, dir2);
  digitalWrite(M3_IN5, dir1); digitalWrite(M3_IN6, dir2);
  digitalWrite(M4_IN7, dir1); digitalWrite(M4_IN8, dir2);
}

// === SAFETY FUNCTIONS === //
void allStop() {
  setAllMotors(STOP);
  setAllDirections(LOW, LOW);
}

void emergencyStop() {
  // Immediate stop for front, gradual for rear
  setFrontMotors(STOP);
  for (int i = MEDIUM_SPEED; i >= STOP; i -= 20) {
    setRearMotors(i);
    delay(15);
  }
  allStop();
}

// === SENSOR FUNCTION === //
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}