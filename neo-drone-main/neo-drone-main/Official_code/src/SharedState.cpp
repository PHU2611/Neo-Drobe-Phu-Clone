#include "state/SharedState.h"

#include "config/BoardConfig.h"

volatile int potValue = 0;
volatile int speedValue = 0;

int rollX = 0;
int rollY = 0;
int throttle = 0;
int yawCmd = 0;

volatile int initThrottle = ESC_MIN + 100; // Base throttle to keep motors spinning at idle

volatile int motor1 = ESC_MIN;
volatile int motor2 = ESC_MIN;
volatile int motor3 = ESC_MIN;
volatile int motor4 = ESC_MIN;

