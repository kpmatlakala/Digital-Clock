# ⏰ OLED Clock Display with DS3231 RTC + Alarm + LED + Buzzer

A minimalistic yet dynamic digital clock project using an **SSD1306 OLED display**, **DS3231 RTC**, **bi-color LED breathing animation**, and **buzzer alarm system** — all built on Arduino. The display alternates between an animated time view and a clean date view, with built-in alert support and future external control via PC.

---

## 🕹 Features

- ⏱ Live digital clock with blinking colon
- 📆 Auto-rotating slides: time & date every 15 minutes
- 📊 Top progress bar: seconds or alarm countdown
- 🔔 Alarm system with buzzer + LED feedback
- 💡 LED breathing animation (Blue idle, Purple transition, Red alert)
- 🔋 Battery-backed DS3231 keeps time even when unplugged
- 🖥️ Time auto-synced from PC during code upload

---

## 📷 UI Layout

### 🔹 Slide 1: Time View
- Large centered `HH:MM:SS`
- Top-left: seconds progress bar (12 ticks)
- Top-right: weekday + date (e.g. `Fr12`)
- LED: blue breathing animation
- Colon blinks every 500ms

### 🔹 Slide 2: Date View (every 15 mins for 15s)
- Top: `DAY DD` (e.g. `FRI 12`)
- Bottom: `MONTH YYYY` (e.g. `SEP 2025`)
- LED: purple breathing animation

### 🔔 Alarm View (triggered at preset time)
- Top: countdown progress bar (full → empty)
- LED: solid red + flashing
- Buzzer: sounds for 30s, then snoozes for 5 min

---

## 🧰 Hardware Used

| Component             | Example/Spec              |
|----------------------|---------------------------|
| Microcontroller      | Arduino Uno / Nano        |
| OLED Display         | SSD1306 (128x64, I2C)     |
| RTC Module           | DS3231                    |
| Bi-color LED         | Common Anode (Red/Blue)   |
| Buzzer               | Active or passive         |
| Audio Amp (future)   | PAM8406 w/ potentiometer  |
| Push button (future) | Momentary, NO             |

> **Wiring Notes:**
> - **OLED & DS3231**: share I2C — SDA → A4, SCL → A5 (Uno/Nano)
> - **LEDs**: Red → D9, Blue → D10 (common anode tied to VCC)
> - **Buzzer**: D8 (digital write HIGH/LOW)
> - **Power**: Ensure enough current supply for buzzer + LED

---

## 📦 Dependencies

Install via Arduino Library Manager:

- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `RTClib`

---

## 🔊 Alarm Logic

- Alarm time is **hardcoded** in code (e.g. `12:20`)
- At alarm time:
  - Buzzer rings for 30 seconds
  - LED turns **solid red**
  - Progress bar shows time left
- If no action taken, alarm **auto-snoozes** for 5 minutes
- Plan to add: manual stop / snooze button

---

## 🚀 Future Enhancements

- [ ] Button input (snooze/stop alarm)
- [ ] Audio amplifier + WAV support via external module
- [ ] PC config tool to set alarms from desktop
- [ ] 12h / 24h time mode toggle
- [ ] Ambient RGB LED notifications
- [ ] EEPROM-based setting persistence


---

## 🤝 Credits

Built using:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [RTClib](https://github.com/adafruit/RTClib)

---

## 💡 Inspiration

A personal, modular smart clock project — from simple timekeeping to a customizable, extensible interface for alerts, audio, and ambient feedback. Built one step at a time.

---

> ⚙️ Open-source & yours to hack. Contributions welcome once live on GitHub!




