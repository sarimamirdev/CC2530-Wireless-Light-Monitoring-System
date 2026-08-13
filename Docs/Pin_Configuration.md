# Pin Configuration Guide

## CC2530 Wireless Light Monitoring System

This document provides detailed pin configuration and wiring instructions for both Board A (Receiver) and Board B (Transmitter).

---

## 📌 Table of Contents

1. [Board B - Transmitter Pin Configuration](#board-b---transmitter)
2. [Board A - Receiver Pin Configuration](#board-a---receiver)
3. [Wiring Diagrams](#wiring-diagrams)
4. [Power Supply Requirements](#power-supply-requirements)
5. [Common Connection Issues](#common-connection-issues)

---

## Board B - Transmitter

### Pin Overview

| **Pin** | **Function** | **Type** | **Description** |
|---------|--------------|----------|-----------------|
| **P0.0** | ADC Input | Analog Input | Light sensor (Photo-resistor) input |
| **P1.0** | LED1 | Digital Output | Indicator LED (Active Low) |
| **P1.1** | LED2 | Digital Output | Indicator LED (Active Low) |
| **VDD** | Power | Power | 3.3V Power Supply |
| **GND** | Ground | Ground | Common Ground |

### Detailed Connections

#### 1. Light Sensor (ADC Input - P0.0)

```
        +3.3V
          |
          R1 (10kΩ)
          |
          +-----> P0.0 (ADC Input)
          |
        LDR (Photo-resistor)
          |
         GND
```

**Components:**
- Photo-resistor (LDR) - 5mm
- Resistor R1 - 10kΩ (Voltage divider)

**Connection Steps:**
1. Connect one end of LDR to P0.0
2. Connect 10kΩ resistor from P0.0 to VDD (3.3V)
3. Connect other end of LDR to GND
4. This creates a voltage divider circuit

#### 2. LED Indicators (P1.0 and P1.1)

```
        +3.3V
          |
         LED1 (P1.0)
          |
       R2 (220Ω)
          |
         GND

        +3.3V
          |
         LED2 (P1.1)
          |
       R3 (220Ω)
          |
         GND
```

**Components:**
- 2x LEDs (Red/Green)
- 2x Resistors - 220Ω (Current limiting)

**Connection Steps:**
1. Connect LED1 anode (+) to VDD (3.3V)
2. Connect LED1 cathode (-) to 220Ω resistor
3. Connect 220Ω resistor to P1.0
4. Repeat for LED2 with P1.1

---

## Board A - Receiver

### Pin Overview

| **Pin** | **Function** | **Type** | **Description** |
|---------|--------------|----------|-----------------|
| **P1.0** | LED1 | Digital Output | Indicator LED (Active Low) |
| **P1.1** | LED2 | Digital Output | Indicator LED (Active Low) |
| **P0.2** | UART TX | Digital Output | Serial data to PC (115200 baud) |
| **P0.3** | UART RX | Digital Input | Serial data from PC (Optional) |
| **VDD** | Power | Power | 3.3V Power Supply |
| **GND** | Ground | Ground | Common Ground |

### Detailed Connections

#### 1. LED Indicators (P1.0 and P1.1)

```
        +3.3V
          |
         LED1 (P1.0)
          |
       R1 (220Ω)
          |
         GND

        +3.3V
          |
         LED2 (P1.1)
          |
       R2 (220Ω)
          |
         GND
```

**Components:**
- 2x LEDs (Red/Green)
- 2x Resistors - 220Ω

**Connection Steps:**
1. Connect LED1 anode (+) to VDD (3.3V)
2. Connect LED1 cathode (-) to 220Ω resistor
3. Connect 220Ω resistor to P1.0
4. Repeat for LED2 with P1.1

#### 2. UART Connection to PC

```
    Board A                    USB-Serial Adapter          PC
    ---------                  ----------------          -----
    P0.2 (TX)    ---------->   RX (Input)               [COM Port]
    P0.3 (RX)    <----------   TX (Output)                 |
    GND          ---------->   GND                        |
                                                          |
                                                      USB Port
```

**Components:**
- USB-to-Serial Adapter (FTDI, CP2102, etc.)
- 3x Jumper wires (Female-Female)

**Connection Steps:**
1. Connect Board A P0.2 to USB-Serial RX
2. Connect Board A P0.3 to USB-Serial TX (optional)
3. Connect Board A GND to USB-Serial GND
4. Connect USB-Serial to PC via USB

---

## Wiring Diagrams

### Complete System Diagram

```
                    ┌─────────────────────────────────────────────┐
                    │                                             │
                    │           BOARD B (Transmitter)             │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P0.0   │──────│  LDR    │              │
                    │  │  ADC    │      │  Sensor │              │
                    │  └─────────┘      └─────────┘              │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P1.0   │──────│  LED1   │              │
                    │  └─────────┘      └─────────┘              │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P1.1   │──────│  LED2   │              │
                    │  └─────────┘      └─────────┘              │
                    │                                             │
                    │  ┌─────────┐                               │
                    │  │  ANT    │───┐                           │
                    │  └─────────┘   │                           │
                    └────────────────┼───────────────────────────┘
                                     │
                                     │ BasicRF Wireless
                                     │ Communication
                                     │
                    ┌────────────────┼───────────────────────────┐
                    │  ┌─────────┐   │                           │
                    │  │  ANT    │───┘                           │
                    │  └─────────┘                               │
                    │                                             │
                    │           BOARD A (Receiver)                │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P1.0   │──────│  LED1   │              │
                    │  └─────────┘      └─────────┘              │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P1.1   │──────│  LED2   │              │
                    │  └─────────┘      └─────────┘              │
                    │                                             │
                    │  ┌─────────┐      ┌─────────┐              │
                    │  │  P0.2   │──────│  USB    │─── PC        │
                    │  │  UART   │      │  Serial │               │
                    │  └─────────┘      └─────────┘              │
                    └─────────────────────────────────────────────┘
```

---

## Power Supply Requirements

### Voltage Specifications

| **Parameter** | **Min** | **Typical** | **Max** | **Unit** |
|---------------|---------|-------------|---------|----------|
| **VDD** | 2.0V | 3.3V | 3.6V | V |
| **Current (Active)** | - | 25mA | 30mA | mA |
| **Current (Sleep)** | - | 1µA | 5µA | µA |

### Power Connection

```
    +---------------------+
    |                     |
    |   3.3V Power Supply |
    |                     |
    |   VDD ──────────────┼────> To CC2530 VDD
    |                     |
    |   GND ──────────────┼────> To CC2530 GND
    |                     |
    +---------------------+
```

---

## Common Connection Issues

### Issue 1: No LED Response

| **Problem** | **Solution** |
|-------------|--------------|
| LEDs not turning ON | Check active-low logic: LOW = ON, HIGH = OFF |
| Flickering LEDs | Check 220Ω resistors; replace if damaged |
| Wrong pin connection | Verify P1.0 and P1.1 connections |

### Issue 2: No UART Output

| **Problem** | **Solution** |
|-------------|--------------|
| No data on terminal | Check baud rate: 115200, 8N1 |
| Garbage characters | Verify TX/RX connections aren't swapped |
| No output at all | Check USB-Serial drivers are installed |

### Issue 3: No ADC Reading

| **Problem** | **Solution** |
|-------------|--------------|
| ADC value always 0 | Check P0.0 is configured as analog input |
| ADC value always 3.3V | Check LDR voltage divider connections |
| Unstable readings | Add 100nF capacitor between P0.0 and GND |

### Issue 4: No RF Communication

| **Problem** | **Solution** |
|-------------|--------------|
| No RF connection | Verify PAN_ID and CHANNEL match on both boards |
| Packet loss | Check antenna connection |
| Wrong addresses | Verify MY_ADDR and DEST_ADDR settings |

---

## Quick Reference Card

### Board B (Transmitter)
```
P0.0  → Light Sensor (ADC Input)
P1.0  → LED1 (Indicator)
P1.1  → LED2 (Indicator)
VDD   → 3.3V Power
GND   → Ground
```

### Board A (Receiver)
```
P1.0  → LED1 (Indicator)
P1.1  → LED2 (Indicator)
P0.2  → UART TX (to PC)
P0.3  → UART RX (from PC)
VDD   → 3.3V Power
GND   → Ground
```

---

## Example Wiring Checklist

### Board B Checklist
- [ ] LDR connected to P0.0 with 10kΩ pull-up resistor
- [ ] LED1 connected to P1.0 with 220Ω resistor
- [ ] LED2 connected to P1.1 with 220Ω resistor
- [ ] VDD connected to 3.3V
- [ ] GND connected to common ground
- [ ] Antenna properly connected

### Board A Checklist
- [ ] LED1 connected to P1.0 with 220Ω resistor
- [ ] LED2 connected to P1.1 with 220Ω resistor
- [ ] P0.2 connected to USB-Serial RX
- [ ] P0.3 connected to USB-Serial TX (optional)
- [ ] VDD connected to 3.3V
- [ ] GND connected to common ground
- [ ] Antenna properly connected

---

## Resources

- [CC2530 Datasheet](https://www.ti.com/lit/ds/symlink/cc2530.pdf)
- [BasicRF User Guide](https://www.ti.com/lit/pdf/swra236)
- [CC2530 Software Examples](https://www.ti.com/tool/CC2530-SW)

---

**Last Updated:** July 2026
**Version:** 1.0

---

[⬆ Back to Top](#pin-configuration-guide)
