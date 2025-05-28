# Vertical Tuinen

This repository contains the code and schematics for **Vertical Tuinen**, a school project aimed at creating a smart vertical garden system that automates the watering of plants. The system is designed to be energy-efficient, modular, and controlled via an external Android application (developed by a team member and not included in this repository).

## Project Overview

The vertical garden system:

- Automatically waters plants based on sensor data.
- Can be controlled remotely using an Android app via Bluetooth.
- Is built with affordability, sustainability, and simplicity in mind.

This project was part of a collaborative school assignment focused on combining hardware, embedded systems, and software control.

## Features

- **Automatic Watering:** Sensors monitor the moisture levels and trigger a water pump when necessary.
- **ESP32-based:** All logic runs on a microcontroller for low power usage and high reliability.
- **Modular Design:** Easy to scale and modify for different sizes or types of gardens.

## Hardware Used

- Arduino Nano
- Moisture sensors
- Water pump
- Relay module
- HC-05 Bluetooth module
- 3D printed frame for mounting
- Tubing and water reservoir

## Setup Instructions

1. Clone this repository or download the code.
2. Flash the code to your ESP32 using VS code with PlatformIO.
3. Wire up the components according to the code pinouts.
4. Use the companion Android app to manually water the plants or monitor the system.

> ⚠️ The Android app is not included in this repository. It was developed separately by a group member.

## Usage

Once powered:

- The system will automatically monitor the soil moisture.
- If soil is too dry, the pump is triggered for a fixed duration.
- Optionally, use the Android app to override or monitor the system manually.

## Contributing

As this was a school project, no active development is expected. However, feel free to fork the project and adapt it for your own needs!


## Acknowledgements

- Project developed as part of a school group assignment.
