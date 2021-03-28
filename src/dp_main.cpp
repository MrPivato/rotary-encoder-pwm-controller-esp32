#include "dp_main.h"

struct dp_Properties dp_Properties;

Adafruit_SSD1306 dp_Screen(dp_Properties.SCREEN_WIDTH, dp_Properties.SCREEN_HEIGHT, &Wire, dp_Properties.OLED_RESET);
TwoWire dp_DISPLAY_I2C = TwoWire(1);

void dp_init_display_i2c()
{
    dp_DISPLAY_I2C.begin(dp_Properties.SDA_PIN, dp_Properties.SCL_PIN, 100000);

    if (!dp_Screen.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // loop forever, for now..
    }

    dp_Screen.clearDisplay();
    dp_Screen.display();
}

void dp_show_current_pwm_info(int frequency, int duty_cycle)
{
    char buffer[20];

    dp_Screen.clearDisplay();
    dp_Screen.setTextSize(1);
    dp_Screen.setTextColor(WHITE);
    dp_Screen.setCursor(0, 0);
    dp_Screen.println("Frequency:");
    dp_Screen.setTextSize(2);
    dp_Screen.setCursor(16, 9);
    snprintf(buffer, sizeof(buffer), "%dHz", frequency);
    dp_Screen.println(buffer);

    dp_Screen.setTextSize(1);
    dp_Screen.setTextColor(WHITE);
    dp_Screen.setCursor(0, 30);
    dp_Screen.println("Duty Cycle:");
    dp_Screen.setTextSize(3);
    dp_Screen.setCursor(16, 42);
    snprintf(buffer, sizeof(buffer), "%d%%", duty_cycle);
    dp_Screen.println(buffer);

    dp_Screen.display();
}