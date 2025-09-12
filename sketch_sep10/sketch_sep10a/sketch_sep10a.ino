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

// Bicolor LED pins (Red-Blue RB4) - using analog pins as digital outputs
#define RED_PIN   A0   // A0 - Red pin
#define BLUE_PIN  A1   // A1 - Blue pin
// Common cathode connects to GND

// Colon blink
bool showColon = true;
unsigned long lastBlink = 0;
const long blinkInterval = 500; // ms

// Slide control
int currentSlide = 1;
unsigned long slideStart = 0;
const unsigned long slide1Duration = 20000; // 20s
const unsigned long slide2Duration = 5000;  // 5s

// LED animation for testing (infinite blinking)
unsigned long lastLEDUpdate = 0;
const long ledAnimationInterval = 1000; // 1 second
bool ledOn = false; // blink on/off state

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

// Bicolor LED functions (Common Anode - inverted logic)
void setColor(int r, int b) {
  // For common anode: LOW = color ON, HIGH = color OFF
  digitalWrite(RED_PIN, r > 0 ? LOW : HIGH);
  digitalWrite(BLUE_PIN, b > 0 ? LOW : HIGH);
}

void flashColor(int r, int b, int duration = 300) {
  setColor(r, b);
  delay(duration);
  setColor(0, 0);
}

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

  // Initialize LED pins (using analog pins as digital outputs)
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setColor(0, 0); // start off

  slideStart = millis();
}

void loop() {
  DateTime now = rtc.now();

  // Blink colon
  if (millis() - lastBlink >= blinkInterval) {
    lastBlink = millis();
    showColon = !showColon;
  }

  // Infinite LED blinking for testing
  if (millis() - lastLEDUpdate >= ledAnimationInterval) {
    lastLEDUpdate = millis();
    ledOn = !ledOn; // toggle on/off
    
    if (ledOn) {
      setColor(255, 255); // purple when on
    } else {
      setColor(0, 0); // off
    }
  }

  // Slide timer with LED flash on transitions (disabled during testing)
  /*
  unsigned long elapsed = millis() - slideStart;
  if (currentSlide == 1 && elapsed > slide1Duration) {
    currentSlide = 2;
    slideStart = millis();
    //flashColor(0, 255); // blue flash for date slide
  } else if (currentSlide == 2 && elapsed > slide2Duration) {
    currentSlide = 1;
    slideStart = millis();
    //flashColor(255, 255); // purple flash back to time (R+B = purple)
  }
  */

  // Clear display
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (currentSlide == 1) {
    // --- SLIDE 1: TIME (bottom aligned) ---
    
    // Progress bar (enabled for testing/visual feedback)
    display.setTextSize(1);
    int bars = map(now.second(), 0, 59, 0, 12);
    display.setCursor(0, 0);
    for (int i = 0; i < bars; i++) display.print("|");
    display.setCursor(0, 8);
    display.print("[");
    display.setCursor(bars * 6, 8);
    display.print("]");

    // Date (top-right corner: "TH 11") - clean and visible
    display.setTextSize(2);
    String shortDate = dayToString(now.dayOfTheWeek()).substring(0, 2) + " " + String(now.day());
    int16_t x1, y1; uint16_t w, h;
    display.getTextBounds(shortDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w, 0);
    display.print(shortDate);

    // Big time (keep size 3 to ensure colon visibility)
    char timeBuffer[6];
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d%c%02d",
             now.hour(), showColon ? ':' : ' ', now.minute());

    char secBuffer[3];
    snprintf(secBuffer, sizeof(secBuffer), "%02d", now.second());

    display.setTextSize(3);
    display.getTextBounds(timeBuffer, 0, 0, &x1, &y1, &w, &h);
    int xMain = (SCREEN_WIDTH - (w + 22)) / 2; // slightly more space for seconds
    int yMain = SCREEN_HEIGHT - h - 4; // bottom aligned
    display.setCursor(xMain, yMain);
    display.print(timeBuffer);

    // Seconds (well positioned)
    display.setTextSize(2);
    int xSec = xMain + w + 3;
    int ySec = yMain + 10; // better vertical alignment
    display.setCursor(xSec, ySec);
    display.print(secBuffer);

  } else if (currentSlide == 2) {
    // --- SLIDE 2: DATE (centered and attractive) ---
    display.setTextSize(3);
    String topDate = dayToString(now.dayOfTheWeek()) + " " + String(now.day());
    int16_t x1, y1; uint16_t w, h;
    display.getTextBounds(topDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 12);
    display.print(topDate);

    display.setTextSize(2);
    String bottomDate = monthToString(now.month()) + " " + String(now.year());
    display.getTextBounds(bottomDate, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 42);
    display.print(bottomDate);
    
    // Add decorative lines above and below
    display.drawLine(10, 8, SCREEN_WIDTH-10, 8, SSD1306_WHITE);
    display.drawLine(10, SCREEN_HEIGHT-8, SCREEN_WIDTH-10, SCREEN_HEIGHT-8, SSD1306_WHITE);
  }

  display.display();
  delay(100);
}