#include <Arduino.h>
#include "MotorControl.h"

void calibratre(){
   delay(2000);
    ESC1.attach(EscPin1, 1000, 2000); //Motor 1
    ESC2.attach(EscPin2, 1000, 2000); //Motor 2
    ESC3.attach(EscPin3, 1000, 2000); //Motor 3
    ESC4.attach(EscPin4, 1000, 2000); //Motor 4

    Serial.println();
    Serial.println("Calibration step 1. Disconnect the battery.");
    Serial.println("Press any key to continue.");
    WaitForKeyStroke();
    ESC1.writeMicroseconds(2000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC2.writeMicroseconds(2000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC3.writeMicroseconds(2000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC4.writeMicroseconds(2000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode


    Serial.println();
    Serial.println("Calibration step 2. Connect the battery.");
    Serial.println("Wait for two short bips.");
    Serial.println("Press any key to continue.");
    WaitForKeyStroke();

    ESC1.writeMicroseconds(1000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC2.writeMicroseconds(1000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC3.writeMicroseconds(1000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode
    ESC4.writeMicroseconds(1000); // Sending MAX_SIGNAL tells the ESC to enter calibration mode

    Serial.println();
    Serial.println("Wait for 4 short bips, and one long bip.");
    Serial.println("Press any key to finish.");
    WaitForKeyStroke();
}

void WaitForKeyStroke() {
  while (!Serial.available()) {
    delay(10);
  }
  while (Serial.available()) {
    Serial.read();
  }
}
    