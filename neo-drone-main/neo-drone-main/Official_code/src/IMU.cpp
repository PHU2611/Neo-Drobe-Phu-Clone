#include "sensors/IMU.h"
#include "config/BoardConfig.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

float x_raw = 0.0f;
float y_raw = 0.0f;
float z_raw = 0.0f;

int16_t gyro_offset_x = -1;
int16_t gyro_offset_y = -1;
int16_t gyro_offset_z = 0;

// BNO055 I2C address is usually 0x28. Some boards use 0x29 (ADR pin high).
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

static void printVec(const char* name, const imu::Vector<3>& v, const char* unit) {
  Serial.printf("%s: %.3f, %.3f, %.3f %s  ", name, v.x(), v.y(), v.z(), unit);
}

static void printQuat(const imu::Quaternion& q) {
  Serial.printf("Quat(w,x,y,z): %.4f, %.4f, %.4f, %.4f  ", q.w(), q.x(), q.y(), q.z());
}

static void printCalStatus() {
  uint8_t sys, gyro, accel, mag;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  // Each is 0..3
  Serial.printf("CAL(sys,g,a,m)=%u,%u,%u,%u  ", sys, gyro, accel, mag);
}

void setupIMU() {
  Serial.begin(115200);
  delay(200);

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000); // BNO055 supports fast mode typically

  Serial.println("\n--- BNO055 9-DOF On-board Fusion Test (ESP32 + PlatformIO) ---");

  if (!bno.begin()) {
    Serial.println("ERROR: BNO055 not detected. Check wiring / address (0x28 vs 0x29).");
    while (true) delay(100);
  }

  delay(100);

  // Use NDOF mode = full sensor fusion (gyro+accel+mag) on-board
  bno.setExtCrystalUse(true); // if your breakout has a 32.768kHz crystal (most do)

  // Use IMU mode (gyro + accel only)
  bno.setMode(OPERATION_MODE_IMUPLUS);
  Serial.println("Keep sensor completely still for gyro calibration...");
  
  Serial.println("BNO055 found. Using NDOF fusion mode.");
  Serial.println("Output: Euler(deg), Quaternion, Acc(m/s^2), Gyro(rad/s), Mag(uT), LinAcc(m/s^2), Gravity(m/s^2), Temp(C), Calib");
  Serial.println("Tip: move device in a figure-8 for magnetometer calibration.");
  printCalStatus();
  delay(1000);
}


void take_out_angles() {
  // --- Fused orientation (on-board) ---
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);           // degrees

  int8_t tempC = bno.getTemp();

  // Print a compact line
  x_raw = euler.z();
  y_raw = euler.x();
  z_raw = euler.y();


  Serial.println();
  delay(100); // 10 Hz
}