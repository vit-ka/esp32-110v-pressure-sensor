# Documentation Design for ESP32 110V Pressure Sensor

## Overview

Add documentation to the project so it can be understood and reproduced after a year. The documentation will consist of a single README.md at the project root, plus ESPHome configuration files stored in the repo with secrets separated out.

## README.md Structure

### 1. Overview
Brief description: ESP32-C3 based water pressure monitoring system. PCB installed indoors (garage), IP68 pressure sensor installed outdoors on water lines (recycled water for irrigation, potable water).

### 2. Bill of Materials

| Component | Description | Notes |
|-----------|-------------|-------|
| ESP32-C3-WROOM-02-N4 | Main microcontroller | WiFi-enabled |
| IRM-05-24 | AC-DC power supply | 110V AC to 24V DC |
| XL1509 | DC-DC buck converter | 24V to 3.3V for ESP32 |
| G1/4 Pressure Transducer | 0-100 PSI / 0-1 MPa, 0-5V output | [AliExpress](https://www.aliexpress.us/item/3256806103713273.html), IP68 |
| 20k + 10k resistors | Voltage divider | Scales 0-5V to ~1.67V for ADC |
| Phoenix terminal blocks | PCB connectors | 2P and 3P variants |
| USB-C 16P connector | Programming/debug | |
| 110V fuse | Input protection | |
| Misc | Capacitors, etc. | Standard supporting components |

### 3. Pinout

| Pin | Function | PCB Design | Reality (reworked) |
|-----|----------|------------|-------------------|
| GPIO1 | Probe 1 (SIG1) | - | Yes (jumper wire) |
| GPIO3 | Probe 2 (SIG2) | - | Yes (jumper wire) |
| GPIO4 | Probe 1 (SIG1) | Yes | - |
| GPIO5 | Probe 2 (SIG2) | Yes | - |
| GPIO8 | Reset button | Yes | Yes |
| GPIO9 | Boot button | Yes | Yes |
| GPIO18 | USB D- | Yes | Yes |
| GPIO19 | USB D+ | Yes | Yes |

**Critical warning:** The current PCB routes signals to GPIO4/GPIO5. GPIO5 is on ADC2, which is unavailable when WiFi is active. Must rework board to route SIG1 to GPIO1 and SIG2 to GPIO3 (or other ADC1 pins).

### 4. Wiring

**Pressure Sensor (3-wire) to Phoenix Terminal Block:**

| Wire | Color | Connects to |
|------|-------|-------------|
| VCC | Brown | 24V terminal |
| GND | Blue | Ground terminal |
| Signal | Yellow | SIG terminal |
| (unused) | Black | Protective earth - leave disconnected |

**110V Input:**
- Live -> Fuse -> IRM-05-24 AC input
- Neutral -> IRM-05-24 AC input

Note: Voltage divider (20k/10k) is on-board, scaling 0-5V sensor output to ~1.67V for ESP32 ADC.

### 5. ESPHome Configuration

Document file structure and setup steps:
1. Copy `example-config.yaml` to `your-device-name.yaml`
2. Update device name, friendly name, fallback AP name
3. Copy `secrets.yaml.example` to `secrets.yaml`, fill in all values
4. Generate API key with `esphome gen-key`

### 6. Calibration

Calibration table is tuned for:
- G1/4 pressure sensor (0-1MPa, 0-5V output)
- 24V input voltage
- 20k/10k voltage divider

Recalibrate if any of these differ.

### 7. PCB

- Project file: `ProPrj_esp32_pressure_sensor_2026-01-10.epro` (EasyEDA format)
- Open in EasyEDA to view schematic, PCB layout, generate Gerbers
- Can order directly through JLCPCB from EasyEDA
- **Known issue:** Fix ADC pin routing before ordering (GPIO4/5 to GPIO1/3), or plan for manual rework

## New Files to Create

### esphome/example-config.yaml
Main ESPHome configuration file with:
- Device name placeholders
- References to secrets via `!secret`
- Include of probe template with pin configuration

### esphome/templates/pressure_probe.yaml
Probe template with:
- ADC sensor with smoothing filter
- Voltage-to-PSI conversion with calibration table
- Variables for probe_id and probe_pin

### esphome/secrets.yaml.example
Template showing required secrets:
```yaml
wifi_ssid: "your-wifi-name"
wifi_password: "your-wifi-password"
api_encryption_key: "generate-with-esphome-gen-key"
ota_password: "your-ota-password"
ap_password: "fallback-hotspot-password"
```

## Implementation Order

1. Create `esphome/` directory structure
2. Write `esphome/templates/pressure_probe.yaml`
3. Write `esphome/example-config.yaml`
4. Write `esphome/secrets.yaml.example`
5. Write `README.md`
6. Update `.gitignore` to exclude `esphome/secrets.yaml`
