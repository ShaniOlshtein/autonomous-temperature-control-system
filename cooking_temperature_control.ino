#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

#define LCD_BL 38
#define BTN_LEFT 0
#define BTN_RIGHT 14

// משתני המשחק
int paddleX = 130;
const int paddleY = 150;
const int paddleWidth = 60;
const int paddleHeight = 10;

int ballX = 160;
int ballY = 20;
int ballSpeedY = 3;
int ballRadius = 6;

int score = 0;
bool gameOver = false;

void setup() {
  Serial.begin(115200);

  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);


  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);

  spr.createSprite(320, 170);
}

void loop() {
  if (!gameOver) {
   
    if (digitalRead(BTN_LEFT) == LOW) {
      paddleX -= 5;
      if (paddleX < 0) paddleX = 0;
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      paddleX += 5;
      if (paddleX + paddleWidth > 320) paddleX = 320 - paddleWidth;
    }

  
    ballY += ballSpeedY;

  
    if (ballY + ballRadius >= paddleY && ballY - ballRadius <= paddleY + paddleHeight) {
      if (ballX >= paddleX && ballX <= paddleX + paddleWidth) {
        score++;
        ballY = 10;
        ballX = random(20, 300);
        ballSpeedY += 0.5;
      }
    }


    if (ballY > 170) {
      gameOver = true;
    }
  } else {
   
    if (digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
      score = 0;
      ballY = 20;
      ballX = 160;
      ballSpeedY = 3;
      paddleX = 130;
      gameOver = false;
      delay(300); 
    }
  }

  spr.fillSprite(TFT_BLACK);

  if (!gameOver) {
    spr.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, TFT_CYAN);
    
   
    spr.fillCircle(ballX, ballY, ballRadius, TFT_YELLOW);

    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Score: " + String(score), 10, 10, 2);
  } else {
   
    spr.setTextColor(TFT_RED, TFT_BLACK);
    spr.drawString("GAME OVER", 80, 50, 4);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.drawString("Final Score: " + String(score), 100, 90, 2);
    spr.drawString("Press any button to restart", 60, 120, 2);
  }


  spr.pushSprite(0, 0);

  delay(16); 
}
