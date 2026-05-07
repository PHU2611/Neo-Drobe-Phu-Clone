#include "HtmlPages.h"

const char HTML_CONTROL_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body {
    text-align: center;
    font-family: sans-serif;
    background: #f8fafc;
  }
  h2 { color: #333; }

  #container {
    display: flex;
    justify-content: space-around;
    align-items: center;
    flex-wrap: wrap;
    margin-top: 20px;
  }

  /* === JOYSTICK === */
  #joystick {
    width: 150px;
    height: 150px;
    background: #ddd;
    border-radius: 50%;
    position: relative;
    touch-action: none;
  }
  #stick {
    width: 50px;
    height: 50px;
    background: #444;
    border-radius: 50%;
    position: absolute;
    top: 50px;
    left: 50px;
  }

  /* === D-PAD === */
  #dpad {
    display: grid;
    grid-template-columns: 60px 60px 60px;
    grid-template-rows: 60px 60px 60px;
    justify-content: center;
    align-items: center;
    gap: 5px;
  }

  .button {
    width: 60px;
    height: 60px;
    border: none;
    border-radius: 50%;
    color: white;
    font-size: 24px;
  }
  #up { grid-column: 2; grid-row: 1; background: #27ae60; }
  #left { grid-column: 1; grid-row: 2; background: #2980b9; }
  #right { grid-column: 3; grid-row: 2; background: #f1c40f; }
  #down { grid-column: 2; grid-row: 3; background: #c0392b; }

  /* === VALUE DISPLAY === */
  #values {
    margin-top: 20px;
    font-size: 18px;
    color: #333;
    background: #e0e7ff;
    border-radius: 10px;
    display: inline-block;
    padding: 10px 20px;
  }
</style>
</head>
<body>
  <h2>ESP32 Drone Controller</h2>

  <div id="container">
    <!-- Left Joystick -->
    <div id="joystick">
      <div id="stick"></div>
    </div>

    <!-- Right D-Pad -->
    <div id="dpad">
      <button class="button" id="up">↑</button>
      <button class="button" id="left">←</button>
      <button class="button" id="right">→</button>
      <button class="button" id="down">↓</button>
    </div>
  </div>

  <!-- Display current values -->
  <div id="values">
    RollX: 0 | RollY: 0 | Throttle: 0 | Yaw: 0
  </div>

  <script>
    const joy = document.getElementById('joystick');
    const stick = document.getElementById('stick');
    const valBox = document.getElementById('values');

    let rollX = 0, rollY = 0, throttle = 0, yaw = 0;

    // --- Joystick movement ---
    joy.addEventListener('touchmove', e => {
      e.preventDefault();
      const touch = e.touches[0];
      const rect = joy.getBoundingClientRect();
      let x = touch.clientX - rect.left - 25;
      let y = touch.clientY - rect.top - 25;
      const dx = x - 50, dy = y - 50;
      const dist = Math.min(Math.sqrt(dx*dx + dy*dy), 50);
      const angle = Math.atan2(dy, dx);
      stick.style.left = 50 + dist * Math.cos(angle) + 'px';
      stick.style.top = 50 + dist * Math.sin(angle) + 'px';

      rollX = Math.round((dx / 50) * 100);
      rollY = Math.round((-dy / 50) * 100);
    });

    joy.addEventListener('touchend', e => {
      stick.style.left = '50px';
      stick.style.top = '50px';
      rollX = 0; rollY = 0;
    });

    // --- Button controls ---
    document.getElementById('up').onclick = () => throttle += 10;
    document.getElementById('down').onclick = () => throttle -= 10;
    document.getElementById('left').onclick = () => yaw -= 10;
    document.getElementById('right').onclick = () => yaw += 10;

    // --- Update display ---
    function updateDisplay() {
      valBox.innerText = `RollX: ${rollX} | RollY: ${rollY} | Throttle: ${throttle} | Yaw: ${yaw}`;
    }

    // --- Send data every 500 ms ---
    setInterval(() => {
      updateDisplay();
      fetch(`/update?rollX=${rollX}&rollY=${rollY}&throttle=${throttle}&yaw=${yaw}`);
    }, 500);
  </script>
</body>
</html>
)rawliteral";