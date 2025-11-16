#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LEDAnimator {
public:
    enum Mode {
        IDLE_BREATH,
        OFF
    };

    LEDAnimator(Adafruit_NeoPixel &strip);

    void begin();
    void setMode(Mode m);
    void update();              // call every loop()
    void setRange(uint8_t first, uint8_t last);

private:
    Adafruit_NeoPixel &leds;
    Mode currentMode = IDLE_BREATH;

    // breathing state
    int breatheBrightness = 0;
    int breatheStep = 4;

    // LED range control
    uint8_t firstLED = 0;
    uint8_t lastLED;

    // internal effect handlers
    void applyIdleBreath();
    void setStripColor(uint32_t color);
};
