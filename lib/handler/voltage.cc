#include <Arduino.h>

#include "constants.h"
#include "handler.h"

namespace {
constexpr uint32_t kResistorCount = 12;

constexpr uint32_t kR1s[kResistorCount] = {
    9904,
    9864,
    9858,
    9853,
    9857,
    9879,
    9854,
    9878,
    9845,
    9881,
    9840,
    9863,
};

constexpr uint32_t kR2s[kResistorCount] = {
    9913,
    9837,
    9856,
    9852,
    9855,
    9866,
    9885,
    9873,
    9907,
    9855,
    9901,
    9850,
};

constexpr double kVoltageAdj = 0.019;

}  // namespace

namespace due {
Voltmeter::Voltmeter(uint32_t analog_pin) : analog_pin_(analog_pin) {}

void Voltmeter::setup() { pinMode(analog_pin_, INPUT); }

void Voltmeter::execute() {
  Serial.print("Input voltages: ");
  double values[kResistorCount] = {0};
  for (uint8_t i = 0; i < kResistorCount; i++) {
    while(!Serial.available()) {
      delay(100);
      continue;
    }
    Serial.print("From serial: ");
    auto serial = Serial.read();
    Serial.println(serial);
    i = serial - 'b';
    Serial.print("Processing pin: ");
    Serial.println(i+1);
    double analogValue = 0;
    int samples = 10;
    for (int j = 0; j < samples; j++) {
      auto v = analogRead(A0 + i);
      // Serial.print("Analog value: ");
      // Serial.println(v);
      analogValue += v;
      delay(10);
    }

    double known_value = 4.022;

    analogValue = analogValue / samples;
    auto voltageAtDivider = kAnalogRefDue * analogValue / 1023;
    auto r1 = kR1s[i];
    auto r2 = kR2s[i];
    auto inputVoltage = (voltageAtDivider * (r1 + r2) / r2) + kVoltageAdj;
    Serial.print(i + 1);
    Serial.print("->");
    Serial.print(inputVoltage, 3);
    Serial.print(", mult: ");
    values[i] = known_value / voltageAtDivider;
    Serial.print(values[i]);
    Serial.print(", ");
    Serial.println();
  }
  Serial.print("Values: {");
  for (uint8_t i = 0; i < kResistorCount; i++) {
    Serial.print(values[i]);
    Serial.print(", ");
  }
  Serial.println("}");
  delay(5000);
}

VoltageOscillator::VoltageOscillator(uint32_t out_pin, uint32_t delay_ms)
    : out_pin_(out_pin),
      delay_ms_(delay_ms),
      pwm_value_(0),
      oscillation_amount_(5) {}

void VoltageOscillator::setup() { pinMode(out_pin_, OUTPUT); }

void VoltageOscillator::execute() {
  analogWrite(out_pin_, pwm_value_);
  Serial.print("Voltage percent: ");
  Serial.println(pwm_value_ * 100 / PWM_MAX_DUTY_CYCLE);

  pwm_value_ = pwm_value_ + oscillation_amount_;

  // Reverse the direction of the oscillation at the ends of the fade:
  if (pwm_value_ <= 0 || pwm_value_ >= PWM_MAX_DUTY_CYCLE) {
    oscillation_amount_ = -oscillation_amount_;
  }
  // Wait to see the oscillation effect.
  delay(delay_ms_);
}

VoltageDemo::VoltageDemo() : voltage_oscillator_(D8, 3000) {}

void VoltageDemo::setup() {
  uint32_t pins[] = {D3, D4, D5, D6, D7, D8};
  for (auto pin : pins) {
    pinMode(pin, OUTPUT);
  }

  analogWrite(D3, 0);                           // 0%
  analogWrite(D4, PWM_MAX_DUTY_CYCLE / 4);      // 25%
  analogWrite(D5, PWM_MAX_DUTY_CYCLE / 2);      // 50%
  analogWrite(D6, PWM_MAX_DUTY_CYCLE * 3 / 4);  // 75%
  analogWrite(D7, PWM_MAX_DUTY_CYCLE);          // 100%
}
void VoltageDemo::execute() { voltage_oscillator_.execute(); }

}  // namespace due
