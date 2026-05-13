#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "control/MotorControl.h" // motors
#include "net/Webserver.h"        // web & wifi
#include "sensors/IMU.h"          // IMU
#include "state/SharedState.h"   // shared variables

unsigned long lastMotorPrintMs = 0;
constexpr unsigned long motorPrintIntervalMs = 700;

void setup() {
    Serial.begin(115200);
    // set up motors, IMU and webserver (server and wifi)
    setupIMU();
    initMotors();
    initWebServer();
    

    Serial.println("System ready");
}

void loop() {

    if(Serial.available() > 0){
        char cmd = Serial.read();
    
        if (cmd == '1') {
            calibratre();
            
        } else if(cmd == '2') {
            //test output motors
            Serial.println("Input Received.");
            while(1){
                const unsigned long nowMs = millis();
                if (nowMs - lastMotorPrintMs >= motorPrintIntervalMs) {
                lastMotorPrintMs = nowMs;
                Serial.printf("Motors: %d, %d, %d, %d\n", motor1, motor2, motor3, motor4);
                Serial.printf("Euler(h,p,r): %.2f, %.2f, %.2f deg  ", x_raw, y_raw, z_raw);
                Serial.println();
                
                }   
                writeMotors();
                Serial.printf("writing...\n");
                Serial.println("!!!");

                if(Serial.read() == '3'){
                    stopMotors();
                    break;
                }
            }
            Serial.println("Exiting to menu...");
            Serial.println();
            //test output IMU
        }
    }
}
