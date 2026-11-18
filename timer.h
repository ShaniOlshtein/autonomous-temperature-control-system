#pragma once
// Cooking state and countdown timer (runtime state only). Owner: control-logic.
// Stage 0: code moved from cooking_temperature_control.ino with no behavior change.
// Stage 0 keeps one remainingSeconds value (decision 13); the duration/remaining split comes in Stage 2.
// This module never calls display code.

// Counts down one second every 1000 ms while cooking is active, the timer is ON, and time remains.
void timerUpdate();

// True when the timer is switched ON.
bool timerIsEnabled();

// Switches the timer ON/OFF; does NOT reset previousMillis (quirk E3 kept).
void timerToggleEnabled();

// Remaining time in seconds.
int timerGetRemainingSeconds();

// Adds delta seconds (may be negative), then clamps < 0 to 0; no upper limit (quirk E5 kept).
void timerAddSeconds(int delta);

// True while cooking is active (after START, until STOP).
bool timerIsCookingActive();

// Sets the cooking state only; the caller must then update the display itself.
void timerSetCookingActive(bool active);

// Restarts the one-second timestamp (previousMillis = millis()) only if the timer is ON.
void timerInitializeForCooking();
