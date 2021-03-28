#pragma once

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

struct dp_Properties
{
    const uint8_t SDA_PIN = 21;
    const uint8_t SCL_PIN = 22;
    const uint8_t SCREEN_WIDTH = 128;
    const uint8_t SCREEN_HEIGHT = 64;
    const int8_t OLED_RESET = -1; // reset pin # (or -1 if sharing Arduino reset pin)
};

extern struct dp_Properties dp_Properties;
extern Adafruit_SSD1306 dp_Screen;
extern TwoWire dp_DISPLAY_I2C;

void dp_init_display_i2c();
void dp_show_current_pwm_info(int frequency, int duty_cycle);