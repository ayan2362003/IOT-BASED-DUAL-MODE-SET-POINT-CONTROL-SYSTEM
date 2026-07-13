<h1 align="center">🌡️ IoT Dual Mode Temperature Set Point Controller</h1>

<p align="center">
  <strong>Real-time temperature monitoring & cloud-connected set-point control using LPC2148 ARM7</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-LPC2148_ARM7-blue?style=for-the-badge&logo=arm&logoColor=white" alt="MCU"/>
  <img src="https://img.shields.io/badge/IDE-Keil_µVision-green?style=for-the-badge&logo=arm&logoColor=white" alt="IDE"/>
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak-orange?style=for-the-badge&logo=icloud&logoColor=white" alt="Cloud"/>
  <img src="https://img.shields.io/badge/WiFi-ESP8266-red?style=for-the-badge&logo=espressif&logoColor=white" alt="WiFi"/>
  <img src="https://img.shields.io/badge/Language-Embedded_C-yellow?style=for-the-badge&logo=c&logoColor=white" alt="Language"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Status-Working-brightgreen?style=flat-square" alt="Status"/>
  <img src="https://img.shields.io/badge/License-MIT-blue?style=flat-square" alt="License"/>
  <img src="https://img.shields.io/badge/Sensor-LM35-purple?style=flat-square" alt="Sensor"/>
  <img src="https://img.shields.io/badge/EEPROM-SPI_25LC-teal?style=flat-square" alt="EEPROM"/>
</p>

---

## 📌 Table of Contents

