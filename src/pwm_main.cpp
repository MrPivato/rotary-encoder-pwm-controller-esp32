#include "pwm_main.h"

struct pwm_Properties pwm_Properties;
struct pwm_Status pwm_Status;

void pwm_init()
{
    ledcSetup(pwm_Properties.CHANNEL, pwm_Properties.FREQUENCY, pwm_Properties.RESOLUTION);
    ledcAttachPin(pwm_Properties.PIN, pwm_Properties.CHANNEL);

    gpio_hold_dis((gpio_num_t)pwm_Properties.PIN);
}

void pwm_set_duty_cycle(int duty_cycle)
{
    ledcWrite(pwm_Properties.CHANNEL, duty_cycle);
}

int pwm_duty_cycle_to_percent(int duty_cycle)
{
    return map(duty_cycle, pwm_Properties.MIN_RESOLUTION_VALUE, pwm_Properties.MAX_RESOLUTION_VALUE, 0, 100);
}

void pwm_hold_state_during_sleep(int duty_cycle)
{
    int ulp_clock_to_khz_scalar = 8500;
    int ulp_clock_error_correction_scalar = 1.07;

    // slow memory init
    memset(RTC_SLOW_MEM, 0, 8192);

    // pwm config
    const gpio_num_t pwm_pin = gpio_num_t(pwm_Properties.PIN);
    const uint32_t pwm_bit = pwm_Properties.CHANNEL + 14;

    // pwm on GPIO init
    rtc_gpio_init(pwm_pin);
    rtc_gpio_set_direction(pwm_pin, RTC_GPIO_MODE_OUTPUT_ONLY);
    rtc_gpio_set_level(pwm_pin, 0);

    // delay time calculation
    uint32_t on_time = map(duty_cycle, pwm_Properties.MIN_RESOLUTION_VALUE, pwm_Properties.MAX_RESOLUTION_VALUE, 0, ulp_clock_to_khz_scalar) / ulp_clock_error_correction_scalar;
    uint32_t off_time = ulp_clock_to_khz_scalar - on_time;

    const ulp_insn_t ulp_program[] = {
        M_LABEL(1),
        I_WR_REG(RTC_GPIO_OUT_REG, pwm_bit, pwm_bit, 1), // turn on
        I_DELAY(on_time),                                // wait
        I_WR_REG(RTC_GPIO_OUT_REG, pwm_bit, pwm_bit, 0), // turn off
        I_DELAY(off_time),                               // wait
        M_BX(1),
    };

    // run ULP program
    size_t size = sizeof(ulp_program) / sizeof(ulp_insn_t);
    ulp_process_macros_and_load(0, ulp_program, &size);
    ulp_run(0);

    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
}