# Project goal

A temperature controller for beer brewing/cooling on the LilyGO T-Display-S3 (ESP32-S3).

## Must do
- Keep the liquid at a target temperature automatically: heating, and cooling if a cooling output is built.
- Settings (target temperature, mode HEAT/COOL, hysteresis/threshold, timer) changeable from a phone over BLE, with default values.
- The screen shows the basics only: current temperature, target, mode, output state, timer, BLE status, faults.
- The buttons on the device keep working.
- Safe by default: relays OFF at boot and on any fault, a hard maximum temperature, and the control keeps working without BLE.

## Not in scope (yet)
WiFi, cloud, a custom phone app (first test with nRF Connect), battery operation.

## Current state
UI prototype with two DS18B20 sensors (average), the ADKeyboard and a countdown timer. No relay and no control logic yet (the summary document describes the intended design).

Source documents: `Project Summary_ Autonomous Cooking Control System.docx`, `README.md`, `Simulation.png`.
