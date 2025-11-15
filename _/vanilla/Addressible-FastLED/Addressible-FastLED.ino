#include <Adafruit_NeoPixel.h>

#define LED_PIN     6
#define NUM_LEDS    5

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // All off
}

void loop() {
  solidColors();
  blinkColors();
  breathingEffect();
  colorWipe();
  rainbowCycle();
  theaterChase();
}

/* ===========================
   1. Solid Colors
   =========================== */
void solidColors() {
  uint32_t colors[] = {
    strip.Color(255, 0, 0),   // Red
    strip.Color(0, 255, 0),   // Green
    strip.Color(0, 0, 255),   // Blue
    strip.Color(255, 255, 0), // Yellow
    strip.Color(255, 0, 255), // Purple
    strip.Color(0, 255, 255), // Cyan
    strip.Color(255, 255, 255)// White
  };

  for (int c = 0; c < 7; c++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, colors[c]);
    }
    strip.show();
    delay(500);
  }
}

/* ===========================
   2. Blink Colors
   =========================== */
void blinkColors() {
  uint32_t colors[] = {
    strip.Color(255, 0, 0),
    strip.Color(0, 255, 0),
    strip.Color(0, 0, 255)
  };

  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 3; i++) {
      for (int p = 0; p < NUM_LEDS; p++) strip.setPixelColor(p, colors[c]);
      strip.show();
      delay(300);

      strip.clear();
      strip.show();
      delay(300);
    }
  }
}

/* ===========================
   3. Breathing Effect
   =========================== */
void breathingEffect() {
  for (int b = 0; b <= 255; b += 5) {
    uint32_t col = strip.Color(0, 0, b);
    for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, col);
    strip.setBrightness(b);
    strip.show();
    delay(30);
  }

  for (int b = 255; b >= 0; b -= 5) {
    uint32_t col = strip.Color(0, 0, b);
    for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, col);
    strip.setBrightness(b);
    strip.show();
    delay(30);
  }

  strip.setBrightness(255); // reset
}

/* ===========================
   4. Color Wipe
   =========================== */
void colorWipe() {
  uint32_t colors[] = {
    strip.Color(255, 0, 0),
    strip.Color(0, 255, 0),
    strip.Color(0, 0, 255)
  };

  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, colors[c]);
      strip.show();
      delay(150);
    }
    strip.clear();
    strip.show();
    delay(200);
  }
}

/* ===========================
   5. Rainbow Cycle
   =========================== */
void rainbowCycle() {
  for (int j = 0; j < 256; j += 8) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.ColorHSV(((i * 40) + j) * 256, 255, 255));
    }
    strip.show();
    delay(50);
  }
}

/* ===========================
   6. Theater Chase
   =========================== */
void theaterChase() {
  uint32_t white = strip.Color(255, 255, 255);

  for (int repeat = 0; repeat < 10; repeat++) {
    for (int offset = 0; offset < 3; offset++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        if ((i + offset) % 3 == 0)
          strip.setPixelColor(i, white);
        else
          strip.setPixelColor(i, 0);
      }
      strip.show();
      delay(200);
    }
  }
}
