/*
Config used to declare pins 
and shared variables / values across files
*/

#pragma once
#include <Arduino.h>

// WiFi
constexpr const char* WIFI_SSID = "ESP32_test";
constexpr const char* WIFI_PASS = "12345678";

// ESC limits
constexpr int ESC_MIN = 1000;
constexpr int ESC_MAX = 2000;

//declare pins
//ESC
constexpr int EscPin1 = 18; 
constexpr int EscPin2 = 19; 
constexpr int EscPin3 = 16; 
constexpr int EscPin4 = 17; 
//I2C -> IMU
constexpr int SCL = 22;   // clock pin for I2C
constexpr int SDA = 21;   // data pin for I2C

// Potentiometer pin
constexpr int potPin = 34;   

//shared potentiometer values
extern volatile int potValue;  // Variable to store potentiometer value
extern volatile int speedValue; // Variable to store mapped speed value

//web shared values
extern int rollX;
extern int rollY;
extern int throttle;
extern int yaw;

// Shared motor values
extern volatile int motor1;
extern volatile int motor2;
extern volatile int motor3;
extern volatile int motor4;
