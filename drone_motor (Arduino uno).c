#include <Servo.h>

// Define the pins connected to each ESC (for brushless motors)
const int motor1Pin = 14;  // Motor 1
const int motor2Pin = 12;  // Motor 2
const int motor3Pin = 13;  // Motor 3
const int motor4Pin = 15;  // Motor 4

// Create Servo objects for each motor
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

//define pulse width
int minPulseWidth = 1000;
int maxPulseWidth = 2000;

void setup() {
  // Attach the Servo objects to the respective pins
  motor1.attach(motor1Pin, minPulseWidth, maxPulseWidth);
  motor2.attach(motor2Pin,minPulseWidth, maxPulseWidth);
  motor3.attach(motor3Pin, minPulseWidth, maxPulseWidth);
  motor4.attach(motor4Pin, minPulseWidth, maxPulseWidth);

  // Initialize motors by sending the minimum PWM signal (usually 1000 or similar for ESC)
  motor1.writeMicroseconds(1000);  // Neutral position, motor not spinning
  motor2.writeMicroseconds(1000);
  motor3.writeMicroseconds(1000);
  motor4.writeMicroseconds(1000);

  delay(2000);  // Wait 2 seconds for ESCs to initialize

  Serial.begin(115200);  // Start serial communication for debugging
  Serial.println("Motors Initialized and Ready!");
}

void loop() {
  // Keep motors at a low speed
  motor1.writeMicroseconds(1000);
  motor2.writeMicroseconds(1000);
  motor3.writeMicroseconds(1000);
  motor4.writeMicroseconds(1000);

  delay(5000);  // Wait for 5 seconds before restarting loop
}
