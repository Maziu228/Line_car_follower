#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

// Motor control pins
const int AIN1 = 13;   // Right motor control pin 1
const int AIN2 = 12;   // Right motor control pin 2
const int PWMA = 6;    // Right motor speed control (PWM)

const int BIN1 = 9;    // Left motor control pin 1
const int BIN2 = 8;    // Left motor control pin 2
const int PWMB = 10;   // Left motor speed control (PWM)

void setup() {
  Serial.begin(9600);
  Serial.println("QTR calibration demo");

  // Sensor setup
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){5, A2, A0, 11, A3, 4}, SensorCount);
  qtr.setEmitterPin(2);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // calibration running

  delay(500);

  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
    delay(10);
    Serial.println(i);
  }
  digitalWrite(LED_BUILTIN, LOW); // calibration done

  // Motor pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.println("Calibration is done");
}

void loop() {
  // Read line position (0 = far left, 5000 = far right)
  uint16_t position = qtr.readLineBlack(sensorValues);

  // Print sensor values
  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.print("Position: ");
  Serial.println(position);

  // Motor control logic
  if (position > 3500) {
    // Turn left
    rightMotor(75);
    leftMotor(25);
  } else if (position > 1500) {
    // Turn right
    rightMotor(25);
    leftMotor(75);
  } else {
    // Go straight
    rightMotor(75);
    leftMotor(75);
  }

  delay(100);
}

// Motor functions
void rightMotor(int speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);
}

void leftMotor(int speed) {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed);
}
