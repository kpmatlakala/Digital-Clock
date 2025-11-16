#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_NeoPixel.h>

#include "ClockDisplay.h"
#include "LEDAnimator.h"

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;
ClockDisplay clockDisp(display);

// NeoPixel
#define LED_PIN 6
#define NUM_LEDS 5
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
LEDAnimator ledAnim(strip);


// Colon blink
bool showColon = true;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500;


void setup() {
  Serial.begin(9600);
  Wire.begin();

  // --- OLED init ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("OLED init failed");
      while (1);
  }
  clockDisp.begin();

  // --- RTC init ---
  if (!rtc.begin()) {
      Serial.println("RTC not found");
      while (1);
  }

  if (rtc.lostPower()) {
      Serial.println("RTC lost power, setting to compile time");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- NeoPixel init ---
    ledAnim.begin();
    ledAnim.setMode(LEDAnimator::IDLE_BREATH);
}


// ---------------- Helper ----------------
void setStripColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}


void loop() {
    DateTime now = rtc.now();

    // --- Colon blink ---
    if (millis() - lastBlink >= blinkInterval) {
        lastBlink = millis();
        showColon = !showColon;
    }

    clockDisp.showTime(now, showColon);

    // --- NeoPixel breathing effect ---
    ledAnim.update();
}

