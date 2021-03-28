#pragma once

#include <Arduino.h>
#include "esp32/ulp.h"
#include "driver/rtc_io.h"

struct pwm_Properties
{
    const uint8_t PIN = 15;
    const int FREQUENCY = 1000;
    const uint8_t CHANNEL = RTCIO_GPIO15_CHANNEL;
    const uint8_t RESOLUTION = 8;
    const uint8_t MIN_RESOLUTION_VALUE = 0;
    const uint8_t MAX_RESOLUTION_VALUE = 255;
};

struct pwm_Status {
    uint8_t duty_cycle;
};

extern struct pwm_Properties pwm_Properties;
extern struct pwm_Status pwm_Status;

void pwm_init();
void pwm_set_duty_cycle(int duty_cycle);
int pwm_duty_cycle_to_percent(int duty_cycle);
void pwm_hold_state_during_sleep(int duty_cycle);