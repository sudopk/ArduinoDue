#ifndef HANDLER_H_
#define HANDLER_H_

#include <Arduino.h>

#include <cstdint>
namespace due {

class Handler {
 public:
  virtual void setup() = 0;
  virtual void execute() = 0;
};

class BlinkLed : public Handler {
 public:
  explicit BlinkLed(uint32_t led_pin = LED_BUILTIN);

  void setup() override;
  void execute() override;

 private:
  const uint32_t led_pin_;
};

class Voltmeter : public Handler {
 public:
  explicit Voltmeter(uint32_t analog_pin);

  void setup() override;
  void execute() override;

 private:
  const uint32_t analog_pin_;
};

class VoltageOscillator : public Handler {
 public:
  VoltageOscillator(uint32_t out_pin, uint32_t delay_ms);

  void setup() override;
  void execute() override;

 private:
  const uint32_t out_pin_;
  const uint32_t delay_ms_;

  int32_t pwm_value_;
  int32_t oscillation_amount_;
};

class LedOscillator : public Handler {
 public:
  explicit LedOscillator(uint32_t led_pin = LED_BUILTIN);

  void setup() override;
  void execute() override;

 private:
  VoltageOscillator voltage_oscillator_;
};

class VoltageDemo : public Handler {
 public:
  VoltageDemo();

  void setup() override;
  void execute() override;

 private:
  VoltageOscillator voltage_oscillator_;
};

} // namespace due

#endif // HANDLER_H_
