# ESP32 110V Pressure Sensor

ESP32-C3 based water pressure monitoring system. The PCB is installed indoors (garage or similar protected location), while the IP68-rated pressure sensor is installed outdoors on water lines.

Use cases:
- Recycled water line monitoring for irrigation (check if pressure is sufficient before watering)
- Potable water pressure monitoring

## Bill of Materials

| Component | Description | Notes |
|-----------|-------------|-------|
| ESP32-C3-WROOM-02-N4 | Main microcontroller | WiFi-enabled |
| IRM-05-24 | AC-DC power supply | 110V AC to 24V DC |
| XL1509 | DC-DC buck converter | 24V to 3.3V for ESP32 |
| G1/4 Pressure Transducer | 0-100 PSI / 0-1 MPa, 0-5V output | [AliExpress](https://www.aliexpress.us/item/3256806103713273.html), IP68, select 0-1MPa 0-5V version |
| 20kΩ + 10kΩ resistors | Voltage divider | Scales 0-5V to ~1.67V for ADC |
| Phoenix terminal blocks | PCB connectors | 2P and 3P variants |
| USB-C 16P connector | Programming/debug | |
| 110V fuse | Input protection | |
| Misc | Capacitors, etc. | Standard supporting components |

Component datasheets are in the `components/` folder.

## Pinout

| Pin | Function |
|-----|----------|
| GPIO1 | Probe 1 (SIG1) |
| GPIO3 | Probe 2 (SIG2) |
| GPIO8 | Reset button |
| GPIO9 | Boot button |
| GPIO18 | USB D- |
| GPIO19 | USB D+ |

## Wiring

### Pressure Sensor (3-wire) to Phoenix Terminal Block

| Wire | Color | Connects to |
|------|-------|-------------|
| VCC | Brown | 24V terminal |
| GND | Blue | Ground terminal |
| Signal | Yellow | SIG terminal |
| (unused) | Black | Protective earth - leave disconnected |

### 110V Input

- Live → Fuse → IRM-05-24 AC input
- Neutral → IRM-05-24 AC input

The voltage divider (20kΩ/10kΩ) is on-board, scaling the 0-5V sensor output to ~1.67V for the ESP32 ADC.

## ESPHome Configuration

Configuration files are in the `esphome/` folder:

```
esphome/
  example-config.yaml              # Copy and customize for your device
  calibration-config.yaml          # Raw voltage output for calibration
  calibration-with-psi.yaml        # Calibration with voltage, moving average, and PSI
  templates/
    pressure_probe.yaml            # Production probe template
    calibration_pressure_probe.yaml # Calibration probe template
    calibration_tables.h           # Shared calibration tables (voltage → PSI)
  secrets.yaml.example             # Template showing required secrets
```

### Setup Steps

1. Copy `example-config.yaml` to `your-device-name.yaml`
2. Update `name`, `friendly_name`, and fallback AP `ssid` in your config
3. Copy `secrets.yaml.example` to `secrets.yaml`
4. Generate an API encryption key: `esphome gen-key`
5. Fill in all values in `secrets.yaml`:
   - `wifi_ssid` - Your WiFi network name
   - `wifi_password` - Your WiFi password
   - `pressure_probe_encryption_key` - Generated key from step 4
   - `pressure_probe_ota_password` - Password for over-the-air updates
   - `pressure_probe_wifi_ap_password` - Password for fallback hotspot

### Probe Configuration

The example config uses GPIO1 and GPIO3. Adjust `probe_pin` values if using different pins.

Each probe references a calibration table by ID. Set `calibration_id` in the probe's vars to match a table defined in `templates/calibration_tables.h`:

```yaml
packages:
  probe1: !include
    file: templates/pressure_probe.yaml
    vars:
      probe_id: "1"
      probe_pin: GPIO1
      calibration_id: "0-0.8MPa"
```

Multiple probes can share the same calibration table.

## Calibration

Calibration tables are defined in `esphome/templates/calibration_tables.h`. Each table maps ADC voltage readings to PSI using piecewise linear interpolation. Tables are identified by string IDs (e.g., `"0-0.8MPa"`, `"0-1.0MPa"`).

Current calibrations are tuned for:
- G1/4 pressure sensor (0-1 MPa / 0-100 PSI, 0-5V output)
- 24V sensor input voltage
- 20kΩ/10kΩ voltage divider

**Recalibrate if any of these differ.**

### Adding a Calibration Table

Edit `esphome/templates/calibration_tables.h`:

1. Add a new array with your measured voltage/PSI pairs:
   ```c
   static const float cal_my_sensor[][2] = {
     {0.000f,  0.0f},
     {0.100f, 10.0f},
     // ... your measured values
   };
   ```
2. Register it in the `CALIBRATION_TABLES` array:
   ```c
   {"my_sensor", cal_my_sensor, sizeof(cal_my_sensor) / sizeof(cal_my_sensor[0])},
   ```
3. Reference the ID in your device config: `calibration_id: "my_sensor"`

### Equipment

A manual hydrostatic pressure tester works well for calibration. The [IRONWALLS Hydrostatic Pressure Tester](https://www.amazon.com/dp/B07PX261P6) (5 MPa / 725 PSI range) uses G1/4 threads, so it connects directly to the sensor without adapters.

### Calibration Steps

1. Flash `calibration-config.yaml` (raw voltage only) or `calibration-with-psi.yaml` (voltage + moving average + PSI from current table)
2. Connect the pressure sensor to the tester
3. Open ESPHome logs to see readings
4. Apply pressure at several points (e.g., 0, 10, 20, 30... 100 PSI)
5. At each point, record the voltage reading from the logs
6. Add or update a calibration table in `templates/calibration_tables.h` (see "Adding a Calibration Table" above)
7. Flash the normal config (`example-config.yaml`) and verify readings match the gauge

## Known Issues

**Boot button wired as boot+reset.** The boot button is wired incorrectly and acts as both boot and reset. To enter boot mode on the ESP32-WROOM, just press the boot button and release — no need to hold it or press reset separately.

## PCB

- Project file: `ProPrj_esp32_pressure_sensor_2026-01-26.epro` (EasyEDA format)
- Open in EasyEDA to view schematic, PCB layout, generate Gerbers
- Can order directly through JLCPCB from EasyEDA
