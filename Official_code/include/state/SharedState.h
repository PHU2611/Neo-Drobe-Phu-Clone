#pragma once

// Runtime/shared variables (not "config"). Definitions live in `src/SharedState.cpp`.

// shared potentiometer values
extern volatile int potValue;
extern volatile int speedValue;

// web/joystick inputs
extern volatile int rollX;
extern volatile int rollY;
extern volatile int throttle;
extern volatile int yawCmd;

// motor values
extern volatile int initThrottle;
extern volatile int motor1;
extern volatile int motor2;
extern volatile int motor3;
extern volatile int motor4;

