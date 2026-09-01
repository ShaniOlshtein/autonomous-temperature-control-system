#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

#define LCD_BL 38
#define BTN_LEFT 0
#define BTN_RIGHT 14





void setup() {
  Serial.begin(115200);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);


  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);

  spr.createSprite(320, 170);
  drawStaticUI();
}
void drawStaticUI(){
  int colWidth = 320 / 3;
  int headerHeight = 170 / 4;
  tft.fillScreen(TFT_BLACK);
  tft.drawFastHLine(0, headerHeight, 320, TFT_WHITE);
  tft.drawFastVLine(colWidth, 0, 170, TFT_WHITE);
  tft.drawFastVLine(colWidth*2, 0, 170, TFT_WHITE);
  tft.setTextDatum(MC_DATUM); 
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  int centerY = headerHeight / 2;
  tft.drawString("Timer", colWidth / 2, centerY, 3);
  tft.drawString("Target Temp", colWidth + (colWidth / 2), centerY, 3);
  tft.drawString("Current Temp", (colWidth * 2) + (colWidth / 2), centerY, 3);
  }
void loop() {
  
}
