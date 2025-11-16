#include "LEDAnimator.h"

LEDAnimator::LEDAnimator(Adafruit_NeoPixel &strip)
: leds(strip)
{
    lastLED = leds.numPixels() - 1;
}

void LEDAnimator::begin() {
    leds.begin();
    leds.show(); // all OFF
}

void LEDAnimator::setMode(Mode m) {
    currentMode = m;
}

void LEDAnimator::setRange(uint8_t first, uint8_t last) {
    firstLED = first;
    lastLED  = (last < leds.numPixels()) ? last : leds.numPixels() - 1;
}

void LEDAnimator::update() {
    switch (currentMode) {
        case IDLE_BREATH: applyIdleBreath(); break;
        case OFF:
        default: break;
    }

    leds.show();
}

// ----------------------------------------------------
// IDLE: cyan breathing animation
// ----------------------------------------------------
void LEDAnimator::applyIdleBreath() {
    // Adjust brightness
    breatheBrightness += breatheStep;

    // Clamp and reverse *before* displaying
    if (breatheBrightness >= 255) {
        breatheBrightness = 255;
        breatheStep = -breatheStep;
    }
    else if (breatheBrightness <= 0) {
        breatheBrightness = 0;
        breatheStep = -breatheStep;
    }

    uint32_t cyan = leds.Color(0, breatheBrightness, breatheBrightness);

    setStripColor(cyan);
}


void LEDAnimator::setStripColor(uint32_t color) {
    for (uint8_t i = firstLED; i <= lastLED; i++)
        leds.setPixelColor(i, color);
}
