#include "handler.h"

#include <Arduino.h>

namespace due {

BlinkLed::BlinkLed(uint32_t led_pin) : led_pin_(led_pin) {}

void BlinkLed::setup() {
  pinMode(led_pin_, OUTPUT);
  digitalWrite(led_pin_, LOW);
}

void BlinkLed::execute() {
  digitalWrite(led_pin_, HIGH);
  delay(1000);
  digitalWrite(led_pin_, LOW);
  delay(1000);
}

LedOscillator::LedOscillator(uint32_t led_pin) : voltage_oscillator_(led_pin,
                                                                     30) {}

void LedOscillator::setup() {
  voltage_oscillator_.setup();
}

void LedOscillator::execute() {
  voltage_oscillator_.execute();
}

} // namespace due


