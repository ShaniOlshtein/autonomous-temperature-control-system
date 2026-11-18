// display_ui.cpp — display module (owner: display-ui).
// Stage 0: code moved verbatim from cooking_temperature_control.ino, no behavior change (decision 15).
#include <SPI.h>
#include <TFT_eSPI.h>
#include "pins.h"
#include "display_ui.h"
#include "timer.h"
#include "config_store.h"
#include "sensors.h"

// 1. Screen dimensions
static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 170;
static const int COL_WIDTH = SCREEN_WIDTH / 3;
static const int HEADER_HEIGHT = SCREEN_HEIGHT / 4;
static const int CONTENT_START_X = 1;
static const int CONTENT_START_Y = HEADER_HEIGHT + 1;
static const int SPR_WIDTH = COL_WIDTH - 2;
static const int SPR_HEIGHT = SCREEN_HEIGHT - CONTENT_START_Y;

// 2. Objects (the only TFT_eSPI instance in the sketch)
static TFT_eSPI tft = TFT_eSPI();
static TFT_eSprite sprTimer = TFT_eSprite(&tft);
static TFT_eSprite sprTarget = TFT_eSprite(&tft);
static TFT_eSprite sprCurrent = TFT_eSprite(&tft);

// 3. Display state
static uint16_t uiBackgroundColor = TFT_BLACK;
static bool uiNeedsRedraw = false;
static SelectionField selectedField = TIMER_POWER;
static bool selectedFieldVisible = true;
static unsigned long lastBlinkMillis = 0;
static const unsigned long SELECTION_BLINK_MS = 400;

// 4. Render functions (defined once)
static void renderTimer(bool isOn, String timeStr)
{
  sprTimer.fillSprite(uiBackgroundColor);
  sprTimer.setTextDatum(MC_DATUM);

  if (isOn && (selectedField != TIMER_POWER || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_GREEN, uiBackgroundColor);
    sprTimer.drawString("ON", COL_WIDTH / 2, 35, 4);
  }
  else if (!isOn && (selectedField != TIMER_POWER || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_RED, uiBackgroundColor);
    sprTimer.drawString("OFF", COL_WIDTH / 2, SPR_HEIGHT / 2, 4);
  }

  if (isOn && (selectedField != TIMER_VALUE || selectedFieldVisible))
  {
    sprTimer.setTextColor(TFT_WHITE, uiBackgroundColor);
    sprTimer.drawString(timeStr, COL_WIDTH / 2, 85, 4);
  }

  sprTimer.pushSprite(CONTENT_START_X, CONTENT_START_Y);
}

static void renderTargetTemp(float temp, bool hot)
{
  sprTarget.fillSprite(uiBackgroundColor);
  sprTarget.setTextDatum(MC_DATUM);

  String str = String((int)temp) + "C";
  if (selectedField != TARGET_VALUE || selectedFieldVisible)
  {
    sprTarget.setTextColor(TFT_WHITE, uiBackgroundColor);
    sprTarget.drawString(str, COL_WIDTH / 2, 38, 6);
  }

  if (selectedField != TARGET_MODE || selectedFieldVisible)
  {
    sprTarget.setTextColor(hot ? TFT_RED : TFT_CYAN, uiBackgroundColor);
    sprTarget.drawString(hot ? "HOT" : "COLD", COL_WIDTH / 2, 88, 4);
  }

  sprTarget.pushSprite(COL_WIDTH + CONTENT_START_X, CONTENT_START_Y);
}

static void renderCurrentTemp(float temp)
{
  sprCurrent.fillSprite(uiBackgroundColor);
  sprCurrent.setTextDatum(MC_DATUM);
  sprCurrent.setTextColor(TFT_WHITE, uiBackgroundColor);

  String str = isnan(temp) ? "ERR" : String((int)temp) + "C";
  sprCurrent.drawString(str, COL_WIDTH / 2, SPR_HEIGHT / 2, 6);

  sprCurrent.pushSprite((COL_WIDTH * 2) + CONTENT_START_X, CONTENT_START_Y);
}

// 5. Static UI drawing
static void drawStaticUI()
{
  tft.fillScreen(uiBackgroundColor);
  tft.drawFastHLine(0, HEADER_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
  tft.drawFastVLine(COL_WIDTH, 0, SCREEN_HEIGHT, TFT_WHITE);
  tft.drawFastVLine(COL_WIDTH * 2, 0, SCREEN_HEIGHT, TFT_WHITE);

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, uiBackgroundColor);

  int centerY = HEADER_HEIGHT / 2;
  tft.drawString("Timer", COL_WIDTH / 2, centerY, 2);
  tft.drawString("Target Temp", COL_WIDTH + (COL_WIDTH / 2), centerY, 2);
  tft.drawString("Current Temp", (COL_WIDTH * 2) + (COL_WIDTH / 2), centerY, 2);
}

// 6. Public functions
void displayUiBacklightOn()
{
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
}

void displayUiInit()
{
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(uiBackgroundColor);

  sprTimer.createSprite(SPR_WIDTH, SPR_HEIGHT);
  sprTarget.createSprite(SPR_WIDTH, SPR_HEIGHT);
  sprCurrent.createSprite(SPR_WIDTH, SPR_HEIGHT);

  drawStaticUI();
}

void displayUiUpdateBlink()
{
  unsigned long now = millis();
  if (now - lastBlinkMillis >= SELECTION_BLINK_MS)
  {
    lastBlinkMillis = now;
    selectedFieldVisible = !selectedFieldVisible;
  }
}

void displayUiRedrawIfNeeded()
{
  if (uiNeedsRedraw)
  {
    drawStaticUI();
    uiNeedsRedraw = false;
  }
}

void displayUiRender()
{
  int remainingSeconds = timerGetRemainingSeconds();

  int minutes = remainingSeconds / 60;
  int seconds = remainingSeconds % 60;
  String timerStr = (minutes < 10 ? "0" : "") + String(minutes) + ":" +
                    (seconds < 10 ? "0" : "") + String(seconds);

  renderTimer(timerIsEnabled(), timerStr);
  renderTargetTemp(configGetTargetTemp(), configGetIsHot());
  renderCurrentTemp(sensorsGetCurrentTemp());
}

void displayUiSetCookingColor(bool active)
{
  uiBackgroundColor = active ? TFT_DARKGREEN : TFT_BLACK;
  uiNeedsRedraw = true;
}

SelectionField displayUiGetSelectedField()
{
  return selectedField;
}

void displayUiSelectField(SelectionField field)
{
  selectedField = field;

  selectedFieldVisible = true;
  lastBlinkMillis = millis();
}
