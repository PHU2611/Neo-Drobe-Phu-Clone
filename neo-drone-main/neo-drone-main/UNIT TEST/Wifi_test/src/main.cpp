#include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiServer.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const char* ssid = "ESP32_test";
const char* password = "12345678";

//simple website
String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>ESP32 Joystick</title>
<style>
  body { font-family: sans-serif; background:#f5f5f5; text-align:center; }
  h2 { color:#333; }
  #joystick {
    margin:40px auto;
    width:200px; height:200px;
    background:#ddd; border-radius:50%;
    touch-action:none; position:relative;
  }
  #stick {
    width:80px; height:80px;
    background:#2196F3; border-radius:50%;
    position:absolute; left:60px; top:60px;
    transition:0.05s;
  }
</style>
</head>
<body>
  <h2>ESP32 WiFi Joystick</h2>
  <div id="joystick">
    <div id="stick"></div>
  </div>
  <p id="coords">X: 0, Y: 0</p>

<script>
let joy = document.getElementById('joystick');
let stick = document.getElementById('stick');
let center = { x: 100, y: 100 };
let radius = 100;

function send(x, y) {
  fetch(`/joystick?x=${x}&y=${y}`).catch(err => {});
}

function moveStick(clientX, clientY) {
  let rect = joy.getBoundingClientRect();
  let x = clientX - rect.left;
  let y = clientY - rect.top;
  let dx = x - center.x;
  let dy = y - center.y;
  let dist = Math.sqrt(dx*dx + dy*dy);
  if (dist > radius) {
    dx *= radius / dist;
    dy *= radius / dist;
  }
  stick.style.left = (center.x + dx - 40) + 'px';
  stick.style.top = (center.y + dy - 40) + 'px';
  let normX = Math.round(dx / radius * 100);
  let normY = Math.round(-dy / radius * 100);
  document.getElementById('coords').innerText = `X: ${normX}, Y: ${normY}`;
  send(normX, normY);
}

function resetStick() {
  stick.style.left = '60px';
  stick.style.top = '60px';
  document.getElementById('coords').innerText = "X: 0, Y: 0";
  send(0, 0);
}

joy.addEventListener('touchmove', e => {
  e.preventDefault();
  let t = e.touches[0];
  moveStick(t.clientX, t.clientY);
});
joy.addEventListener('touchend', resetStick);
joy.addEventListener('mousedown', e => moveStick(e.clientX, e.clientY));
joy.addEventListener('mousemove', e => { if (e.buttons) moveStick(e.clientX, e.clientY); });
joy.addEventListener('mouseup', resetStick);
joy.addEventListener('mouseleave', resetStick);
</script>
</body>
</html>
)HTML";

void setup() {
  Serial.begin(115200);

  // Start WiFi in Access Point mode
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Define routes page
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(200, "text/html", htmlPage);
  });

  // Start server
  server.begin();
  Serial.println("Async Web Server started!");
}

void loop() {

  }
