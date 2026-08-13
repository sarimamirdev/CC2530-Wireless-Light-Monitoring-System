# User Guide

## CC2530 Wireless Light Monitoring System

Welcome to the CC2530 Wireless Light Monitoring System! This guide will help you set up, configure, and use the system step by step.

---

## 📌 Table of Contents

1. [Introduction](#introduction)
2. [What's Included](#whats-included)
3. [Hardware Setup](#hardware-setup)
4. [Software Installation](#software-installation)
5. [Programming the Boards](#programming-the-boards)
6. [Running the System](#running-the-system)
7. [Testing & Verification](#testing--verification)
8. [Troubleshooting](#troubleshooting)
9. [Frequently Asked Questions](#frequently-asked-questions)
10. [Getting Help](#getting-help)

---

## Introduction

The **CC2530 Wireless Light Monitoring System** is a wireless sensor network that monitors light intensity remotely. It consists of two nodes:

- **Board B (Transmitter)**: Reads light sensor data and sends it wirelessly
- **Board A (Receiver)**: Receives data, shows status on LEDs, and sends data to PC

### System Features

✅ **Wireless Communication** - No cables between boards  
✅ **Real-time Monitoring** - Instant light intensity updates  
✅ **Visual Feedback** - LEDs indicate light level  
✅ **PC Logging** - Data displayed on PC via UART  
✅ **Easy Customization** - Change student name and class

---

## What's Included

### Hardware Components

| **Item** | **Quantity** | **Description** |
|----------|--------------|-----------------|
| CC2530 Module | 2 | Main microcontroller with RF |
| CC2530 Development Board | 2 | Breakout board with pins |
| Light Sensor (LDR) | 1 | Photo-resistor for light sensing |
| LED (Red/Green) | 4 | Indicator LEDs (2 per board) |
| Resistor (10kΩ) | 1 | For LDR voltage divider |
| Resistor (220Ω) | 4 | For LED current limiting |
| USB-to-Serial Adapter | 1 | For PC communication |
| USB Cable | 1 | Power and programming |
| Jumper Wires | 20+ | For connections |

### Software Components

| **Item** | **Description** |
|----------|-----------------|
| IAR EW8051 | Compiler/IDE for CC2530 |
| SmartRF Flash Programmer | Flashing tool |
| SmartRF Studio 7 | RF configuration tool |
| Terminal Software | For viewing UART data |
| Project Files | Source code from repository |

---

## Hardware Setup

### Step 1: Set Up Board B (Transmitter)

#### 1.1 Connect Light Sensor

```
    Board B Connections:
    ┌─────────────────────────────────┐
    │                                 │
    │  P0.0 ─────┬─────── LDR ───────┼── GND
    │            │                    │
    │          10kΩ                   │
    │            │                    │
    │          VDD (3.3V)             │
    │                                 │
    └─────────────────────────────────┘
```

**Steps:**
1. Connect one end of LDR to P0.0
2. Connect 10kΩ resistor from P0.0 to VDD (3.3V)
3. Connect other end of LDR to GND

#### 1.2 Connect LEDs

```
    Board B LED Connections:
    ┌─────────────────────────────────┐
    │                                 │
    │  VDD ─────────> LED1 ──220Ω── P1.0 │
    │                                 │
    │  VDD ─────────> LED2 ──220Ω── P1.1 │
    │                                 │
    └─────────────────────────────────┘
```

**Steps:**
1. Connect LED1 anode (+) to VDD (3.3V)
2. Connect LED1 cathode (-) to 220Ω resistor
3. Connect 220Ω resistor to P1.0
4. Repeat for LED2 with P1.1

### Step 2: Set Up Board A (Receiver)

#### 2.1 Connect LEDs

```
    Board A LED Connections:
    ┌─────────────────────────────────┐
    │                                 │
    │  VDD ─────────> LED1 ──220Ω── P1.0 │
    │                                 │
    │  VDD ─────────> LED2 ──220Ω── P1.1 │
    │                                 │
    └─────────────────────────────────┘
```

**Steps:**
1. Connect LED1 anode (+) to VDD (3.3V)
2. Connect LED1 cathode (-) to 220Ω resistor
3. Connect 220Ω resistor to P1.0
4. Repeat for LED2 with P1.1

#### 2.2 Connect UART to PC

```
    Board A              USB-Serial Adapter
    ┌─────────┐         ┌──────────────┐
    │  P0.2   │────────>│  RX (Input)  │
    │  (TX)   │         │              │
    │         │         │              │
    │  P0.3   │<────────│  TX (Output) │
    │  (RX)   │         │              │
    │         │         │              │
    │  GND    │────────>│  GND         │
    └─────────┘         └──────┬───────┘
                               │
                           USB Port
                               │
                            ┌──┴──┐
                            │ PC  │
                            └─────┘
```

**Steps:**
1. Connect Board A P0.2 to USB-Serial RX
2. Connect Board A P0.3 to USB-Serial TX (optional)
3. Connect Board A GND to USB-Serial GND
4. Plug USB-Serial into PC

---

## Software Installation

### 1. Install IAR EW8051

1. **Download IAR EW8051**
   - Go to: [iar.com/ew8051](https://www.iar.com/ew8051)
   - Create account and download the installer
   - Choose version 10.x or later

2. **Install IAR EW8051**
   - Run the installer
   - Follow installation wizard
   - Accept license agreement
   - Choose installation directory (default: C:\Program Files\IAR Systems\)

3. **Activate License**
   - Launch IAR EW8051
   - Enter license key (if purchased) or start trial
   - Click "Activate"

### 2. Install SmartRF Flash Programmer

1. **Download SmartRF Flash Programmer**
   - Go to: [ti.com/tool/flash-programmer](https://www.ti.com/tool/flash-programmer)
   - Download the latest version

2. **Install SmartRF Flash Programmer**
   - Run the installer
   - Follow installation wizard
   - Accept license agreement

3. **Connect USB Programming Adapter**
   - Plug CC2530 programming adapter into USB
   - Windows will install drivers automatically

### 3. Install SmartRF Studio 7 (Optional)

1. **Download SmartRF Studio 7**
   - Go to: [ti.com/tool/smartrftm-studio](https://www.ti.com/tool/smartrftm-studio)
   - Download the latest version

2. **Install SmartRF Studio 7**
   - Run the installer
   - Follow installation wizard

### 4. Install Terminal Software

Choose any terminal software:

| **Software** | **Download Link** | **Platform** |
|--------------|-------------------|--------------|
| **Tera Term** | [teratermproject.github.io](https://teratermproject.github.io/) | Windows |
| **PuTTY** | [putty.org](https://www.putty.org/) | Windows/Linux/Mac |
| **RealTerm** | [realterm.sourceforge.net](https://realterm.sourceforge.net/) | Windows |
| **Serial Monitor** | Built-in Arduino IDE | Windows/Mac/Linux |

---

## Programming the Boards

### Step 1: Connect Programming Hardware

```
    CC2530 Board          USB Programmer
    ┌─────────┐         ┌──────────────┐
    │  DC     │────────>│  DC          │
    │  DD     │────────>│  DD          │
    │  RST    │────────>│  RST         │
    │  GND    │────────>│  GND         │
    └─────────┘         └──────┬───────┘
                               │
                           USB Port
                               │
                            ┌──┴──┐
                            │ PC  │
                            └─────┘
```

### Step 2: Open Project in IAR EW8051

1. Launch **IAR EW8051**
2. Click **File → Open → Workspace**
3. Browse to your project folder
4. Select the `.eww` file
5. Click **Open**

### Step 3: Select Board Type

1. In the Project Explorer, right-click on the project
2. Select **Options**
3. Go to **C/C++ Compiler → Preprocessor**
4. Add define:
   - For Board A: `#define MY_ADDR 0x0001`
   - For Board B: `#define MY_ADDR 0x0002`
5. Click **OK**

### Step 4: Build the Project

1. Click **Project → Rebuild All** (or press F7)
2. Check Build Output:
   ```
   Total number of errors: 0
   Total number of warnings: 0
   ```
3. If no errors, proceed to flashing

### Step 5: Flash the Board

1. **Connect CC2530 board** to programming adapter
2. Open **SmartRF Flash Programmer**
3. Select the connected CC2530 device
4. Click **Browse** and select the compiled `.hex` file
5. Click **Program**
6. Wait for completion message: **"Programming successful!"**

### Step 6: Repeat for Second Board

1. Change address define for the other board
2. Rebuild the project
3. Connect the second board
4. Program with the new firmware

---

## Running the System

### Step 1: Power On

1. **Power on Board B** (Transmitter)
   - Connect USB power or 3.3V supply
   - The board will start reading sensor data

2. **Power on Board A** (Receiver)
   - Connect USB power or 3.3V supply
   - The board will start listening for data

### Step 2: Monitor UART Output

1. **Connect USB-Serial Adapter** to PC
2. Open **Terminal Software**
3. Configure settings:
   ```
   Baud Rate:   115200
   Data Bits:   8
   Stop Bits:   1
   Parity:      None
   Flow Control:None
   ```

4. **Connect** to the COM port
5. You should see output like:
   ```
   The illumination data collected by Student Zhiyong of IoT Class 2401 is: 1.234V
   The illumination data collected by Student Zhiyong of IoT Class 2401 is: 0.876V
   The illumination data collected by Student Zhiyong of IoT Class 2401 is: 2.145V
   ```

### Step 3: Observe LED Behavior

| **Voltage Reading** | **Light Level** | **LED1** | **LED2** | **Meaning** |
|---------------------|-----------------|----------|----------|-------------|
| > 1.5V | Bright | OFF | OFF | Room is well-lit |
| 0.5V - 1.5V | Moderate | ON | OFF | Room is moderately lit |
| < 0.5V | Dark | ON | ON | Room is dark |

### Step 4: Test the System

1. **Test Bright Light**
   - Shine a flashlight on the LDR
   - Voltage reading should increase (> 1.5V)
   - Both LEDs should turn OFF

2. **Test Moderate Light**
   - Move to normal room lighting
   - Voltage reading should be between 0.5V - 1.5V
   - LED1 should be ON, LED2 OFF

3. **Test Darkness**
   - Cover the LDR completely
   - Voltage reading should drop (< 0.5V)
   - Both LEDs should turn ON

---

## Testing & Verification

### Test 1: Check UART Communication

```bash
# Expected output format
The illumination data collected by Student [Name] of IoT Class [Class] is: [Voltage]V

# Example
The illumination data collected by Student Zhiyong of IoT Class 2401 is: 1.234V
```

### Test 2: Check Wireless Communication

1. **Verify RF Link**:
   - Disconnect Board A from power
   - Board B should still be running
   - Wait 5 seconds, reconnect Board A
   - Board A should resume receiving data

2. **Check Packet Loss**:
   - Monitor UART output for 1 minute
   - Count received messages
   - Should be approximately 60 messages (1 per second)

### Test 3: Check ADC Accuracy

| **Test Condition** | **Expected Voltage** | **Tolerance** |
|--------------------|---------------------|---------------|
| Direct Sunlight | > 2.0V | ±0.2V |
| Normal Room Light | 0.8V - 1.2V | ±0.2V |
| Covered Sensor | < 0.3V | ±0.1V |

### Test 4: Check LED Response

| **Action** | **Expected Response** | **Result** |
|------------|----------------------|------------|
| Cover LDR | Both LEDs ON | [ ] Pass |
| Expose to light | LED1 ON, LED2 OFF | [ ] Pass |
| Very bright light | Both LEDs OFF | [ ] Pass |

---

## Troubleshooting

### Problem 1: No UART Output

| **Possible Cause** | **Solution** |
|--------------------|--------------|
| Wrong baud rate | Set terminal to 115200, 8N1 |
| Wrong COM port | Check Device Manager for correct port |
| USB-Serial drivers | Install or update drivers |
| TX/RX swapped | Swap P0.2 and P0.3 connections |
| Board not powered | Check 3.3V power connection |

### Problem 2: No RF Communication

| **Possible Cause** | **Solution** |
|--------------------|--------------|
| Different PAN_ID | Verify both boards have same PAN_ID |
| Different channel | Verify both boards have same channel |
| Wrong addresses | Check MY_ADDR and DEST_ADDR settings |
| Antenna issue | Check antenna connection |
| Distance too far | Move boards closer (within 10m) |

### Problem 3: LEDs Not Working

| **Possible Cause** | **Solution** |
|--------------------|--------------|
| Wrong polarity | LEDs should be connected anode to VDD |
| Missing resistor | Add 220Ω current limiting resistor |
| Active low logic | LOW = ON, HIGH = OFF |
| Pin configuration | Verify P1.0 and P1.1 connections |
| LED burnt | Replace LED |

### Problem 4: ADC Reading Always 0

| **Possible Cause** | **Solution** |
|--------------------|--------------|
| Wrong pin configuration | Ensure P0.0 is analog input |
| Missing pull-up | Add 10kΩ resistor to VDD |
| LDR not connected | Check LDR connections |
| Broken LDR | Replace LDR |
| ADC not initialized | Check ADC_Init() function |

### Problem 5: Garbage UART Output

| **Possible Cause** | **Solution** |
|--------------------|--------------|
| Wrong baud rate | Set terminal to 115200 |
| Wrong parity | Set parity to none |
| Data bits mismatch | Set data bits to 8 |
| Stop bits mismatch | Set stop bits to 1 |
| Noise on UART lines | Use shielded wires |

---

## Frequently Asked Questions

### Q1: Can I use other light sensors?

**A:** Yes! You can use:
- Photo-transistors
- Ambient light sensors (BH1750, TSL2561)
- Photodiodes
- Just adjust the voltage divider values

### Q2: What's the maximum communication distance?

**A:** Up to 30 meters line-of-sight (indoor) and up to 100 meters (outdoor). Range depends on:
- Environment (walls/obstacles)
- Antenna type
- Power setting

### Q3: Can I add more boards?

**A:** Yes! Add more boards with unique addresses:
- Board C: `0x0003`
- Board D: `0x0004`
- etc.

### Q4: How to change the student name/class?

**A:** Edit in `BoardA_Receiver.c`:
```c
#define STUDENT_NAME    "Your Name"
#define STUDENT_CLASS   "Your Class"
```
Recompile and reflash the firmware.

### Q5: Can I use it with batteries?

**A:** Yes! The CC2530 is low-power:
- Active mode: ~25mA
- Idle mode: ~1µA
- Use 2x AA batteries (3V) or Li-ion battery

### Q6: Why is the voltage reading different?

**A:** Voltage depends on:
- Light intensity
- LDR characteristics
- Resistor value
- Power supply voltage (3.3V)

### Q7: How to log data to a file?

**A:** Use terminal software with logging feature:
- **Tera Term**: File → Log
- **PuTTY**: Session → Logging
- **RealTerm**: Capture → Start

### Q8: Can I use it with Arduino?

**A:** Not directly, but you can:
- Use UART to Arduino for processing
- Use SPI to communicate with Arduino
- Or use it as standalone system

---

## Getting Help

### Online Resources

| **Resource** | **Link** | **Description** |
|--------------|----------|-----------------|
| GitHub Repository | [Link](https://github.com/sarimamirdev/CC2530-Wireless-Light-Monitoring-System) | Project source code |
| CC2530 Datasheet | [TI Website](https://www.ti.com/lit/ds/symlink/cc2530.pdf) | Technical specifications |
| BasicRF Guide | [TI Website](https://www.ti.com/lit/pdf/swra236) | RF protocol documentation |
| IAR Support | [IAR Website](https://www.iar.com/support) | Compiler support |

### Contact

- **GitHub Issues**: [Create an Issue](https://github.com/sarimamirdev/CC2530-Wireless-Light-Monitoring-System/issues)
- **Email**: (Your email here)
- **Portfolio**: (Your portfolio link)

### Quick Links

- 📖 [Pin Configuration](Pin_Configuration.md)
- 🔧 [Project README](../README.md)
- 📄 [License](../LICENSE)

---

## Appendix

### A. Terminal Settings Example (Tera Term)

```
1. File → New Connection
2. Select COM port (e.g., COM3)
3. Setup → Serial Port:
   - Baud Rate: 115200
   - Data: 8 bits
   - Parity: none
   - Stop: 1 bit
   - Flow Control: none
4. Setup → Terminal:
   - ANSI color: unchecked
   - Terminal size: 80x24
```

### B. Command Quick Reference

| **Action** | **Command/Procedure** |
|------------|-----------------------|
| Build project | Press F7 |
| Download HEX | SmartRF Flash Programmer |
| Open terminal | Tera Term / PuTTY |
| Reset board | Press reset button |
| Change address | Edit `#define MY_ADDR` |

---

## Revision History

| **Version** | **Date** | **Changes** |
|-------------|----------|-------------|
| 1.0 | July 2026 | Initial release |
| 1.1 | - | Updated troubleshooting section |

---

## Feedback

Your feedback helps improve this guide! If you encounter any issues or have suggestions:

1. Create an issue on GitHub
2. Submit a pull request
3. Contact the author

---

**Document Version:** 1.0  
**Last Updated:** July 2026  
**Author:** Sarim Amir

---

[⬆ Back to Top](#user-guide)
