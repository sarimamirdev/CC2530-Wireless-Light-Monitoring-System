# CC2530 Wireless Light Monitoring System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-CC2530-blue)](https://www.ti.com/product/CC2530)
[![IDE](https://img.shields.io/badge/IDE-IAR%20EW8051-green)](https://www.iar.com/ew8051)

## 📌 Overview

This project implements a **wireless light monitoring system** using two **TI CC2530** modules communicating via **BasicRF protocol**.

### System Components:
- **Board B (Transmitter)**: Reads light intensity using ADC → Transmits data wirelessly
- **Board A (Receiver)**: Receives data → Controls LEDs → Sends formatted data to PC via UART

## ✨ Features

- **Wireless Communication**: BasicRF protocol (IEEE 802.15.4 based)
- **Real-time Sensing**: 12-bit ADC for light intensity measurement
- **Automatic LED Feedback**: 
  - Voltage > 1.5V: All LEDs OFF (Bright light)
  - Voltage 0.5V - 1.5V: One LED ON (Moderate light)  
  - Voltage < 0.5V: Two LEDs ON (Dark)
- **UART Output**: Formatted data sent to PC at 115200 baud
- **Customizable**: Student name and class displayed in UART output

## 📸 Images

![ADC Light Intensity Circuit](Images/CC2530_ADC_Light_Intensity_Circuit.jpg)
*ADC Light Intensity Circuit Diagram*

![BasicRF Light Monitoring System](Images/CC2530_BasicRF_Light_Monitoring.jpg)
*BasicRF Light Monitoring System*

![Wireless Light Monitor Overview](Images/CC2530_Wireless_Light_Monitor.jpg)
*Wireless Light Monitoring System Overview*

## 🛠️ Hardware Requirements

| Component | Specification |
|-----------|---------------|
| Microcontroller | TI CC2530 (2.4GHz SoC) |
| Light Sensor | Photo-resistor / Light sensor (ADC input) |
| LEDs | 2× Indicator LEDs (Active Low) |
| Power Supply | 3.3V DC |
| PC Connection | UART via USB-Serial adapter |

## 🔌 Pin Connections

### Board B (Transmitter)
| Pin | Function | Description |
|-----|----------|-------------|
| P0.0 | ADC Input | Light sensor analog input |
| P1.0 | LED1 | Indicator (Active Low) |
| P1.1 | LED2 | Indicator (Active Low) |

### Board A (Receiver)
| Pin | Function | Description |
|-----|----------|-------------|
| P1.0 | LED1 | Indicator (Active Low) |
| P1.1 | LED2 | Indicator (Active Low) |
| P0.2 | UART TX | Serial output to PC |

## 📡 Network Configuration

```c
#define MY_ADDR     0x0001      // Board A (Receiver)
#define DEST_ADDR   0x0002      // Board B (Transmitter)
#define CHANNEL     20          // RF Channel (11-26)
#define PAN_ID      0x2301      // Personal Area Network ID
#define UART_BAUDRATE 115200
```

## 🚀 Quick Start

### 1. Clone the Repository
```bash
git clone https://github.com/sarimamirdev/CC2530-Wireless-Light-Monitoring-System.git
cd CC2530-Wireless-Light-Monitoring-System
```

### 2. Open in IAR EW8051
- Open the project in IAR Embedded Workbench for 8051

### 3. Configure Board Address
In the code, set the appropriate address:
- **Board A (Receiver)**: `#define MY_ADDR 0x0001`
- **Board B (Transmitter)**: `#define MY_ADDR 0x0002`

### 4. Build & Flash
- Compile the project (F7)
- Flash using SmartRF Flash Programmer

### 5. Monitor UART Output
- Connect Board A to PC via USB-Serial
- Open terminal (115200 baud, 8N1)
- You'll see output like:
```
The illumination data collected by Student Zhiyong of IoT Class 2401 is: 1.234V
```

## 📊 LED Behavior

| Voltage Range | Light Level | LED1 | LED2 |
|---------------|-------------|------|------|
| > 1.5V | Bright | OFF | OFF |
| 0.5V - 1.5V | Moderate | ON | OFF |
| < 0.5V | Dark | ON | ON |

## 📁 Project Structure

```
CC2530-Wireless-Light-Monitoring-System/
├── Code/
│   ├── BoardA_Receiver.c      # Complete receiver code
│   ├── BoardB_Transmitter.c   # Complete transmitter code
│   └── Common_Headers.h       # Shared definitions
├── Images/                     # Hardware photos & screenshots
│   ├── CC2530_ADC_Light_Intensity_Circuit.jpg
│   ├── CC2530_BasicRF_Light_Monitoring.jpg
│   └── CC2530_Wireless_Light_Monitor.jpg
├── .gitignore
├── LICENSE
└── README.md
```

## 🧰 Tools Required

- **IDE**: IAR Embedded Workbench for 8051
- **Programmer**: SmartRF Flash Programmer
- **Terminal**: Tera Term / PuTTY (115200 baud)
- **RF Config**: SmartRF Studio 7

## 🔧 Troubleshooting

| Issue | Solution |
|-------|----------|
| No RF communication | Check PAN_ID and CHANNEL match on both boards |
| UART not working | Verify baud rate (115200) and TX/RX connections |
| No ADC reading | Ensure P0.0 is configured as analog input |
| LEDs not responding | Check active-low logic (LOW = ON) |

## 📄 License

This project is licensed under the **MIT License** – see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Sarim Amir** - IoT Developer

[![GitHub](https://img.shields.io/badge/GitHub-sarimamirdev-181717?style=for-the-badge&logo=github)](https://github.com/sarimamirdev)

---

⭐ If you find this project helpful, please consider giving it a star!
