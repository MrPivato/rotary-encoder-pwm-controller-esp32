#pragma once

#include <Arduino.h>

struct re_Properties
{
    const uint8_t CLK_PIN = 18;
    const uint8_t DT_PIN = 19;
    const uint8_t BTN_PIN = 5;
};

struct re_Status
{
    int counter = 0;
    int current_state_clk;
    int previous_state_clk;
    bool direction; // 0 == CCW and 1 == CW
    bool button_pressed = false;
};

extern struct re_Properties re_Properties;
extern volatile struct re_Status re_Status;

void re_init();
void re_attach_interrupts();
void re_detach_interrupts();
void re_handle_turn();
void re_handle_press();
void re_reset_encoder();
