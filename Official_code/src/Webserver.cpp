#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <cstdio>

#include "net/Webserver.h"
#include "config/BoardConfig.h"
#include "control/MotorControl.h"
#include "HtmlPages.h"
#include "state/SharedState.h"

AsyncWebServer server(80);

namespace {
portMUX_TYPE webInputMux = portMUX_INITIALIZER_UNLOCKED;

volatile int pendingThrottle = 0;
volatile int pendingYawCmd = 0;
volatile int pendingRollX = 0;
volatile int pendingRollY = 0;
volatile bool pendingInputUpdated = false;
TaskHandle_t webServerTaskHandle = nullptr;

void debugLog(const char* runId, const char* hypothesisId, const char* location, const char* message, int throttleValue, int yawValue, int rollXValue, int rollYValue) {
    FILE* logFile = fopen("debug-b9e027.log", "a");
    if (logFile == nullptr) {
        return;
    }
    fprintf(
        logFile,
        "{\"sessionId\":\"b9e027\",\"runId\":\"%s\",\"hypothesisId\":\"%s\",\"location\":\"%s\",\"message\":\"%s\",\"data\":{\"throttle\":%d,\"yawCmd\":%d,\"rollX\":%d,\"rollY\":%d},\"timestamp\":%lu}\n",
        runId,
        hypothesisId,
        location,
        message,
        throttleValue,
        yawValue,
        rollXValue,
        rollYValue,
        static_cast<unsigned long>(millis())
    );
    fclose(logFile);
}

void webServerTask(void* /*parameter*/) {
    initWebServer();
    vTaskDelete(nullptr);
}
} // namespace

void initWebServer() {
    WiFi.softAP(WIFI_SSID, WIFI_PASS);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send_P(200, "text/html", HTML_CONTROL_PAGE);
    });

    // Receive joystick data
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *req) {
        int nextThrottle = pendingThrottle;
        int nextYaw = pendingYawCmd;
        int nextRollX = pendingRollX;
        int nextRollY = pendingRollY;

        if (req->hasParam("throttle")) nextThrottle = req->getParam("throttle")->value().toInt();
        if (req->hasParam("yaw"))      nextYaw      = req->getParam("yaw")->value().toInt();
        if (req->hasParam("rollX"))    nextRollX    = req->getParam("rollX")->value().toInt();
        if (req->hasParam("rollY"))    nextRollY    = req->getParam("rollY")->value().toInt();

        portENTER_CRITICAL(&webInputMux);
        pendingThrottle      = nextThrottle;
        pendingYawCmd        = nextYaw;
        pendingRollX         = nextRollX;
        pendingRollY         = nextRollY;
        pendingInputUpdated  = true;
        portEXIT_CRITICAL(&webInputMux);

        // #region agent log
        debugLog("initial", "H1", "src/Webserver.cpp:/update", "web_update_received", nextThrottle, nextYaw, nextRollX, nextRollY);
        // #endregion

        req->send(200, "text/plain", "OK");
    });

    server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *req) {
        stopMotors();
        req->send(200, "text/plain", "STOP");
    });

    server.begin();
}

void startWebServerTask() {
    if (webServerTaskHandle != nullptr) {
        return;
    }
    xTaskCreatePinnedToCore(webServerTask, "web-server", 8192, nullptr, 1, &webServerTaskHandle, 0);

    // #region agent log
    debugLog("initial", "H4", "src/Webserver.cpp:startWebServerTask", "web_server_task_started", throttle, yawCmd, rollX, rollY);
    // #endregion
}

bool fetchLatestWebInputs() {
    bool hasNewInput = false;
    int nextThrottle = 0;
    int nextYaw = 0;
    int nextRollX = 0;
    int nextRollY = 0;

    portENTER_CRITICAL(&webInputMux);
    hasNewInput = pendingInputUpdated;
    if (hasNewInput) {
        nextThrottle = pendingThrottle;
        nextYaw      = pendingYawCmd;
        nextRollX    = pendingRollX;
        nextRollY    = pendingRollY;
        pendingInputUpdated = false;
    }
    portEXIT_CRITICAL(&webInputMux);

    if (hasNewInput) {
        throttle = nextThrottle;
        yawCmd   = nextYaw;
        rollX    = nextRollX;
        rollY    = nextRollY;

        // #region agent log
        debugLog("initial", "H2", "src/Webserver.cpp:fetchLatestWebInputs", "main_loop_applied_latest_inputs", throttle, yawCmd, rollX, rollY);
        // #endregion
    }

    return hasNewInput;
}
