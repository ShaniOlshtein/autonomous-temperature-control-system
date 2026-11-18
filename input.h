#pragma once
// ADKeyboard input module (owner: display-ui). Buttons only write settings/state through
// config_store and timer setters; they never drive relays. Stage 0: moved code, no behavior change.

// Configures the ADKeyboard pin (pinMode INPUT + 11 dB attenuation on ADKEY_OUT_PIN). Call before sensors init.
void inputInit();

// Reads and debounces the ADKeyboard, prints the 250 ms ADC debug line, handles presses and long presses (was updateButtons).
void inputUpdate();
