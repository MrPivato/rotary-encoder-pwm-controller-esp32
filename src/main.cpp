#include <Arduino.h>

#include "pwm_main.h"
#include "dp_main.h"
#include "re_main.h"

void go_to_sleep(int duty_cycle);

void setup()
{
  Serial.begin(115200);

  pwm_init();
  dp_init_display_i2c();
  re_init();
}

void loop()
{
  // rotary encoder knob turn --> re_handle_turn();
  // and
  // rotary encoder button press --> re_handle_press();
  // are triggered with interrupts, no need to call them

  pwm_Status.duty_cycle = re_Status.counter;

  pwm_set_duty_cycle(pwm_Status.duty_cycle);

  if (re_Status.button_pressed)
  {
    pwm_hold_state_during_sleep(pwm_Status.duty_cycle);
    go_to_sleep(10);
  }

  dp_show_current_pwm_info(pwm_Properties.FREQUENCY, pwm_duty_cycle_to_percent(pwm_Status.duty_cycle));
}

void go_to_sleep(int time_to_sleep_in_seconds)
{

  esp_sleep_enable_timer_wakeup(time_to_sleep_in_seconds * 1000000);
  esp_deep_sleep_start();
}