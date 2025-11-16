#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

class ClockDisplay {
public:
    ClockDisplay(Adafruit_SSD1306 &oled);

    void begin();
    void showTime(DateTime now, bool showColon = true, float temperatureC = NAN);

private:
    Adafruit_SSD1306 &display;

    String dayToString(uint8_t day);

    // 15-second swap timer
    unsigned long lastToggle = 0;
    bool showDate = true;

    void drawCompactDate(DateTime now);
    void drawTemperature(float temp);
};