- [About The Project](#-about-the-project)
- [Key Features](#-key-features)
- [Hardware Components](#-hardware-components)
- [Circuit Details](#-circuit-details)
- [Software Architecture](#-software-architecture)
- [ThingSpeak Cloud Integration](#️-thingspeak-cloud-integration)
- [Working Flow](#-working-flow)
- [Project Structure](#-project-structure)
- [Build & Flash](#️-build--flash)
- [Configuration Reference](#️-configuration-reference)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)

---

## 📖 About The Project

An embedded IoT system built on the **LPC2148 (ARM7TDMI-S)** microcontroller that monitors ambient temperature using an **LM35 precision sensor**, displays readings on a **16×2 character LCD**, and supports **dual-mode set-point control**:

- 🔘 **Local Mode** — Enter set point directly via the on-board **4×4 matrix keypad**
- ☁️ **Remote Mode** — Set point is synced automatically from the **ThingSpeak IoT cloud**

When the measured temperature exceeds the active set point, a **buzzer alarm** is triggered. All set-point values are persisted in a **SPI EEPROM** so they survive power cycles.

### 💡 Why Dual Mode?

| Scenario | Mode Used | How It Works |
|----------|-----------|--------------|
| Operator is **on-site** | 🔘 Local | Press interrupt button → type value on keypad → saved to EEPROM + uploaded to cloud |
| Operator is **remote** | ☁️ Remote | Update Field 2 on ThingSpeak dashboard → MCU syncs every 3 min → applies new set point |
| **First boot** / empty EEPROM | 🔄 Default | Automatically loads `30.0°C` as the default set point |

### 📷 Project In Action

<p align="center">
  <img src="project/full%20hardware.jpg" alt="IoT Dual Mode Set Point Controller - Full Hardware Setup" width="600"/>
  &nbsp;&nbsp;
  <img src="project/tep%20and%20se%20show%20display.png" alt="LCD Display showing Temperature and Set Point" width="600"/>
</p>

<p align="center"><em>Left: complete hardware setup &nbsp;·&nbsp; Right: LCD showing live temperature (Line 1) and active set point (Line 2)</em></p>

---

## ✨ Key Features

<table>
  <tr>
    <td align="center">🌡️<br/><strong>Temperature<br/>Monitoring</strong></td>
    <td align="center">📶<br/><strong>WiFi Cloud<br/>Upload</strong></td>
    <td align="center">⌨️<br/><strong>Keypad<br/>Input</strong></td>
    <td align="center">💾<br/><strong>EEPROM<br/>Persistence</strong></td>
    <td align="center">🔔<br/><strong>Buzzer<br/>Alert</strong></td>
    <td align="center">☁️<br/><strong>Remote<br/>Set Point</strong></td>
  </tr>
</table>

| Feature | Details |
|---------|---------|
| 🌡️ **Real-time ADC sensing** | LM35 analog output → 10-bit ADC → °C conversion |
| 🖥️ **Live LCD display** | Simultaneous temperature + set-point on 16×2 HD44780 |
| ⌨️ **Local keypad entry** | 4×4 matrix with decimal support, backspace (`+`), confirm (`e`) |
| 💾 **Non-volatile storage** | Set point saved to SPI EEPROM byte-by-byte |
| 📶 **Wi-Fi connectivity** | ESP-01 module driven via AT commands over UART0 |
| ☁️ **Cloud upload** | Temperature → ThingSpeak Field 1 every **120 seconds** |
| 🔄 **Cloud sync** | Set point ← ThingSpeak Field 2 every **180 seconds** |
| 🔔 **Over-temp alarm** | Buzzer ON when `temperature > set_point` |
| ⚡ **Interrupt-driven** | EINT0 hardware interrupt for instant keypad trigger |
| 🛡️ **Fault tolerant** | Default 30.0°C if EEPROM is blank; safe JSON parsing |

---

## 🔧 Hardware Components

| # | Component | Specification | Interface | Purpose |
|---|-----------|--------------|-----------|---------|
| 1 | **LPC2148** | ARM7TDMI-S, 60 MHz, 512KB Flash, 40KB SRAM | — | Main MCU |
| 2 | **LM35** | 10 mV/°C, 0–100°C range, ±0.5°C accuracy | ADC Ch.1 | Temperature sensing |
| 3 | **16×2 LCD** | HD44780 controller, 5×8 dot matrix | 8-bit GPIO | Display output |
| 4 | **4×4 Keypad** | 16-key matrix (0–9, `.`, `e`, `+`, etc.) | 8 GPIO lines | Local set-point input |
| 5 | **ESP-01** | ESP8266, 802.11 b/g/n, AT firmware v1.x | UART @ 9600 | Wi-Fi / cloud bridge |
| 6 | **SPI EEPROM** | 25LC040 (or similar), 512 bytes | SPI @ 100 kHz | Set-point persistence |
| 7 | **Buzzer** | 3.3V active buzzer + NPN driver transistor | GPIO | Over-temp alarm |
| 8 | **Crystal** | 12 MHz HC-49S | OSC pins | Clock source (PLL ×5 → 60 MHz) |

---

## 🔌 Circuit Details

### 🖥️ LCD (HD44780) — 8-bit Parallel Mode on Port 0

<p align="center">
  <img src="project/LCD.png" alt="LCD Circuit Connection" width="700"/>
</p>

| LCD Signal | LPC2148 Pin | Type | Description |
|------------|-------------|------|-------------|
| D0–D7 | P0.8 – P0.15 | Output | 8-bit parallel data bus |
| RS | P0.16 | Output | Register Select (0=Command, 1=Data) |
| EN | P0.17 | Output | Enable pulse (falling-edge triggered) |
| RW | P0.18 | Output | Read/Write (hardwired LOW = Write-only) |
| V0 | Potentiometer (10kΩ) | Analog | Contrast adjustment |
| VSS / VDD | GND / 3.3V | Power | Supply rails |
| LED+/LED− | 3.3V / GND | Power | Backlight (via 100Ω resistor) |

---

### 🌡️ LM35 Temperature Sensor → ADC Channel 1

<p align="center">
  <img src="project/LM35.png" alt="LM35 Temperature Sensor Connection" width="700"/>
</p>

| LM35 Pin | Connection | Notes |
|----------|------------|-------|
| VCC | +3.3V | Supply voltage |
| OUT | P0.28 (AD0.1) | Analog output → ADC Channel 1 |
| GND | Ground | Reference |

> **Conversion Formula:**
> ```
> ADC Value (10-bit) → Voltage = (ADC_Result / 1023) × 3.3V
> Temperature (°C)  = Voltage × 100
> Temperature (°F)  = (°C × 1.8) + 32
> ```

> **ADC Configuration:**
> - Clock: 3 MHz (PCLK / divider)
> - Resolution: 10-bit (0–1023)
> - Mode: Software-triggered, single conversion

---

### ⌨️ 4×4 Matrix Keypad — Port 1

<p align="center">
  <img src="project/Keypad.png" alt="4x4 Matrix Keypad Connection" width="700"/>
</p>

| Line | LPC2148 Pin | Direction | Function |
|------|-------------|-----------|----------|
| ROW0 | P1.16 | Output (scan) | Row driver |
| ROW1 | P1.17 | Output (scan) | Row driver |
| ROW2 | P1.18 | Output (scan) | Row driver |
| ROW3 | P1.19 | Output (scan) | Row driver |
| COL0 | P1.20 | Input (pull-up) | Column reader |
| COL1 | P1.21 | Input (pull-up) | Column reader |
| COL2 | P1.22 | Input (pull-up) | Column reader |
| COL3 | P1.23 | Input (pull-up) | Column reader |

> **Special Keys:**
> - `0`–`9` and `.` → Digit/decimal entry
> - `e` → **Enter** (confirm set point)
> - `+` → **Backspace** (delete last character)

---

### 📶 ESP-01 Wi-Fi Module — UART0

<p align="center">
  <img src="project/ESP-01%20Wi-Fi%20Module.png" alt="ESP-01 WiFi Module Connection" width="700"/>
</p>

| ESP-01 Pin | LPC2148 Pin | Direction | Notes |
|------------|-------------|-----------|-------|
| RX | P0.0 (TXD0) | MCU → ESP | Serial data to ESP |
| TX | P0.1 (RXD0) | ESP → MCU | Serial data from ESP |
| VCC | 3.3V | Power | **Must be 3.3V** (not 5V!) |
| CH_PD | 3.3V | Enable | Chip enable (must be HIGH) |
| GND | GND | Power | Common ground |
| RST | 10kΩ → 3.3V | Reset | Active LOW, pulled HIGH |
| GPIO0 | N/C | — | Float for normal operation |
| GPIO2 | N/C | — | Float for normal operation |

> **UART Configuration:**
> | Parameter | Value |
> |-----------|-------|
> | Baud Rate | 9600 bps |
> | Data Bits | 8 |
> | Parity | None |
> | Stop Bits | 1 |
> | Flow Control | None |
> | RX Mode | **Interrupt-driven** (VIC Channel 6) |
> | Buffer Size | 200 bytes circular |

---

### 💾 SPI EEPROM (25LC040) — SPI0

<p align="center">
  <img src="project/spi-eeprom.png" alt="SPI EEPROM Connection" width="700"/>
</p>

| EEPROM Pin | LPC2148 Pin | Notes |
|------------|-------------|-------|
| CLK | P0.4 (SCK0) | SPI clock |
| SO (MISO) | P0.5 (MISO0) | Master In, Slave Out |
| SI (MOSI) | P0.6 (MOSI0) | Master Out, Slave In |
| CS̄ | P0.7 (GPIO) | Active LOW chip select |
| VCC | 3.3V | Supply |
| GND | GND | Ground |
| WP̄ / HOLD̄ | 3.3V | Tied HIGH (protections disabled) |

> **SPI Configuration:**
> | Parameter | Value |
> |-----------|-------|
> | Clock Speed | 100 kHz |
> | Mode | 0 (CPOL=0, CPHA=0) |
> | Bit Order | MSB first |
> | Role | Master |
>
> **EEPROM Commands Used:**
> | Opcode | Hex | Function |
> |--------|-----|----------|
> | WREN | `0x06` | Write Enable |
> | WRDI | `0x04` | Write Disable |
> | READ | `0x03` | Read byte at address |
> | WRITE | `0x02` | Write byte at address |

---

### 🔔 Buzzer — GPIO P0.23

| Signal | Pin | Logic |
|--------|-----|-------|
| Buzzer ON | P0.23 = HIGH | Temperature **exceeds** set point |
| Buzzer OFF | P0.23 = LOW | Temperature **within** set point |

---

### ⚡ External Interrupt (EINT0) — Set Point Trigger

| Parameter | Value |
|-----------|-------|
| Pin | P0.3 (configured as EINT0 via PINSEL) |
| VIC Channel | 15 |
| Trigger | Edge-triggered (`EXTMODE = 1`) |
| Action | Sets `flag = 1` → main loop enters keypad input mode |

---

### 📋 Complete GPIO Pin Summary

| LPC2148 Pin | Function | Peripheral | Direction |
|-------------|----------|------------|-----------|
| P0.0 | TXD0 | ESP-01 RX | Output |
| P0.1 | RXD0 | ESP-01 TX | Input |
| P0.3 | EINT0 | Interrupt button | Input |
| P0.4 | SCK0 | SPI EEPROM Clock | Output |
| P0.5 | MISO0 | SPI EEPROM Data In | Input |
| P0.6 | MOSI0 | SPI EEPROM Data Out | Output |
| P0.7 | GPIO (CS) | SPI EEPROM Select | Output |
| P0.8–P0.15 | GPIO | LCD Data D0–D7 | Output |
| P0.16 | GPIO | LCD RS | Output |
| P0.17 | GPIO | LCD EN | Output |
| P0.18 | GPIO | LCD RW | Output |
| P0.23 | GPIO | Buzzer | Output |
| P0.28 | AD0.1 | LM35 Analog Input | Input |
| P1.16–P1.19 | GPIO | Keypad Rows | Output |
| P1.20–P1.23 | GPIO | Keypad Columns | Input |

---

## 🧠 Software Architecture

### Module Breakdown

<p align="center">
  <img src="project/Module%20Breakdown.png" alt="Software Module Breakdown" width="700"/>
</p>

| Module | Files | Responsibility |
|--------|-------|---------------|
| **Application** | `main.c` | Main loop, set-point comparison, buzzer control, upload/sync timing |
| **WiFi/Cloud** | `esp01.c` `.h` | AT command sequences: connect AP, send to ThingSpeak, read from ThingSpeak |
| **Display** | `lcd.c` `.h` `lcd_defines.h` | HD44780 init, command/data write, string/float display |
| **Keypad** | `kpm.c` `.h` `kpm_defines.h` | Row-column scanning, key decoding, debounce |
| **ADC** | `adc.c` `.h` `adc_defines.h` | ADC init, single-channel read, voltage conversion |
| **EEPROM** | `spi_eeprom.c` `.h` `spi_eeprom_defines.h` | Byte-level read/write with WREN/READ/WRITE opcodes |
| **SPI Bus** | `spi.c` `.h` `spi_defines.h` | SPI0 master init, clock config |
| **UART** | `uart0.c` `.h` `uart0_defines.h` | TX char/string/float, interrupt-driven RX buffer |
| **RTC** | `rtc.c` `.h` `rtc_defines.h` | On-chip RTC initialization, time set |
| **Delay** | `delay.c` `.h` | Blocking millisecond and second delays |
| **Interrupt** | `eint0_irq_test.c` | EINT0 ISR — sets `flag=1` for keypad mode |
| **Pin Config** | `pin_connect_block.c` `.h` | PINSEL register configuration utility |
| **Types** | `types.h` | `u8`, `s8`, `u16`, `u32`, `f32`, `f64` aliases |
| **Startup** | `Startup.s` | ARM7 vector table, stack init, C runtime entry |

---

### 🔄 Dual Mode Operation Flow

<p align="center">
  <img src="project/Dual%20Mode%20Operation%20Flow.png" alt="Dual Mode Operation Flow Diagram" width="700"/>
</p>

| Event | Mode | Action |
|-------|------|--------|
| **EINT0 Interrupt** (flag=1) | 🔘 Local | Keypad input → save to EEPROM → upload to ThingSpeak Field 2 |
| **Every 120 seconds** | ☁️ Upload | Read LM35 → send temperature to ThingSpeak Field 1 |
| **Every 180 seconds** | ☁️ Sync | Read ThingSpeak Field 2 → if changed, update set point + save EEPROM |
| **Every 1 second** | 🔄 Loop | Read LM35 → update LCD → check buzzer |

---

## ☁️ ThingSpeak Cloud Integration

<p align="center">
  <img src="project/thingspeak.png" alt="ThingSpeak Dashboard" width="700"/>
</p>

| Direction | Field | API Key Type | HTTP Method | Interval | Trigger |
|-----------|-------|-------------|-------------|----------|---------|
| 📤 **Upload** temperature | Field 1 | Write Key | `GET /update?api_key=...&field1=` | Every 120s | Timer |
| 📤 **Upload** set point | Field 2 | Write Key | `GET /update?api_key=...&field2=` | On demand | Local keypad change |
| 📥 **Download** set point | Field 2 | Read Key | `GET /channels/{id}/feeds/last.json` | Every 180s | Timer |

**ESP-01 AT Command Sequence (for upload):**
```
AT+CIPCLOSE                                          ← Close any stale TCP connection
AT+CIPSTART="TCP","api.thingspeak.com",80            ← Open TCP to ThingSpeak
AT+CIPSEND=52                                        ← Declare payload length
GET /update?api_key=XXXXX&field1=32.5\r\n\r\n        ← HTTP GET request
AT+CIPCLOSE                                          ← Close connection
```

**ESP-01 AT Command Sequence (for sync/read):**
```
AT+CIPCLOSE
AT+CIPSTART="TCP","api.thingspeak.com",80
AT+CIPSEND=66
GET /channels/3246033/feeds/last.json?api_key=XXXXX\r\n\r\n
→ Parse JSON response for "field2":"<value>"
AT+CIPCLOSE
```

---

## 🔄 Working Flow

```
Power ON
   │
   ├─► Initialize all peripherals (LCD, ADC, KPM, SPI, UART0, RTC, EINT0)
   ├─► Connect ESP-01 to Wi-Fi ("Ayan" SSID)
   ├─► Read set point from EEPROM (default: 30.0°C if empty)
   │
   └─► Loop forever:
          │
          ├─ Read LM35 temperature (°C)
          ├─ Display on LCD:  Line1: TEMP: XX.XX C
          │                   Line2: SP:   XX.XX C
          │
          ├─ If temperature > set point → Buzzer ON 🔔
          │  Else                       → Buzzer OFF
          │
          ├─ Every 120s → Upload temp to ThingSpeak Field 1  📤
          │
          ├─ Every 180s → Sync set point from ThingSpeak Field 2  📥
          │               If changed → save to EEPROM + update locally
          │
          └─ On EINT0 (keypad button) → Enter local set-point mode  ⌨️
                                         Type value → Confirm with 'e'
                                         Save to EEPROM + upload to cloud
```

---

## 📁 Project Structure

```
Working_IOT_DUAL_MODE_SET_POINT/
│
├── 📄 README.md                          # This file
├── 🖼️ project/                           # Project images & diagrams
│   ├── full hardware.jpg                 # Complete hardware setup photo
│   ├── tep and se show display.png       # LCD output screenshot
│   ├── LCD.png                           # LCD circuit connection
│   ├── LM35.png                          # LM35 sensor connection
│   ├── Keypad.png                        # Keypad matrix connection
│   ├── ESP-01 Wi-Fi Module.png           # ESP-01 wiring
│   ├── spi-eeprom.png                    # SPI EEPROM connection
│   ├── thingspeak.png                    # ThingSpeak dashboard
│   ├── Module Breakdown.png              # Software architecture diagram
│   ├── Dual Mode Operation Flow.png      # Dual mode flowchart
│   └── Flash to LPC2148.png             # Flashing guide
│
└── Working_IOT_DUAL_MODE_SET_POINT/
    └── IOT_DUAL_MODE_SET_POINT/
        │
        │── ⭐ main.c                     # Main application entry point
        │
        │── 📶 esp01.c / esp01.h           # ESP-01 WiFi AT command driver
        │── 🌡️ adc.c / adc.h               # ADC initialization & read
        │── 🖥️ lcd.c / lcd.h               # 16x2 LCD HD44780 driver
        │── ⌨️ kpm.c / kpm.h               # 4x4 Keypad matrix scanner
        │── 💾 spi_eeprom.c / spi_eeprom.h # SPI EEPROM byte operations
        │── 🔌 spi.c / spi.h               # SPI0 master bus driver
        │── 📡 uart0.c / uart0.h           # UART0 TX/RX with ISR
        │── ⏰ rtc.c / rtc.h               # On-chip RTC driver
        │── ⏱️ delay.c / delay.h           # Blocking delay utilities
        │── ⚡ eint0_irq_test.c            # EINT0 interrupt handler
        │── 🔧 pin_connect_block.c / .h    # PINSEL register config
        │
        │── 📋 adc_defines.h               # ADC clock & bit defines
        │── 📋 lcd_defines.h               # LCD pin & command defines
        │── 📋 kpm_defines.h               # Keypad row/col pin map
        │── 📋 spi_defines.h               # SPI clock config
        │── 📋 uart0_defines.h             # Baud rate & UART config
        │── 📋 rtc_defines.h               # RTC configuration
        │── 📋 pin_functions_definition.h  # PINSEL function constants
        │── 📋 types.h                     # Type aliases (u8, u32, f32...)
        │
        │── 🔩 Startup.s                   # ARM7 startup assembly
        │── 🔩 aurdino.c / aurdino.h       # Arduino companion code
        │
        └── 🏗️ major.uvproj               # Keil µVision project file
            major.hex                      # Compiled firmware binary
```

---

## 🛠️ Build & Flash

### Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| **Keil µVision** | v4 or v5 (MDK-ARM) | IDE and ARM-CC compiler |
| **Flash Magic** | Latest | UART ISP programming tool |
| **USB-to-Serial** | Any FTDI/CH340/CP2102 | Connect PC to LPC2148 UART0 |
| **Power Supply** | 3.3V regulated | MCU and peripherals |

### Step-by-Step Build

```bash
# 1. Open the project in Keil
File → Open Project → IOT_DUAL_MODE_SET_POINT/major.uvproj

# 2. Select Target
Project → Select Target → "Target 1"

# 3. Build the project
Press F7  (or Project → Build Target)
# Output: major.hex in the project directory

# 4. Verify build
# Check Build Output window: "0 Error(s), 0 Warning(s)"
```

### Flash to LPC2148

<p align="center">
  <img src="project/Flash%20to%20LPC2148.png" alt="Flash to LPC2148 Guide" width="700"/>
</p>

```
Flash Magic Settings:
  ┌─────────────────────────────────┐
  │  Device     : LPC2148          │
  │  COM Port   : COM3 (or yours)  │
  │  Baud Rate  : 9600             │
  │  Interface  : None (ISP)       │
  │  Oscillator : 12 MHz           │
  │                                │
  │  Hex File   : major.hex        │
  │                                │
  │  [✓] Erase blocks used by file │
  │  [ ] Verify after programming  │
  └─────────────────────────────────┘

  Steps:
  1. Set BOOT pins: P0.14 = LOW (ISP mode)
  2. Press RESET on LPC2148 board
  3. Click "Start" in Flash Magic
  4. Wait for "Finished" message
  5. Set P0.14 = HIGH (normal mode)
  6. Press RESET to run firmware
```

---

## ⚙️ Configuration Reference

### 🔧 Application Settings (`main.c`)

| Define | Default | Unit | Description |
|--------|---------|------|-------------|
| `UPLOAD_INTERVAL` | `120` | seconds | Time between temperature cloud uploads |
| `SYNC_INTERVAL` | `180` | seconds | Time between remote set-point sync |
| `DEFAULT_SET_POINT` | `"30.0"` | °C | Fallback set point if EEPROM is empty/corrupt |
| `BUZZER` | `23` | GPIO pin | P0.23 — buzzer output pin |

### 📶 WiFi & Cloud Settings (`esp01.c`)

| Parameter | Default | ⚠️ Change Required |
|-----------|---------|---------------------|
| Wi-Fi SSID | `"Ayan"` | **Yes** — your network name |
| Wi-Fi Password | `"1234567890"` | **Yes** — your network password |
| ThingSpeak Write API Key | `0V4LV45QS58V4QF6` | **Yes** — from your ThingSpeak channel |
| ThingSpeak Read API Key | `LC3FVAJ49NHP4OKB` | **Yes** — from your ThingSpeak channel |
| ThingSpeak Channel ID | `3246033` | **Yes** — your channel number |

### ⚡ Hardware Clock Settings

| Define | File | Value | Derived |
|--------|------|-------|---------|
| `FOSC` | Multiple | `12000000` (12 MHz) | Crystal frequency |
| `CCLK` | Multiple | `FOSC × 5` = 60 MHz | CPU clock (via PLL) |
| `PCLK` | Multiple | `CCLK / 4` = 15 MHz | Peripheral clock |
| `BAUD` | `uart0_defines.h` | `9600` | UART baud rate |
| `ADC_CLK` | `adc_defines.h` | `3000000` (3 MHz) | ADC conversion clock |
| `SPI_CLK` | `spi_defines.h` | `100000` (100 kHz) | SPI bus clock |

---

## 🔍 Troubleshooting

| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| LCD shows nothing | Contrast not set | Adjust V0 potentiometer |
| LCD shows blocks | Init timing issue | Check `delay_ms()` after `init_lcd()` |
| ESP shows "WIFI FAILED" | Wrong SSID/password | Update credentials in `esp01.c` |
| ESP shows "TCP FAILED" | No internet / ThingSpeak down | Check router, verify API keys |
| "NO DATA" on sync | ThingSpeak Field 2 is empty | Write a value to Field 2 first |
| Buzzer always ON | Set point too low | Enter a higher set point via keypad |
| Buzzer never triggers | LM35 not connected / wrong channel | Check P0.28 wiring, verify ADC Ch.1 |
| EEPROM not saving | CS pin wrong / SPI not init | Verify P0.7 wiring, check `init_spi()` |
| Keypad not responding | Wrong port pins | Verify P1.16–P1.23 connections |
| Set point resets on boot | EEPROM blank (0xFF) | Normal on first boot — enter value via keypad |

---

## 🤝 Contributing

Contributions are welcome! Here's how:

1. **Fork** this repository
2. **Create** a feature branch: `git checkout -b feature/my-improvement`
3. **Commit** your changes: `git commit -m "Add: description of change"`
4. **Push** to the branch: `git push origin feature/my-improvement`
5. **Open** a Pull Request

### Ideas for Improvement

- [ ] Add **multiple temperature sensor** support (multi-channel ADC)
- [ ] Implement **PID control** for relay/heater output
- [ ] Add **OLED display** support (I2C SSD1306)
- [ ] Create a **web dashboard** instead of ThingSpeak
- [ ] Add **data logging** to SD card via SPI
- [ ] Implement **OTA firmware update** via ESP-01
- [ ] Add **Blynk / MQTT** protocol support

---

## 📄 License

This project is open source and available for **educational and academic purposes**.  
Feel free to use, modify, and share with attribution.

---

<p align="center">
  <strong>Built with ❤️ by Ayan</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-LPC2148_ARM7-informational?style=flat-square&logo=arm" alt="Platform"/>
  <img src="https://img.shields.io/badge/Toolchain-Keil_MDK-success?style=flat-square" alt="Toolchain"/>
  <img src="https://img.shields.io/badge/Cloud-ThingSpeak_IoT-orange?style=flat-square" alt="Cloud"/>
  <img src="https://img.shields.io/badge/Language-Embedded_C-blue?style=flat-square&logo=c" alt="Language"/>
</p>

<p align="center">
  ⭐ Star this repo if you found it helpful!
</p>
