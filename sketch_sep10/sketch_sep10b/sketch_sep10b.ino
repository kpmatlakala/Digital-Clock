#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RTC
RTC_DS3231 rtc;

// Bicolor LED pins (Common Anode: LOW = ON)
#define RED_PIN 9
#define BLUE_PIN 10

// Colon blink
bool showColon = true;
unsigned long lastBlink = 0;
const long blinkInterval = 500;  // ms

// Slide control
int currentSlide = 1;
unsigned long slideStart = 0;
const unsigned long dateDisplayDuration = 15000;  // 15 seconds

// LED breathing animation modes
enum LedMode {
  MODE_BLUE_IDLE,
  MODE_PURPLE_TRANSITION
};

LedMode currentLedMode = MODE_BLUE_IDLE;
unsigned long lastModeChange = 0;
const unsigned long blueCycleDuration = 4000;    // 4 seconds per full cycle
const unsigned long purpleCycleDuration = 3000;  // 3 seconds per full cycle

// Guard to prevent repeat triggering on same minute
bool hasShownDateThisQuarter = false;

// -------------------- Utility Functions --------------------
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

String monthToString(uint8_t month) {
  switch (month) {
    case 1: return "JAN";
    case 2: return "FEB";
    case 3: return "MAR";
    case 4: return "APR";
    case 5: return "MAY";
    case 6: return "JUN";
    case 7: return "JUL";
    case 8: return "AUG";
    case 9: return "SEP";
    case 10: return "OCT";
    case 11: return "NOV";
    case 12: return "DEC";
    default: return "UNK";
  }
}

// -------------------- LED Breathing --------------------
void updateLED() {
  unsigned long nowMillis = millis();
  unsigned long cycleDuration = (currentLedMode == MODE_BLUE_IDLE) ? blueCycleDuration : purpleCycleDuration;

  float progress = (nowMillis % cycleDuration) / (float)cycleDuration;
  float brightness = (progress < 0.5) ? (progress * 2.0) : (2.0 - progress * 2.0);
  int pwmValue = 255 - (int)(brightness * 255);  // Inverted for common anode

  if (currentLedMode == MODE_BLUE_IDLE) {
    analogWrite(RED_PIN, 255);        // OFF
    analogWrite(BLUE_PIN, pwmValue);  // Blue breathing
  } else if (currentLedMode == MODE_PURPLE_TRANSITION) {
    analogWrite(RED_PIN, pwmValue);  // Purple = Red + Blue
    analogWrite(BLUE_PIN, pwmValue);
  }
}

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 255);   // OFF
  analogWrite(BLUE_PIN, 255);  // OFF

  slideStart = millis();
}

// -------------------- Main Loop --------------------
void loop() {
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  int currentMinute = now.minute();
  int currentSecond = now.second();
  bool isQuarter = (currentMinute % 15 == 0);

  // Blink colon every 500ms
  if (currentMillis - lastBlink >= blinkInterval) {
    lastBlink = currentMillis;
    showColon = !showColon;
  }

  // Start Date Slide if on a quarter
  if (isQuarter && !hasShownDateThisQuarter && currentSlide == 1 && currentSecond < 2) {
    currentSlide = 2;
    slideStart = currentMillis;
    currentLedMode = MODE_PURPLE_TRANSITION;
    lastModeChange = currentMillis;
    hasShownDateThisQuarter = true;
  }

  // Reset guard flag once we're past the quarter
  if (!isQuarter) {
    hasShownDateThisQuarter = false;
  }

  // Return to Time Slide after 15 seconds
  if (currentSlide == 2 && currentMillis - slideStart >= dateDisplayDuration) {
    currentSlide = 1;
    slideStart = currentMillis;
    currentLedMode = MODE_BLUE_IDLE;
    lastModeChange = currentMillis;
  }

  // Update LED animation
  updateLED();

  // Clear display
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (currentSlide == 1) {
    // --- SLIDE 1: TIME VIEW ---
    display.setTextSize(1);
    int bars = map(now.second(), 0, 59, 0, 12);
    display.setCursor(0, 0);
    for (int i = 0; i < bars; i++) display.print("|");
    display.setCursor(0, 8);
    display.print("[");
    display.setCursor(bars * 6, 8);
    display.print("]");

    // Date (top-right)
    display.setTextSize(2);
    String shortDate = dayToString(now.dayOfTheWeek()).substring(0, 2) + " " + String(now.day());
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(shortDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w, 0);
    display.print(shortDate);

    // Time (centered)
    char timeBuffer[6];
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d%c%02d", now.hour(), showColon ? ':' : ' ', now.minute());
    char secBuffer[3];
    snprintf(secBuffer, sizeof(secBuffer), "%02d", now.second());

    display.setTextSize(3);
    display.getTextBounds(timeBuffer, 0, 0, &x1, &y1, &w, &h);
    int xMain = (SCREEN_WIDTH - (w + 22)) / 2;
    int yMain = SCREEN_HEIGHT - h - 4;
    display.setCursor(xMain, yMain);
    display.print(timeBuffer);

    display.setTextSize(2);
    int xSec = xMain + w + 3;
    int ySec = yMain + 10;
    display.setCursor(xSec, ySec);
    display.print(secBuffer);

  } else if (currentSlide == 2) {
    // --- SLIDE 2: DATE VIEW ---
    display.setTextSize(3);
    String topDate = dayToString(now.dayOfTheWeek()) + " " + String(now.day());
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(topDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 12);
    display.print(topDate);

    display.setTextSize(2);
    String bottomDate = monthToString(now.month()) + " " + String(now.year());
    display.getTextBounds(bottomDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 42);
    display.print(bottomDate);

    display.drawLine(10, 8, SCREEN_WIDTH - 10, 8, SSD1306_WHITE);
    display.drawLine(10, SCREEN_HEIGHT - 8, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 8, SSD1306_WHITE);
  }

  display.display();
  delay(50);  // Smooth rendering
}
