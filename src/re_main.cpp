#include "re_main.h"

struct re_Properties re_Properties;
volatile struct re_Status re_Status;

void re_init()
{
    pinMode(re_Properties.CLK_PIN, INPUT);
    pinMode(re_Properties.DT_PIN, INPUT);
    pinMode(re_Properties.BTN_PIN, INPUT_PULLUP);

    re_attach_interrupts();

    re_Status.previous_state_clk = digitalRead(re_Properties.CLK_PIN);
}

void re_attach_interrupts()
{
    attachInterrupt(re_Properties.BTN_PIN, re_handle_press, FALLING);
    attachInterrupt(re_Properties.CLK_PIN, re_handle_turn, CHANGE);
    attachInterrupt(re_Properties.DT_PIN, re_handle_turn, CHANGE);
}

void re_detach_interrupts()
{
    detachInterrupt(re_Properties.CLK_PIN);
    detachInterrupt(re_Properties.DT_PIN);
    detachInterrupt(re_Properties.BTN_PIN);
}

void re_handle_turn()
{
    re_Status.current_state_clk = digitalRead(re_Properties.CLK_PIN);

    re_detach_interrupts();

    // If the previous and the current state of the re_Properties.CLK_PIN are different then a pulse has occured
    if (re_Status.current_state_clk != re_Status.previous_state_clk)
    {
        if (digitalRead(re_Properties.DT_PIN) != re_Status.current_state_clk)
        {
            // the encoder is rotating clockwise
            re_Status.counter++;
            re_Status.direction = 1;
        }
        else
        {
            // Encoder is rotating counterclockwise
            re_Status.counter--;
            re_Status.direction = 0;
        }
    }

    // Update re_Status.previous_state_clk with the current state
    re_Status.previous_state_clk = re_Status.current_state_clk;

    (re_Status.counter > 255) ? re_Status.counter = 255 : re_Status.counter;
    (re_Status.counter < 0) ? re_Status.counter = 0 : re_Status.counter;

    re_attach_interrupts();
}

void re_handle_press()
{
    re_detach_interrupts();
    re_Status.button_pressed = true;
    re_attach_interrupts();
}

void re_reset_encoder()
{
    Serial.println("implement after some time of usage it gets set to 0");
}