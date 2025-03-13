# MH-Z19C CO2 Sensor with Arduino UNO R4 WiFi

This project implements a CO2 monitoring system using the MH-Z19C sensor and Arduino UNO R4 WiFi board. MH-Z19C uses UART communication protocol via digital pins 2 and 3.

## Hardware Requirements

- Arduino UNO board
- MH-Z19C CO2 Sensor
- LCD1602 with I2C adapter
- 5V Relay Module (1 channel)
- 5V DC Fan
- 4S AA battery pack power supply for fan

## Wiring

### Component Connections
| Component Pin | Arduino/Power Connection |
|--------------|-------------------------|
| **MH-Z19C Sensor** |                   |
| VCC          | 5V                     |
| GND          | GND                    |
| TX           | D2 (RX)                |
| RX           | D3 (TX)                |
| **LCD1602 I2C** |                     |
| VCC          | 5V                     |
| GND          | GND                    |
| SDA          | A4                     |
| SCL          | A5                     |
| **Relay Module** |                    |
| VCC          | 5V                     |
| GND          | GND                    |
| SIGNAL       | D4                     |
| **Fan** |                             |
| Positive     | Relay NO (Normally Open) |
| Negative     | Power Supply Ground    |

Note: Connect power supply positive to relay COM (Common) terminal.

![MH-Z19C Wiring Diagram](./images/MH-Z19C%20Wiring.png)

### Hardware Setup
1. The sensor requires 5V DC power supply
2. Allow proper ventilation around the sensor for accurate readings

### Required Libraries:
1. SoftwareSerial (included in Arduino IDE)
2. Wire (included in Arduino IDE)
3. LiquidCrystal I2C (Need to install)

## Installation

1. Clone this repository or download the source code
2. Install the LiquidCrystal I2C library:
   - In Arduino IDE, go to Tools > Manage Libraries
   - Search for "LiquidCrystal I2C"
   - Install the library by Frank de Brabander
3. Open the `MHZ19C_CO2_Monitor.ino` file in Arduino IDE
4. Connect your Arduino UNO to your computer
5. Select the correct board and port in Arduino IDE
6. Upload the code to your board

## Display Information

The LCD display shows:
- Line 1: "CO2 Level:"
- Line 2: CO2 concentration in ppm and air quality indicator
  - GOOD: ≤ 800 ppm
  - FAIR: 801-1200 ppm
  - POOR: > 1200 ppm

## Important Notes

1. **Warm-up Time**: The MH-Z19C sensor requires a warm-up time of about 3 minutes for accurate readings.

2. **Calibration**:
   - The sensor has automatic baseline correction
   - For manual calibration, expose the sensor to fresh air (400ppm CO2) for 20 minutes
   - Avoid using in enclosed spaces for long periods without ventilation

3. **Measurement Range**:
   - Standard measuring range: 400-5000ppm
   - Accuracy: ±50ppm +5% reading value
   - Response time: <120 seconds

4. **Operating Conditions**:
   - Temperature range: 0-50°C
   - Humidity range: 0-95% RH (non-condensing)

5. **Power Supply**:
   - Use a 5V power supply for the sensor and relay module
   - Use a 4S AA battery pack power supply for the fan

## Troubleshooting

1. If no readings are displayed:
   - Check all wire connections
   - Verify power supply voltage
   - Ensure TX/RX pins are correctly connected

2. If readings are inconsistent:
   - Allow proper warm-up time
   - Check for proper ventilation
   - Verify power supply stability

## Operation Logic

The system includes automatic ventilation control:
1. When CO2 levels are GOOD (≤800 ppm):
   - Fan remains OFF
   - Display shows "GOOD"

2. When CO2 levels are FAIR or POOR (>800 ppm):
   - Fan turns ON automatically
   - Display shows "FAIR" or "POOR"
   - Fan continues running until CO2 levels return to GOOD

3. Safety Features:
   - Fan turns off if sensor encounters an error
   - System requires 5 seconds warm-up time
   - Readings update every 2 seconds

## Safety Notes

1. **Electrical Safety**:
   - Ensure proper isolation between high and low voltage circuits
   - Use appropriate power supply for the fan
   - Double-check all connections before powering on

2. **Ventilation Notes**:
   - Position fan for optimal airflow
   - Ensure fan capacity matches room size
   - Keep ventilation paths unobstructed