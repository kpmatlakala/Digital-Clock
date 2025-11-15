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

void ClockDisplay::showTime(DateTime now, bool showColon) {
    display.clearDisplay();

    // --- Top-right compact day/date ---
    display.setTextSize(2);
    String dayStr = dayToString(now.dayOfTheWeek()).substring(0,2);
    dayStr.setCharAt(0, toupper(dayStr.charAt(0)));
    dayStr.setCharAt(1, tolower(dayStr.charAt(1)));
    String compact = dayStr + String(now.day());

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(compact, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(128 - w - 2, 2);
    display.print(compact);

    // --- Bottom main time HH:MM ---
    display.setTextSize(3);
    char digits[3];

    snprintf(digits, sizeof(digits), "%02d", now.hour());
    int16_t cursorX = 8;
    int16_t cursorY = 45;
    display.setCursor(cursorX, cursorY);
    display.print(digits);

    // Colon blink
    if (showColon) display.print(':');
    else display.print(' ');

    cursorX = display.getCursorX(); // update after colon

    snprintf(digits, sizeof(digits), "%02d", now.minute());
    display.print(digits);

    // --- Seconds on same line, slightly smaller ---
    display.setTextSize(2);
    char sec[3];
    snprintf(sec, sizeof(sec), "%02d", now.second());

    // int16_t tw, th;
    display.getTextBounds(String(sec), 0, 0, &x1, &y1, &w, &h);

    display.setCursor(cursorX + w + 16, cursorY + 6); // align nicely with HH:MM
    display.print(sec);

    display.display();
}
