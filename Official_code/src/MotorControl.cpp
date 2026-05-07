#include "MotorControl.h"
#include "Config.h"

Servo ESC1, ESC2, ESC3, ESC4;

volatile int motor1 = ESC_MIN;
volatile int motor2 = ESC_MIN;
volatile int motor3 = ESC_MIN;
volatile int motor4 = ESC_MIN;

void initMotors() {
    ESC1.attach(ESC_PIN_1, ESC_MIN, ESC_MAX);
    ESC2.attach(ESC_PIN_2, ESC_MIN, ESC_MAX);
    ESC3.attach(ESC_PIN_3, ESC_MIN, ESC_MAX);
    ESC4.attach(ESC_PIN_4, ESC_MIN, ESC_MAX);
}

void writeMotors() {
    //calculate speed for each motor based on joystick values
    motor1 = 1000 + throttle + rollY + yaw;
    motor2 = 1000 + throttle - rollX - yaw;
    motor3 = 1000 + throttle - rollY + yaw;
    motor4 = 1000 + throttle + rollX - yaw;

    //test output
    Serial.printf("Motors: %d, %d, %d, %d\n", motor1, motor2, motor3, motor4);
    delay(700);

    //write to motors
    ESC1.writeMicroseconds(motor1);
    ESC2.writeMicroseconds(motor2);
    ESC3.writeMicroseconds(motor3);
    ESC4.writeMicroseconds(motor4);
}

void stopMotors() {
    motor1 = motor2 = motor3 = motor4 = ESC_MIN;
}
