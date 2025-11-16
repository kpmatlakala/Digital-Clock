#include "ClockDisplay.h"

ClockDisplay::ClockDisplay(Adafruit_SSD1306 &oled)
    : display(oled) {}

void ClockDisplay::begin() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

String ClockDisplay::dayToString(uint8_t day) {
    static const char* days[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    return days[day];
}

// ---------------- TOP-RIGHT DRAW HELPERS ----------------

void ClockDisplay::drawCompactDate(DateTime now) {
    String dayStr = dayToString(now.dayOfTheWeek()).substring(0,2);
    dayStr.setCharAt(0, toupper(dayStr.charAt(0)));
    dayStr.setCharAt(1, tolower(dayStr.charAt(1)));

    String compact = dayStr + String(now.day());

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(compact, 0, 0, &x1, &y1, &w, &h);

    display.setCursor(128 - w - 2, 2);
    display.print(compact);
}

void ClockDisplay::drawTemperature(float temp) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d`C", (int)round(temp));

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);

    display.setCursor(128 - w - 2, 2);
    display.print(buf);
}

// ---------------- MAIN TIME DRAW ----------------

void ClockDisplay::showTime(DateTime now, bool showColon, float temperatureC) {
    display.clearDisplay();

    // ---- Toggle every 15 seconds ----
    unsigned long t = millis();
    if (t - lastToggle >= 15000) {
        lastToggle = t;
        showDate = !showDate;
    }

    // ---- Draw top-right ----
    display.setTextSize(2);
    if (showDate || isnan(temperatureC))
        drawCompactDate(now);
    else
        drawTemperature(temperatureC);

    // ---- Bottom Time HH:MM ----
    display.setTextSize(3);
    char digits[3];

    snprintf(digits, sizeof(digits), "%02d", now.hour());
    int16_t cursorX = 8;
    int16_t cursorY = 45;
    display.setCursor(cursorX, cursorY);
    display.print(digits);

    // Colon blink
    display.print(showColon ? ':' : ' ');

    cursorX = display.getCursorX(); // update after colon

    snprintf(digits, sizeof(digits), "%02d", now.minute());
    display.print(digits);

    // ---- Seconds (small) ----
    display.setTextSize(2);
    char sec[3];
    snprintf(sec, sizeof(sec), "%02d", now.second());

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(String(sec), 0, 0, &x1, &y1, &w, &h);

    display.setCursor(cursorX + w + 16, cursorY + 6);
    display.print(sec);

    display.display();
}
