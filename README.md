# OLED Clock Display with DS3231 RTC

A minimalistic yet stylish digital clock project using an **SSD1306 OLED display** and a **DS3231 Real-Time Clock (RTC)** module, built with Arduino. The display alternates between a dynamic time view and a clean date view, with future support for RGB LED indicators.

## 🕹 Features

- ⏱ Digital clock display with blinking colon
- 📆 Auto-rotating slides (time and date)
- 📊 Visual progress bar for seconds
- 💡 Planned RGB LED integration (e.g., ambient alerts)
- 🔋 Battery-backed RTC with automatic time sync on first run

## 📷 UI Layout

### Slide 1: Time View
- Large centered `HH:MM:SS` display
- Top-left: second progress bar (`|`)
- Top-right: short weekday + date (`MO 11`)

### Slide 2: Date View
- Centered large `DAY DD` + `MONTH YYYY`
- Horizontal lines for visual styling

## 🧰 Hardware Required

| Component            | Example/Spec            |
|---------------------|-------------------------|
| Microcontroller     | Arduino Uno / Nano      |
| OLED Display        | SSD1306 (128x64, I2C)   |
| RTC Module          | DS3231                  |
| RGB LED (optional)  | Common cathode RGB LED  |
| Resistors (for RGB) | ~220Ω recommended       |

> **Wiring Notes:**
> - OLED: SDA → A4, SCL → A5 (on Uno/Nano)
> - DS3231 uses I2C as well, connects to same pins
> - RGB LED pins: Red → D2, Green/Purple → D3, Blue → D4

## 📦 Dependencies
Install these from the Library Manager:
- Adafruit GFX Library
- Adafruit SSD1306
- RTClib

## 🚀 Future Improvements

- Add 12h/24h toggle
- Add button to manually switch slides
- Add RGB LED animations (e.g., flash on hour change)
- Add alarm or reminder functionality
- Support low-power dim mode

