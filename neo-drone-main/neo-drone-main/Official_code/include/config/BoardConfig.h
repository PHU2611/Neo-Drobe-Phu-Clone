#pragma once
#include <Arduino.h>

// ESC limits
constexpr int ESC_MIN = 1000;
constexpr int ESC_MAX = 2000;

// ESC pins
constexpr int EscPin1 = 25;
constexpr int EscPin2 = 18;
constexpr int EscPin3 = 4;
constexpr int EscPin4 = 14;

// I2C -> IMU
constexpr int I2C_SCL = 22; // clock pin for I2C
constexpr int I2C_SDA = 23; // data pin for I2C

// Potentiometer pin
constexpr int potPin = 34;

