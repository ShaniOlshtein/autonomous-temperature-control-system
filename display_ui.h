#pragma once
// Display module (owner: display-ui). Owns the only TFT_eSPI instance and the sprites
// sprTimer, sprTarget, sprCurrent (all private to display_ui.cpp). Stage 0: moved code, no behavior change.

// Editable fields, in navigation order (declared only here; also used by input.*).
enum SelectionField
{
  TIMER_POWER,
  TIMER_VALUE,
  TARGET_VALUE,
  TARGET_MODE,
  SELECTION_FIELD_COUNT
};

// Turns the backlight on (pinMode + HIGH on LCD_BL). Call right after Serial.begin().
void displayUiBacklightOn();

// tft.init, rotation 1, fillScreen, creates the 3 sprites, draws the static UI. Call after sensors init.
void displayUiInit();

// Toggles the visibility of the selected field every SELECTION_BLINK_MS (was updateSelectionBlink).
void displayUiUpdateBlink();

// Redraws the static grid and headers if a redraw was requested, then clears the request.
void displayUiRedrawIfNeeded();

// Builds the mm:ss string and renders the timer, target and current columns (reads module getters).
void displayUiRender();

// Sets the background color for the cooking state and requests a redraw on EVERY call (was updateCookingDisplayColor).
void displayUiSetCookingColor(bool active);

// Returns the currently selected field.
SelectionField displayUiGetSelectedField();

// Selects a field, makes it visible and restarts the blink period (same order as the old moveSelection).
void displayUiSelectField(SelectionField field);
