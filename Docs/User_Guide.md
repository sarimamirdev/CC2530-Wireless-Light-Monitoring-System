# CC2530 Wireless Light Monitoring System — User Guide

This guide explains how to set up, configure, program, and test the CC2530 Wireless Light Monitoring System.

---

## 📌 Table of Contents

1. [System Overview](#system-overview)
2. [Hardware Requirements](#hardware-requirements)
3. [Hardware Connections](#hardware-connections)
4. [Software Requirements](#software-requirements)
5. [Source Code Configuration](#source-code-configuration)
6. [Building the Firmware](#building-the-firmware)
7. [Programming the Boards](#programming-the-boards)
8. [Running the System](#running-the-system)
9. [LED Behavior](#led-behavior)
10. [UART Output](#uart-output)
11. [Testing](#testing)
12. [Troubleshooting](#troubleshooting)

---

## 1. System Overview

The system consists of two CC2530 boards.

### Board B — Transmitter

Board B:

1. Reads the light sensor through ADC input P0.0.
2. Converts the ADC reading into a voltage value.
3. Formats the voltage as text.
4. Transmits the value to Board A using BasicRF.

### Board A — Receiver

Board A:

1. Receives the voltage value through BasicRF.
2. Converts the received text into a floating-point value.
3. Controls two LEDs according to the measured voltage.
4. Sends the formatted measurement to a PC through UART.

### Communication Flow

```text
Light Sensor
     │
     ▼
┌────────────────────┐
│ Board B            │
│ CC2530 Transmitter │
│ ADC → Voltage      │
└─────────┬──────────┘
          │
          │ BasicRF
          ▼
┌────────────────────┐
│ Board A            │
│ CC2530 Receiver    │
│ RF → LEDs → UART   │
└─────────┬──────────┘
          │
          │ UART
          ▼
         PC
