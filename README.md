# ⏰ OLED Clock Display with DS3231 RTC + Alarm + LED + Buzzer

A minimalistic digital clock project using an **SSD1306 OLED display** and **DS3231 RTC**, with planned support for **LED animations** and **buzzer alarms** — built on Arduino/PlatformIO. Currently, the LED animations are temporarily removed for simplified debugging.

The display alternates between a time view and a date view, with built-in alert support and future external control via PC.

---

## 🕹 Features (Current Status)

- ⏱ Live digital clock with blinking colon
- 📆 Auto-rotating slides: time & date every 15 minutes
- 📊 Top progress bar: seconds or alarm countdown
- 🔔 Alarm system (planned: buzzer + LED feedback)
- 💡 LED animations temporarily removed; will reintroduce idle, transition, and alarm effects
- 🔋 Battery-backed DS3231 keeps time even when unplugged
- 🛠️ Code now set up in **PlatformIO** for easier library management

---

## 📷 UI Layout

### 🔹 Slide 1: Time View
- Large centered `HH:MM:SS`
- Top-left: seconds progress bar (12 ticks)
- Top-right: weekday + date (e.g. `Fr12`)
- Colon blinks every 500ms
- LED animations currently disabled

### 🔹 Slide 2: Date View (every 15 mins for 15s)
- Top: `DAY DD` (e.g. `FRI 12`)
- Bottom: `MONTH YYYY` (e.g. `SEP 2025`)
- LED animations currently disabled

### 🔔 Alarm View (planned)
- Top: countdown progress bar
- LED: solid red + flashing
- Buzzer: sounds for 30s, then snoozes for 5 min

---

## 🧰 Hardware Used

| Component             | Example/Spec              |
|----------------------|---------------------------|
| Microcontroller      | Arduino Uno / Nano        |
| OLED Display         | SSD1306 (128x64, I2C)     |
| RTC Module           | DS3231                    |
| Bi-color LED         | Common Anode (planned)    |
| Buzzer               | Active or passive (planned) |
| Push button (planned)| Momentary, NO             |

> **Wiring Notes:**
> - **OLED & DS3231**: share I2C — SDA → A4, SCL → A5 (Uno/Nano)
> - **LEDs & Buzzer**: planned pins, currently inactive
> - **Power**: Ensure sufficient current supply for LEDs + buzzer when implemented

---

## 📦 Dependencies

Install via Arduino Library Manager or PlatformIO:

- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `RTClib`
- `FastLED` (for future LED effects)

---

## 🔊 Alarm Logic (Planned)

- Alarm time hardcoded (e.g., `12:20`)
- Planned behavior at alarm time:
  - Buzzer rings for 30 seconds
  - LED turns solid red
  - Progress bar shows countdown
- Planned snooze: 5 minutes
- Future: manual stop / snooze button input

---

## 🚀 Next Steps / Development Roadmap

- [x] PlatformIO project structure
- [x] Basic OLED clock functionality working
- [x] Blink colon and update seconds progress bar
- [ ] Reintroduce LED idle breathing effect
- [ ] Implement LED transitions & alarm flashes
- [ ] Add buzzer alarm logic
- [ ] Button input for snooze/stop
- [ ] PC-based alarm configuration
- [ ] 12h / 24h time toggle
- [ ] EEPROM-based setting persistence

---

## 🤝 Credits

Built using:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [RTClib](https://github.com/adafruit/RTClib)
- [FastLED](https://github.com/FastLED/FastLED) (planned)

---

## 💡 Inspiration

A personal, modular smart clock project — from simple timekeeping to a customizable, extensible interface for alerts, audio, and ambient feedback. Built step by step.

---

> ⚙️ Open-source & yours to hack. Contributions welcome once live on GitHub!
