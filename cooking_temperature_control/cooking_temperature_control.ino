// Cooking / brewing temperature controller for the LilyGO T-Display-S3.
// setup() and loop() only call each module's init/update functions, in the documented order
// (docs/stage0_refactor.md "Call order"). Owner of this file: control-logic.
// Stage 0: no behavior change (code moved into modules only).
//
// Modules (owner):
//   pins.h          - GPIO assignments (hardware-pins)
//   config_store.*  - settings: target temperature, HOT/COLD (config-store)
//   sensors.*       - DS18B20 non-blocking reads (control-logic)
//   timer.*         - cooking state and countdown (control-logic)
//   display_ui.*    - TFT, sprites, rendering (display-ui)
//   input.*         - ADKeyboard buttons (display-ui)

#include "display_ui.h"
#include "input.h"
#include "sensors.h"
#include "timer.h"

void setup()
{
  Serial.begin(115200);
  Serial.setTxTimeoutMs(0);  // G3: never block loop() on USB-CDC writes when no host reads the port (decision 37)
  displayUiBacklightOn();
  inputInit();
  sensorsInit();
  displayUiInit();
}

void loop()
{
  inputUpdate();
  displayUiUpdateBlink();
  sensorsUpdate();
  displayUiRedrawIfNeeded();
  timerUpdate();
  displayUiRender();
}
