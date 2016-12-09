#include <Arduino.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library
// #include "Joystick.h"
// #include "Cursor.h"
#include "display.h"

#define BLACK    0x0000
#define GREEN    0x07E0
#define YELLOW   0xFFE0
#define ORANGE   0xfa00

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7735 tft2 = Adafruit_ST7735(CS, DC, RST);

void lcdInt() {
  tft.initR(INITR_BLACKTAB);
  tft2.initR(INITR_BLACKTAB);
  // fill screen
  tft2.fillScreen(BLACK);
  tft.fillScreen(ST7735_BLACK);
  // draw grid
  for (int16_t y = 36; y < 157; y += 12) {
  tft.drawFastHLine(4, y, 120, YELLOW);
  tft2.drawFastHLine(4, y, 120, GREEN);
  }
  for (int16_t x = 4; x < 125; x += 12){
  tft.drawFastVLine(x, 36, 120, YELLOW);
  tft2.drawFastVLine(x, 36, 120, GREEN);
  }
  tft2.fillCircle(g_prevX, g_prevY, 5, ST7735_GREEN);
  delay(150);
}

void joystickInt() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);
  joystick_int_vert = analogRead(VERT);
  joystick_int_horiz = analogRead(HORIZ);
}

void gridInt() {
  int p = 0;
  for (int q = 0; q < 10; q++) {
    grid1[q][p] = 0;
    grid2[q][p] = 0;
    p++;
  }
}

void fontInt() {
  tft.fillRect(1, 20, 120, 10, BLACK);
  tft.setCursor(13,8);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("Please initialze");
  tft.setCursor(32,18);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("your ships!");

  tft2.fillRect(1, 20, 120, 10, BLACK);
  tft2.setCursor(13,8);
  tft2.setTextColor(GREEN, BLACK);
  tft2.print("Please initialze");
  tft2.setCursor(32,18);
  tft2.setTextColor(GREEN, BLACK);
  tft2.print("your ships!");
}

void setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
  fontInt();   // intialzie font on screen
}

// based on response from arduino two, update grid on arduino one
void updateGridPos(bool occupied) {
  // convert pixel position to grid index
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;

  if (screen_order == 0) {
    // if hit, draw orange circle
    if(occupied && grid1[X_coordinate][Y_coordinate] != 1){
      tft.fillCircle(g_prevX, g_prevY, 5, ORANGE);
      grid1[X_coordinate][Y_coordinate] = 5; // hit
    }
    // if miss, draw cross
    else if(!occupied && grid1[X_coordinate][Y_coordinate] != 1){
      tft.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
      tft.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
      grid1[X_coordinate][Y_coordinate] = 8; // miss
    }
  }
  else if (screen_order == 1) {
    // if miss, draw cross
    if(!occupied && grid2[X_coordinate][Y_coordinate] != 1){
      tft2.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft2.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
      tft2.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
      grid2[X_coordinate][Y_coordinate] = 8; // miss
    }
    // if hit, draw orange circle
    else if(occupied && grid2[X_coordinate][Y_coordinate] != 1){
      tft2.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft2.fillCircle(g_prevX, g_prevY, 5, ORANGE);
      grid2[X_coordinate][Y_coordinate] = 5; // hit
    }
  }
  // reset cursor position
  g_cursorX = 58; g_cursorY = 90;
}

// when game ends, display winner
void winner() {
  if (screen_order == 0) {
      tft.fillRect(0, 0, 128, 33, BLACK);
      tft.setCursor(1, 1); tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK);
      tft.print("WINNER!");
      delay(750);
      tft.fillRect(0, 0, 128, 33, BLACK);
      tft.setCursor(1, 1); tft.setTextSize(2);
      tft.setTextColor(BLACK, WHITE);
      tft.print("PLAYER 1");
      delay(750);
      tft.fillRect(0, 0, 128, 33, BLACK);
      tft.setCursor(1, 1); tft.setTextSize(2);
      tft.setTextColor(WHITE, BLACK);
      tft.print("WINNER!");
      delay(750);
      tft.fillRect(0, 0, 128, 33, BLACK);
      tft.setCursor(1, 1); tft.setTextSize(2);
      tft.setTextColor(BLACK, WHITE);
      tft.print("PLAYER 1");
      delay(750);
      tft.setCursor(1, 25); tft.setTextSize(1);
      tft.setTextColor(WHITE, BLACK);
      tft.print("CONGRATULATIONS!");
  }
  if (screen_order == 1) {
      tft2.fillRect(0, 0, 128, 33, BLACK);
      tft2.setCursor(1, 1); tft2.setTextSize(2);
      tft2.setTextColor(WHITE, BLACK);
      tft2.print("WINNER!");
      delay(750);
      tft2.fillRect(0, 0, 128, 33, BLACK);
      tft2.setCursor(1, 1); tft2.setTextSize(2);
      tft2.setTextColor(BLACK, WHITE);
      tft2.print("PLAYER 2");
      delay(750);
      tft2.fillRect(0, 0, 128, 33, BLACK);
      tft2.setCursor(1, 1); tft2.setTextSize(2);
      tft2.setTextColor(WHITE, BLACK);
      tft2.print("WINNER!");
      delay(750);
      tft2.fillRect(0, 0, 128, 33, BLACK);
      tft2.setCursor(1, 1); tft2.setTextSize(2);
      tft2.setTextColor(BLACK, WHITE);
      tft2.print("PLAYER 2");
      delay(750);
      tft2.setCursor(1, 25); tft2.setTextSize(1);
      tft2.setTextColor(WHITE, BLACK);
      tft2.print("CONGRATULATIONS!");
  }
}

// after player turn switches, update font on screen
void fontUpdate() {
  tft.fillRect(1, 1, 130, 34, BLACK);
  tft2.fillRect(1, 1, 130, 34, BLACK);
  if(screen_order == 0) {
    tft.setCursor(28,8);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("Please take");
    tft.setCursor(32,18);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("your move!");
    tft2.setCursor(28,8);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("Please wait");
    tft2.setCursor(18,18);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("for your turn!");
  }
  else if(screen_order == 1) {
    tft2.setCursor(28,8);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("Please take");
    tft2.setCursor(32,18);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("your move!");
    tft.setCursor(28,8);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("Please wait");
    tft.setCursor(18,18);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("for your turn!");
  }
}
