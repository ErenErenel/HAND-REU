# HAND REU: Inflatable Electroadhesive Clutches

Welcome to the repository for the **Inflatable Electroadhesive (EA) Clutches** project, part of the [NSF HAND REU](https://hand-erc.org/) program at Northwestern University.

This project focuses on the **design, fabrication, and experimental modeling** of soft inflatable electroadhesive clutches for robotic hand applications. The research aims to optimize **engagement time**, **contact area**, and **force transmission** under varying pressure and preload conditions.

---

## Repository Structure

All work lives under `EA-Clutch/`, split into embedded firmware (PlatformIO/Arduino),
data-acquisition scripts, and characterization analysis:

```
EA-Clutch/
├── Actuation/                   High-voltage EA pad actuation (Teensy 4.1)
├── BrushedMotor/                Encoder-based brushed DC motor control (Teensy 4.1)
├── TransmittedPreload/          DRV8835 motor + dual NAU7802 load cells (Teensy 4.1)
├── Preload_with_ssd1306_display/ Load-cell preload readout on SSD1306 OLED (Teensy 4.1)
├── Syringe_Pump_Stepper_Motor/  TMC2209 stepper syringe pump for inflation (Arduino Uno)
├── DataAcq/                     Python/MATLAB serial data acquisition + plotting
├── Inflatable-Characterization/ Contact area, inflation angle, preload-vs-pressure analysis
├── Inflatable Electroadhesive Clutches Poster.pdf
└── Inflatable Electroadhesive Clutches Presentation.pptx
```

## Experimental Setup

The test rig combines several subsystems, each with its own firmware module:

- **Inflation control** — a stepper-driven syringe pump (`Syringe_Pump_Stepper_Motor`,
  Arduino Uno + TMC2209) sets the internal pressure of the inflatable clutch.
- **Preload application & measurement** — a brushed DC motor with a quadrature encoder
  (`BrushedMotor` / `TransmittedPreload`, Teensy 4.1) applies a controlled preload,
  while **NAU7802** load cells measure the applied and transmitted force.
- **Electroadhesion** — the `Actuation` firmware (Teensy 4.1) drives the high-voltage
  electrode pins for the EA pads, engaging pads in a sliding window pattern.
- **Live readout** — `Preload_with_ssd1306_display` shows filtered load-cell preload on
  an SSD1306 OLED for quick bench feedback.

## Data Acquisition & Characterization

- **`DataAcq/`** — Python scripts read load-cell and motor-load data over serial
  (115200 baud) and log/plot it; MATLAB (`contact_area.m`) supports contact-area work.
- **`Inflatable-Characterization/`** — analysis of the inflatable's behavior:
  - **Contact area** vs. pressure across inflation heights (Lₒ = 12–16 mm)
  - **Inflation angle / geometry** modeling
  - **Preload vs. pressure** acquisition and plotting

## Hardware

| Subsystem | Controller | Key parts |
|-----------|-----------|-----------|
| HV actuation | Teensy 4.1 | High-voltage electrode drivers |
| Preload / force | Teensy 4.1 | DRV8835 driver, brushed motor + encoder, NAU7802 load cells |
| OLED readout | Teensy 4.1 | NAU7802, SSD1306 display |
| Syringe pump | Arduino Uno | TMC2209 stepper driver, AccelStepper |

## Building the Firmware

Each firmware folder is a self-contained **PlatformIO** project. With the PlatformIO
CLI installed:

```bash
cd EA-Clutch/Actuation
pio run                # build
pio run --target upload  # flash to the board
```

The target board (Teensy 4.1 or Arduino Uno) is set in each module's `platformio.ini`.

## Running the Python Scripts

```bash
pip install pyserial numpy pandas matplotlib scipy
python EA-Clutch/DataAcq/preload.py
```

Update the serial `PORT` near the top of each script to match your system.

---

## Contributors

- **Eren Erenel:** Undergraduate Researcher, Northwestern University
- **Damian Gonzalez:** Mentor, Northwestern University
- **Affiliation:** Center for Robotics and Biosystems, Northwestern University
