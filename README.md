# ⏰ OLED Clock Display with DS3231 RTC + Temperature + LED Breathing Animation

A clean and minimal digital clock project using an **SSD1306 OLED**, **DS3231 RTC**, and **NeoPixel LED strip**, built with **PlatformIO**.  
The display now alternates between **date and temperature**, and LED breathing animation has been re-enabled with a smooth non-flickering effect.

---

## 🕹 Features (Current Status)

- ⏱ **Live digital clock** with blinking colon  
- 🌡 **DS3231 on-board temperature** displayed every 15 seconds (alternates with date)  
- 📆 **Compact day/date view** (e.g., `Fr12`)  
- 💡 **Cyan breathing NeoPixel animation** (smooth, fixed peak flicker)  
- 🔋 **Battery-backed RTC** keeps time when powered off  
- 🛠 **PlatformIO project structure** with organized classes (`ClockDisplay`, `LEDAnimator`)  

---

## 📷 UI Layout

### 🔹 **Time View (Primary)**
- Large centered `HH:MM` with blinking colon  
- Seconds shown small on the right  
- Top-right alternates every 15s between:
  - **DATE** (e.g., `Fr12`)
  - **TEMPERATURE** (e.g., `25°C` or `25`˙`C`)  

### 🔹 **Temperature/Date Alternation**
- Toggles automatically every **15 seconds**  
- Temperature uses DS3231 built-in thermistor and is formatted cleanly  

### 🔹 **LED Animation (Idle Mode)**
- Smooth cyan breathe effect  
- No flicker at brightness peaks  
- Fully encapsulated in `LEDAnimator.cpp`

---

## 🧰 Hardware Used

| Component             | Notes                         |
|----------------------|-------------------------------|
| Microcontroller      | Arduino Nano (ATmega328P)     |
| Display              | SSD1306 128×64 I2C OLED       |
| RTC Module           | DS3231 (temp + clock)         |
| LEDs                 | WS2812b (NeoPixel, 5 LEDs)    |
| Buzzer *(planned)*   | Active/passive                |
| Button *(planned)*   | Snooze / mode                 |

**Wiring (Nano):**
- **I2C:** SDA → A4, SCL → A5  
- **NeoPixel:** D6  
- **RTC power:** 3.3V or 5V depending on module  

---

## 📦 Dependencies (PlatformIO)

```ini
lib_deps =
    adafruit/Adafruit BusIO
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    adafruit/RTClib
    northernwidget/DS3231
    adafruit/Adafruit NeoPixel
```
All installed automatically through platformio.ini.

## 🔥 LED Animation System
The LED engine is modular and supports:
- Idle breathing mode (currently active)
- Future modes:
  * Alarm flashing
  * Transition fades
  * Multi-color effects

The breathing animation was updated to remove the peak brightness flicker by clamping and reversing direction before drawing.

## 🌡 Temperature Display Notes
- Uses rtc.getTemperature()
- DS3231 temperature updates internally every 64 seconds
- Display format currently:
  * 25˙C or fallback 25'C
- Rendering position corrected to align with compact date region

🚀 Roadmap
- [x] PlatformIO migration
- [x] OLED clock with time + seconds
- [x] Day/date compact view
- [x] Temperature alternating UI
- [x] NeoPixel breathing animation (fixed)
- [ ] LED transition modes
- [ ] Buzzer + alarm logic
- [ ] Snooze button input
- [ ] PC-based settings editor
- [ ] Animation profiles (quiet/night mode)
- [ ] EEPROM save/load of settings

## 🤝 Credits
- Adafruit GFX & SSD1306
- RTClib
- DS3231 Library
- Adafruit NeoPixel

## 💡 Inspiration
A small modular clock evolving into a full-featured smart desktop companion — ambient LEDs, alarm logic, and a clean UI.