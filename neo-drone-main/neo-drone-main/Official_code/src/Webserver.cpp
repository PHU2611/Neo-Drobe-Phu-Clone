#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "net/Webserver.h"
#include "config/NetworkConfig.h"
#include "state/SharedState.h"
#include "control/MotorControl.h"
#include "net/HTMLPages.h"

AsyncWebServer server(80);

void initWebServer() {

    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send_P(200, "text/html", HTML_CONTROL_PAGE);
    });

    // server.on("/set", HTTP_GET, [](AsyncWebServerRequest *req){
    //     if (req->hasParam("m1")) motor1 = req->getParam("m1")->value().toInt();
    //     if (req->hasParam("m2")) motor2 = req->getParam("m2")->value().toInt();
    //     if (req->hasParam("m3")) motor3 = req->getParam("m3")->value().toInt();
    //     if (req->hasParam("m4")) motor4 = req->getParam("m4")->value().toInt();
    //     req->send(200, "text/plain", "OK");
    // });

    // Receive joystick data
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *req) {
        if (req->hasParam("throttle")) throttle = req->getParam("throttle")->value().toInt();
        if (req->hasParam("yaw")) yawCmd = req->getParam("yaw")->value().toInt();
        if (req->hasParam("rollX")) rollX = req->getParam("rollX")->value().toInt();
        if (req->hasParam("rollY")) rollY = req->getParam("rollY")->value().toInt();

        //Serial.printf("Throttle=%d | Yaw=%d | RollX=%d | RollY=%d\n", throttle, yawCmd, rollX, rollY);
        req->send(200, "text/plain", "OK");
    });

    server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *req){
        stopMotors();
        req->send(200, "text/plain", "STOP");
    });

    server.begin();
}
