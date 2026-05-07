#include <Arduino.h>

#include "control/PID.h"

#include "state/SharedState.h"
#include "sensors/IMU.h"

float rollPid = 0.0f;
float pitchPid = 0.0f;

namespace {
  // PID gains (start low)
  float Kp_roll = 1.0f, Ki_roll = 0.0f, Kd_roll = 0.0f;
  float Kp_pitch = 1.0f, Ki_pitch = 0.0f, Kd_pitch = 0.0f;

  uint32_t lastUs = 0;

  float integralR = 0.0f, integralP = 0.0f, integralY = 0.0f;
  float prevErrorR = 0.0f, prevErrorP = 0.0f, prevErrorY = 0.0f;
}

void pid_tuning() {
  take_out_angles(); // updates x_raw/y_raw

  const float gyroRoll = x_raw;
  const float gyroPitch = z_raw;

  const uint32_t nowUs = micros();
  float dt = (lastUs == 0) ? 0.01f : (nowUs - lastUs) * 1e-6f;
  lastUs = nowUs;
  if (dt <= 0.0f) dt = 0.01f;

  const float setpointR = static_cast<float>(rollX);
  const float setpointP = static_cast<float>(rollY);

  const float errorR = setpointR - gyroRoll;
  const float errorP = setpointP - gyroPitch;

  integralR += errorR * dt;
  integralP += errorP * dt;

  const float derivativeR = (errorR - prevErrorR) / dt;
  const float derivativeP = (errorP - prevErrorP) / dt;

  prevErrorR = errorR;
  prevErrorP = errorP;

  rollPid = (Kp_roll * errorR) + (Ki_roll * integralR) + (Kd_roll * derivativeR);
  pitchPid = (Kp_pitch * errorP) + (Ki_pitch * integralP) + (Kd_pitch * derivativeP);
}