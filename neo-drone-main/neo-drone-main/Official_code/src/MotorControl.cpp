#include "control/MotorControl.h"
#include "config/BoardConfig.h"
#include "state/SharedState.h"
#include "sensors/IMU.h"
#include "control/PID.h"

namespace {
Servo ESC1, ESC2, ESC3, ESC4;
}

    

void initMotors() {
    ESC1.attach(EscPin1, ESC_MIN, ESC_MAX);
    ESC2.attach(EscPin2, ESC_MIN, ESC_MAX);
    ESC3.attach(EscPin3, ESC_MIN, ESC_MAX);
    ESC4.attach(EscPin4, ESC_MIN, ESC_MAX);
}


void writeMotors() {
    pid_tuning(); //update pid value

    //calculate speed for each motor based on joystick values
    /*
    1 2
    4 3
    */
    motor1 = constrain(initThrottle + throttle - static_cast<int>(pitchPid) + static_cast<int>(rollPid) + static_cast<int>(yawCmd), ESC_MIN, ESC_MAX);
    motor2 = constrain(initThrottle + throttle - static_cast<int>(pitchPid) - static_cast<int>(rollPid) - static_cast<int>(yawCmd), ESC_MIN, ESC_MAX);
    motor3 = constrain(initThrottle + throttle + static_cast<int>(pitchPid) - static_cast<int>(rollPid) + static_cast<int>(yawCmd), ESC_MIN, ESC_MAX);
    motor4 = constrain(initThrottle + throttle + static_cast<int>(pitchPid) + static_cast<int>(rollPid) - static_cast<int>(yawCmd), ESC_MIN, ESC_MAX);
    

    //write to motors
    ESC1.writeMicroseconds(motor1);
    ESC2.writeMicroseconds(motor2);
    ESC3.writeMicroseconds(motor3);
    ESC4.writeMicroseconds(motor4);
}

void stopMotors() {
    motor1 = motor2 = motor3 = motor4 = ESC_MIN;
    ESC1.writeMicroseconds(motor1);
    ESC2.writeMicroseconds(motor2);
    ESC3.writeMicroseconds(motor3);
    ESC4.writeMicroseconds(motor4);
}

void calibratre(){
   delay(2000);
    ESC1.attach(EscPin1, ESC_MIN, ESC_MAX); //Motor 1
    ESC2.attach(EscPin2, ESC_MIN, ESC_MAX); //Motor 2
    ESC3.attach(EscPin3, ESC_MIN, ESC_MAX); //Motor 3
    ESC4.attach(EscPin4, ESC_MIN, ESC_MAX); //Motor 4

    Serial.println();
    Serial.println("Calibration step 1. Disconnect the battery.");
    Serial.println("Press any key to continue.");
    Serial.read();
    WaitForKeyStroke();
    ESC1.writeMicroseconds(ESC_MAX); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC2.writeMicroseconds(ESC_MAX); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC3.writeMicroseconds(ESC_MAX); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC4.writeMicroseconds(ESC_MAX); // Sending MAX_SIGNAL tells the ESC to enter calibration mode


    Serial.println();
    Serial.println("Calibration step 2. Connect the battery.");
    Serial.println("Wait for two short bips.");
    Serial.println("Press any key to continue.");
    WaitForKeyStroke();

    ESC1.writeMicroseconds(ESC_MIN); // Sending MIN_SIGNAL completes calibration step
    ESC2.writeMicroseconds(ESC_MIN); // Sending MIN_SIGNAL completes calibration step
    ESC3.writeMicroseconds(ESC_MIN); // Sending MIN_SIGNAL completes calibration step
    ESC4.writeMicroseconds(ESC_MIN); // Sending MIN_SIGNAL completes calibration step

    Serial.println();
    Serial.println("Wait for 4 short bips, and one long bip.");
    Serial.println("Press any key to finish.");
    WaitForKeyStroke();
    Serial.println("fucu");
}

void WaitForKeyStroke() {
  while (!Serial.available()) {
    delay(10);
  }
  while (Serial.available()) {
    Serial.read();
  }
}