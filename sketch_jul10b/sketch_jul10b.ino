#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;

bool alarmActive = true;  // toggle to false to simulate OFF
bool showColon = true;
unsigned long lastBlink = 0;
const long blinkInterval = 500;  // ms

const uint8_t bellBitmap[8] PROGMEM = {
  0b11111111, // ████████
  0b10000001, // █      █
  0b10000001, // █      █
  0b10000001, // █      █
  0b10000001, // █      █
  0b10000001, // █      █
  0b10000001, // █      █
  0b11111111  // ████████
};

const uint8_t hollowSquareBitmap[8] PROGMEM = {
  0b11111111, // row 0 → ████████
  0b10000001, // row 1 → █      █
  0b10000001, // row 2 → █      █
  0b10000001, // row 3 → █      █
  0b10011001, // row 4 → █      █
  0b10011001, // row 5 → █      █
  0b10000001, // row 6 → █      █
  0b11111111  // row 7 → ████████
};



void setup() {
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
}

void loop() {
  DateTime now = rtc.now();
  display.clearDisplay();

  // --------- Top Row: AL | DAY | DATE ---------
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // display.setCursor(0, 0);
  if (alarmActive) {    
    display.drawBitmap(4, 4, hollowSquareBitmap, 8, 8, WHITE);
    // display.drawBitmap(10, 20, bellBitmap, 8, 8, SSD1306_WHITE); // Bell ON indicator
  } else {
    display.print("-");  // Bell OFF indicator
  }

  String dayOfWeek = dayToString(now.dayOfTheWeek());
  int16_t x1, y1;
  uint16_t w_day, h_day;
  display.getTextBounds(dayOfWeek, 0, 0, &x1, &y1, &w_day, &h_day);
  display.setCursor((SCREEN_WIDTH - w_day) / 2, 0);
  display.print(dayOfWeek);

  String dayOfMonth = String(now.day());
  int16_t x2, y2;
  uint16_t w_date, h_date;
  display.getTextBounds(dayOfMonth, 0, 0, &x2, &y2, &w_date, &h_date);
  display.setCursor(SCREEN_WIDTH - w_date, 0);
  display.print(dayOfMonth);

  // --------- Blinking colon ----------
  if (millis() - lastBlink >= blinkInterval) {
    lastBlink = millis();
    showColon = !showColon;
  }

  // --------- Bottom: HH:MMss as one line ----------
  int displayHour = now.twelveHour();
  if (displayHour == 0) displayHour = 12;

  char timeString[6];  // "HH:MM"
  sprintf(timeString, "%02d%c%02d", displayHour, showColon ? ':' : ' ', now.minute());

  char secString[3];  // "ss"
  sprintf(secString, "%02d", now.second());

  // Measure widths to align as a block
  display.setTextSize(3);
  display.getTextBounds(timeString, 0, 0, &x1, &y1, &w_day, &h_day);

  display.setTextSize(2);
  display.getTextBounds(secString, 0, 0, &x2, &y2, &w_date, &h_date);

  uint16_t totalWidth = w_day + w_date + 4;  // small gap between
  int16_t startX = (SCREEN_WIDTH - totalWidth) / 2;
  int16_t yPos = SCREEN_HEIGHT - h_day - 2;

  // Draw HH:MM (large)
  display.setTextSize(3);
  display.setCursor(startX, yPos);
  display.print(timeString);

  // Draw ss (smaller, inline)
  display.setTextSize(2);
  display.setCursor(startX + w_day + 4, yPos + 4);  // slight vertical adjust
  display.print(secString);

  display.display();
  delay(100);  // smooth refresh
}

String dayToString(uint8_t day) {
  switch (day) {
    case 0: return "SUN";
    case 1: return "MON";
    case 2: return "TUE";
    case 3: return "WED";
    case 4: return "THU";
    case 5: return "FRI";
    case 6: return "SAT";
    default: return "UNK";
  }
}
