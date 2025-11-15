#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#include "ClockDisplay.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;

ClockDisplay clockDisp(display);


// colon blink
bool showColon = true;
unsigned long lastBlink = 0;
const unsigned long blinkInterval = 500;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("OLED init failed");
      while (1);
  }

  clockDisp.begin();

  if (!rtc.begin()) {
      Serial.println("RTC not found");
      while (1);
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (rtc.lostPower()) {
      Serial.println("RTC lost power, setting to compile time");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
    DateTime now = rtc.now();

    // blink colon
    if (millis() - lastBlink >= blinkInterval) {
        lastBlink = millis();
        showColon = !showColon;
    }

    clockDisp.showTime(now, showColon);
    delay(50);
}
