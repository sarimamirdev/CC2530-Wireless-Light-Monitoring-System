# CC2530 Wireless Light Monitoring System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-CC2530-blue)](https://www.ti.com/product/CC2530)

## 📌 Overview

Two CC2530 modules communicating via **BasicRF** for wireless light monitoring:

- **Board B (Transmitter)**: Reads light sensor (ADC) → Sends data via RF
- **Board A (Receiver)**: Receives data → Controls LEDs → Sends to PC via UART

## 🎯 Features

- Wireless communication via BasicRF (IEEE 802.15.4-based)
- 12-bit ADC for light intensity measurement
- Automatic LED feedback based on light levels
- UART data transmission to PC (115200 baud)
- Customizable student name/class in output

## 🔌 Pin Connections

| Board | Pin | Function | Description |
|-------|-----|----------|-------------|
| **Board B (Transmitter)** | P0.0 | ADC Input | Light sensor input |
| | P1.0 | LED1 | Indicator (Active Low) |
| | P1.1 | LED2 | Indicator (Active Low) |
| **Board A (Receiver)** | P1.0 | LED1 | Indicator (Active Low) |
| | P1.1 | LED2 | Indicator (Active Low) |
| | P0.2 | UART TX | Serial output to PC |

## 📡 Network Settings

```c
#define MY_ADDR     0x0001      // Board A (Receiver)
#define DEST_ADDR   0x0002      // Board B (Transmitter)
#define CHANNEL     20          // RF Channel
#define PAN_ID      0x2301      // Network ID
```

## 🚀 Quick Start

1. **Clone the repository**
   ```bash
   git clone https://github.com/sarimamirdev/CC2530-Wireless-Light-Monitoring-System.git
   ```

2. **Open the code** in IAR EW8051

3. **Configure Board Address** in code:
   - Board A: `#define MY_ADDR 0x0001`
   - Board B: `#define MY_ADDR 0x0002`

4. **Build & Flash** using SmartRF Flash Programmer

5. **Monitor UART Output** (115200 baud, 8N1):
   ```
   The illumination data collected by Student Zhiyong of IoT Class 2401 is: 1.234V
   ```

## 📊 LED Behavior

| Voltage | Light Level | LED1 | LED2 |
|---------|-------------|------|------|
| > 1.5V | Bright | OFF | OFF |
| 0.5V - 1.5V | Moderate | ON | OFF |
| < 0.5V | Dark | ON | ON |

## 📁 Project Structure

```
CC2530-Wireless-Light-Monitoring/
├── Code/
│   ├── BoardA_Receiver.c      # Receiver code
│   ├── BoardB_Transmitter.c   # Transmitter code
│   └── Common_Headers.h       # Shared definitions
├── Images/                     # Hardware photos & screenshots
├── Docs/                       # Documentation
└── Tools/                      # Configuration files
```

## 🧰 Tools Required

- **IDE**: IAR Embedded Workbench for 8051
- **Programmer**: SmartRF Flash Programmer
- **Terminal**: Tera Term / PuTTY (115200 baud)
- **RF Config**: SmartRF Studio 7

## 📸 Images

![System Overview](Images/System_Overview.jpg)

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 👤 Author

**Zhiyong** - IoT Class 2401

---

⭐ Star this repository if you find it useful!
