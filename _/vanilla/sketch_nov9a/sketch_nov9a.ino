#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DS3231 rtc;

// Function to calculate day of week (0=Sunday, 6=Saturday)
int calculateDayOfWeek(int y, int m, int d) {
    if (m < 3) {
        m += 12;
        y--;
    }
    int h = (d + (13 * (m + 1)) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
    return (h + 5) % 7;
}

// Function to get day name
String getDayName(int dayOfWeek) {
    const char* days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    return (dayOfWeek >= 0 && dayOfWeek < 7) ? days[dayOfWeek] : "ERR";
}

void setup() {
     Serial.begin(9600);
    
    // Initialize I2C
    Wire.begin();
    
    // Initialize RTC
    rtc.begin();
    
    // Auto-detect if RTC needs time set (if year is unrealistic)
    RTCDateTime dt = rtc.getDateTime();
    if (dt.year < 2020 || dt.year > 2030) {
        Serial.println("RTC needs time set, adjusting...");
        rtc.setDateTime(__DATE__, __TIME__);
    }
    
    // Initialize OLED
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
    
    Serial.println("RTC and OLED initialized successfully!");
}

void loop() {
    // Get complete date-time structure from RTC
    RTCDateTime dt = rtc.getDateTime();
    
    // Calculate day of week
    int dayOfWeek = calculateDayOfWeek(dt.year, dt.month, dt.day);
    String dayName = getDayName(dayOfWeek);
    
    // Print to Serial for debugging
    Serial.print("Time: ");
    Serial.print(dt.hour);
    Serial.print(":");
    Serial.print(dt.minute);
    Serial.print(":");
    Serial.print(dt.second);
    Serial.print("  Date: ");
    Serial.print(dt.day);
    Serial.print("/");
    Serial.print(dt.month);
    Serial.print("/");
    Serial.print(dt.year);
    Serial.print("  Day: ");
    Serial.println(dayName);
    
    // Clear display
    display.clearDisplay();
    
    // Display time (large)
    display.setTextSize(3);
    display.setCursor(0, 0);
    
    // Format time with leading zeros
    if (dt.hour < 10) display.print("0");
    display.print(dt.hour);
    display.print(":");
    if (dt.minute < 10) display.print("0");
    display.print(dt.minute);
    
    // Display seconds (smaller)
    display.setTextSize(2);
    display.setCursor(90, 8);
    display.print(":");
    if (dt.second < 10) display.print("0");
    display.print(dt.second);
    
    // Display date and day
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print(dt.day);
    display.print("/");
    display.print(dt.month);
    display.print("/");
    display.print(dt.year);
    display.print(" ");
    display.print(dayName);
    
    // Add a separator line
    display.drawLine(0, 32, SCREEN_WIDTH, 32, SSD1306_WHITE);
    
    display.display();
    delay(1000);
}