#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Scheduler.h>

#include "handler.h"

auto handler = due::Voltmeter(A0);
// auto handler = due::BlinkLed();
auto handlerRx = due::BlinkLed(PIN_LED_RXL);
auto handlerTx = due::BlinkLed(PIN_LED_TXL);
// auto handler = due::LedOscillator();
void loop2();
void loop3();
void loop4();

// LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  Serial.println("In the setup");
  handler.setup();
  handlerRx.setup();
  handlerTx.setup();

  // lcd.begin(20, 4);
  // lcd.cursor();
  // lcd.blink();
  // Scheduler.startLoop(handlerRx.execute);
  // Scheduler.startLoop(loop2);
}

int char_offset = 0;

void loop2() {
  Serial.println("Writing to lcd");
  // lcd.print("Hare Krishna, Rohan!");
  // lcd.print('a' + char_offset);
  char_offset = (char_offset + 1) % 26;
  delay(3000);

  // lcd.clear();
  delay(1000);
}

void loop1() { handlerRx.execute(); }

void loop() {
  // delay(10000);
  handler.execute();
}

void loop3() { handlerTx.execute(); }
