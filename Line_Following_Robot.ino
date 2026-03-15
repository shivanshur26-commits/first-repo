#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12

// ✅ TUNE THESE VALUES
#define MOTOR_SPEED   130   // Straight speed  → lower if missing turns
#define TURN_SPEED    150   // Turning speed   → raise if turns are weak
#define RIGHT_TRIM      -10   // +/- to fix drift right/left (range: -20 to +20)

int enableRightMotor = 6;
int rightMotorPin1 = 7;
int rightMotorPin2 = 8;
int enableLeftMotor = 5;
int leftMotorPin1 = 9;
int leftMotorPin2 = 10;

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000010;
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);

  // Stop motors at start
  analogWrite(enableRightMotor, 0);
  analogWrite(enableLeftMotor, 0);
}

void loop() {
  int R = digitalRead(IR_SENSOR_RIGHT);
  int L = digitalRead(IR_SENSOR_LEFT);

  if (L == LOW && R == LOW) {
    // ✅ Both on white → Go STRAIGHT
    rotateMotor(MOTOR_SPEED + RIGHT_TRIM, MOTOR_SPEED - RIGHT_TRIM);
  }
  else if (R == HIGH && L == LOW) {
    // ✅ Right on black → Turn RIGHT (stop left, spin right)
    rotateMotor(-TURN_SPEED, TURN_SPEED);
  }
  else if (L == HIGH && R == LOW) {
    // ✅ Left on black → Turn LEFT (stop right, spin left)
    rotateMotor(TURN_SPEED, -TURN_SPEED);
  }
  else {
    // ✅ Both on black → STOP
    rotateMotor(0, 0);
  }
}

void rotateMotor(int rightSpeed, int leftSpeed) {
  // Right motor direction
  if (rightSpeed < 0) {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  } else if (rightSpeed > 0) {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  } else {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, HIGH);
  }

  // Left motor direction
  if (leftSpeed < 0) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  } else if (leftSpeed > 0) {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  } else {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, HIGH);
  }

  analogWrite(enableRightMotor, abs(rightSpeed));
  analogWrite(enableLeftMotor,  abs(leftSpeed));
}
