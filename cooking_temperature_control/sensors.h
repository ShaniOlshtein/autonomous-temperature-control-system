#pragma once
// DS18B20 temperature sensors (non-blocking reads). Owner: control-logic.
// Stage 0: code moved from cooking_temperature_control.ino with no behavior change.
// OneWire / DallasTemperature objects are private to sensors.cpp.

// Starts the bus, prints the device count, and requests the first conversion without blocking.
void sensorsInit();

// Reads the results 750 ms after a request, prints them, and starts a new request every 2000 ms.
void sensorsUpdate();

// Average of the valid sensors in C; NAN when no sensor is valid or before the first read.
float sensorsGetCurrentTemp();
