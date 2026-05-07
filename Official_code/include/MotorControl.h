#pragma once //protection against multiple inclusions in the same file
#include <ESP32Servo.h>

void initMotors();
void writeMotors();
void stopMotors();
void WaitForKeyStroke();
void calibratre();