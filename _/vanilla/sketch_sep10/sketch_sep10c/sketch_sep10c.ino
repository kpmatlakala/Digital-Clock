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
#define RED_PIN   9
#define BLUE_PIN  10

// Buzzer pin
#define BUZZER_PIN 6

// Colon blink
bool showColon = true;
unsigned long lastBlink = 0;
const long blinkInterval = 500; // ms

// Slide control
int currentSlide = 1;
unsigned long slideStart = 0;
const unsigned long dateDisplayDuration = 15000; // 15 seconds

// LED breathing animation modes
enum LedMode {
  MODE_BLUE_IDLE,
  MODE_PURPLE_TRANSITION,
  MODE_ALARM_ON
};

LedMode currentLedMode = MODE_BLUE_IDLE;
unsigned long lastModeChange = 0;
const unsigned long blueCycleDuration = 4000;   // 4 seconds per full cycle
const unsigned long purpleCycleDuration = 3000; // 3 seconds per full cycle

// Guard to prevent repeat triggering on same minute
bool hasShownDateThisQuarter = false;

// -------------------- Alarm Setup --------------------
// Hardcoded alarm time (24-hour format)
const int alarmHour = 12;
const int alarmMinute = 20;

enum AlarmState {
  ALARM_IDLE,
  ALARM_RINGING,
  ALARM_SNOOZED
};

AlarmState alarmState = ALARM_IDLE;

unsigned long alarmStartMillis = 0;
const unsigned long alarmDuration = 30000; // 30 seconds buzzer on
const unsigned long snoozeDuration = 5 * 60 * 1000UL; // 5 minutes snooze

unsigned long snoozeStartMillis = 0;

// -------------------- Utility Functions --------------------
String dayToString(uint8_t day) {
  switch (day) {
    case 0: return "SUN"; case 1: return "MON"; case 2: return "TUE";
    case 3: return "WED"; case 4: return "THU"; case 5: return "FRI";
    case 6: return "SAT"; default: return "UNK";
  }
}

String monthToString(uint8_t month) {
  switch (month) {
    case 1: return "JAN"; case 2: return "FEB"; case 3: return "MAR";
    case 4: return "APR"; case 5: return "MAY"; case 6: return "JUN";
    case 7: return "JUL"; case 8: return "AUG"; case 9: return "SEP";
    case 10: return "OCT"; case 11: return "NOV"; case 12: return "DEC";
    default: return "UNK";
  }
}

// -------------------- LED Breathing --------------------
void updateLED() {
  unsigned long nowMillis = millis();
  unsigned long cycleDuration;

  if (alarmState == ALARM_RINGING) {
    // Solid Red ON when alarm ringing
    analogWrite(RED_PIN, 0);  // ON (common anode)
    analogWrite(BLUE_PIN, 255); // OFF
    return;
  }

  cycleDuration = (currentLedMode == MODE_BLUE_IDLE) ? blueCycleDuration : purpleCycleDuration;

  float progress = (nowMillis % cycleDuration) / (float)cycleDuration;
  float brightness = (progress < 0.5) ? (progress * 2.0) : (2.0 - progress * 2.0);
  int pwmValue = 255 - (int)(brightness * 255); // Inverted for common anode

  if (currentLedMode == MODE_BLUE_IDLE) {
    analogWrite(RED_PIN, 255);
    analogWrite(BLUE_PIN, pwmValue);
  } else if (currentLedMode == MODE_PURPLE_TRANSITION) {
    analogWrite(RED_PIN, pwmValue);
    analogWrite(BLUE_PIN, pwmValue);
  }
}

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 255);
  analogWrite(BLUE_PIN, 255);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  slideStart = millis();
}

// -------------------- Alarm Handling --------------------
void startAlarm() {
  alarmState = ALARM_RINGING;
  alarmStartMillis = millis();
  currentLedMode = MODE_ALARM_ON;
  Serial.println("Alarm started!");
}

void stopAlarm() {
  alarmState = ALARM_SNOOZED;
  snoozeStartMillis = millis();
  currentLedMode = MODE_BLUE_IDLE;
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("Alarm snoozed!");
}

