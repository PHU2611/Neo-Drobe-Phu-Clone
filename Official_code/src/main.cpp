#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <cstdio>
#include "control/MotorControl.h" // motors
#include "net/Webserver.h"        // web & wifi
#include "sensors/IMU.h"          // IMU
#include "state/SharedState.h"    // shared variables

unsigned long lastMotorPrintMs = 0;
constexpr unsigned long motorPrintIntervalMs = 700;
unsigned long lastWebFetchDebugMs = 0;

namespace {
void debugLogMain(const char* runId, const char* hypothesisId, const char* location, const char* message) {
    FILE* logFile = fopen("debug-b9e027.log", "a");
    if (logFile == nullptr) {
        return;
    }
    fprintf(
        logFile,
        "{\"sessionId\":\"b9e027\",\"runId\":\"%s\",\"hypothesisId\":\"%s\",\"location\":\"%s\",\"message\":\"%s\",\"data\":{\"throttle\":%d,\"yawCmd\":%d,\"rollX\":%d,\"rollY\":%d,\"motor1\":%d,\"motor2\":%d,\"motor3\":%d,\"motor4\":%d},\"timestamp\":%lu}\n",
        runId,
        hypothesisId,
        location,
        message,
        throttle,
        yawCmd,
        rollX,
        rollY,
        motor1,
        motor2,
        motor3,
        motor4,
        static_cast<unsigned long>(millis())
    );
    fclose(logFile);
}
} // namespace

void setup() {
    Serial.begin(115200);
    initMotors();
    startWebServerTask();

    // #region agent log
    debugLogMain("initial", "H4", "src/main.cpp:setup", "setup_completed_after_webserver_task_start");
    // #endregion

    Serial.println("System ready");
}

void loop() {
    Serial.printf("Start up (1), loop (0)\n");

    if (Serial.read() == '1') {
        calibratre();
    } else {
        const bool hasNewWebInput = fetchLatestWebInputs();
        if (hasNewWebInput) {
            // #region agent log
            debugLogMain("initial", "H3", "src/main.cpp:loop", "pid_loop_received_fresh_web_inputs");
            // #endregion
        }

        // Periodic motor state print
        const unsigned long nowMs = millis();
        if (nowMs - lastMotorPrintMs >= motorPrintIntervalMs) {
            lastMotorPrintMs = nowMs;
            Serial.printf("Motors: %d, %d, %d, %d\n", motor1, motor2, motor3, motor4);
            Serial.printf("Euler(h,p,r): %.2f, %.2f, %.2f deg  ", x_raw, y_raw, z_raw);

            if (nowMs - lastWebFetchDebugMs >= 1000) {
                lastWebFetchDebugMs = nowMs;
                // #region agent log
                debugLogMain("initial", "H5", "src/main.cpp:loop", "periodic_main_loop_state");
                // #endregion
            }
        }

        writeMotors();
    }
}