void updateAlarm(DateTime now) {
  unsigned long nowMillis = millis();

  switch (alarmState) {
    case ALARM_IDLE:
      // Check if current time matches alarm time
      if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
        startAlarm();
      }
      break;

    case ALARM_RINGING:
      // Buzzer ON during alarmDuration
      if (nowMillis - alarmStartMillis < alarmDuration) {
        digitalWrite(BUZZER_PIN, (millis() / 250) % 2);  // Beep ON/OFF every 250ms
      } else {
        // If no stop after alarmDuration, auto snooze
        stopAlarm();
      }
      break;

    case ALARM_SNOOZED:
      digitalWrite(BUZZER_PIN, LOW);
      // Wait snoozeDuration then ring again
      if (nowMillis - snoozeStartMillis >= snoozeDuration) {
        alarmState = ALARM_IDLE;  // Reset to check alarm time again
        Serial.println("Snooze period ended, ready to ring again.");
      }
      break;
  }
}

// -------------------- Main Loop --------------------
void loop() {
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  int currentMinute = now.minute();
  int currentSecond = now.second();
  bool isQuarter = (currentMinute % 15 == 0);

  // Blink colon
  if (currentMillis - lastBlink >= blinkInterval) {
    lastBlink = currentMillis;
    showColon = !showColon;
  }

  // Switch to date slide at quarter hour
  if (isQuarter && !hasShownDateThisQuarter && currentSlide == 1 && currentSecond < 2) {
    currentSlide = 2;
    slideStart = currentMillis;
    currentLedMode = MODE_PURPLE_TRANSITION;
    lastModeChange = currentMillis;
    hasShownDateThisQuarter = true;
  }

  if (!isQuarter) {
    hasShownDateThisQuarter = false;
  }

  // Return to time slide after duration
  if (currentSlide == 2 && currentMillis - slideStart >= dateDisplayDuration) {
    currentSlide = 1;
    slideStart = currentMillis;
    currentLedMode = MODE_BLUE_IDLE;
    lastModeChange = currentMillis;
  }

  updateAlarm(now);   // alarm buzzer + state update
  updateLED();        // LED breathing update

  // OLED rendering
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (currentSlide == 1) {
    // ========== SLIDE 1: TIME VIEW ==========

    // 🔹 Alarm progress bar (top left)
    int totalBars = 12;
    int filledBars = 0;

    if (alarmState == ALARM_RINGING) {
      unsigned long elapsed = currentMillis - alarmStartMillis;
      filledBars = map(elapsed, 0, alarmDuration, 0, totalBars);
      if (filledBars > totalBars) filledBars = totalBars;
    } else {
      // Show seconds progress when not alarming
      filledBars = map(now.second(), 0, 59, 0, totalBars);
    }

    int barHeight = 4;
    for (int i = 0; i < totalBars; i++) {
      int barX = i * 6;
      if (i < filledBars) {
        display.fillRect(barX, 0, 4, barHeight, SSD1306_WHITE);
      } else {
        display.drawRect(barX, 0, 4, barHeight, SSD1306_WHITE);
      }
    }

    // 🔹 Top-right compact date (e.g., Fr12)
    display.setTextSize(2);
    String shortDate = dayToString(now.dayOfTheWeek()).substring(0, 2);
    shortDate.setCharAt(0, toupper(shortDate.charAt(0)));
    shortDate.setCharAt(1, tolower(shortDate.charAt(1)));
    shortDate += String(now.day());

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(shortDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w - 1, 5);  // Just below bar
    display.print(shortDate);

    // 🔹 Main Time (centered)
    display.setTextSize(3);
    char timeBuffer[6];
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d%c%02d", now.hour(), showColon ? ':' : ' ', now.minute());

    display.getTextBounds(timeBuffer, 0, 0, &x1, &y1, &w, &h);
    int xMain = (SCREEN_WIDTH - (w + 22)) / 2;
    int yMain = SCREEN_HEIGHT - h - 4;
    display.setCursor(xMain, yMain);
    display.print(timeBuffer);

    // 🔹 Seconds (small)
    char secBuffer[3];
    snprintf(secBuffer, sizeof(secBuffer), "%02d", now.second());
    display.setTextSize(2);
    int xSec = xMain + w + 3;
    int ySec = yMain + 10;
    display.setCursor(xSec, ySec);
    display.print(secBuffer);

    // 🔹 Alarm text if ringing
    if (alarmState == ALARM_RINGING) {
      display.setTextSize(1);
      display.setCursor(5, 10);
      display.print(F("!!! ALARM !!!"));
    }

  } else if (currentSlide == 2) {
    // ========== SLIDE 2: DATE VIEW ==========
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
  delay(50);
}
