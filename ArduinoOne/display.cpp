#include <Arduino.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library
#include "config.h"
#include "client.h"
#include "joystick.h"
#include "cursor.h"
#include "display.h"

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
void updateFont() {
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

void displayError(uint8_t num) {
  while(true) {
    if(screen_order == 0) {
      tft.fillRect(1, 1, 130, 34, ST7735_BLACK);
      tft.fillCircle(g_prevX, g_prevY, 5, RED);
      delay(300);
      tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
      delay(300);
      joystickScan();
      if(g_cursorX != g_prevX || g_cursorY != g_prevY) {
        tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
        tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
        if(num == 5) {
        tft.fillCircle(g_prevX, g_prevY, 5, ORANGE);
        }
        else if(num == 8) {
          tft.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
          tft.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
        }
        g_prevX = g_cursorX;
        g_prevY = g_cursorY;
        break;
      }
    }
    else if(screen_order == 1) {
      tft2.drawRect(1, 1, 130, 34, ST7735_BLACK); // fill with black first
      tft2.fillCircle(g_prevX, g_prevY, 5, RED);
      delay(300);
      tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
      delay(300);
      joystickScan();
      if(g_cursorX != g_prevX || g_cursorY != g_prevY) {
        tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
        if(num == 5) {
        tft2.fillCircle(g_prevX, g_prevY, 5, ORANGE);
        }
        else if(num == 8) {
          tft2.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
          tft2.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
        }
        g_prevX = g_cursorX;
        g_prevY = g_cursorY;
        break;
      }
    }
  }
}

void updateOwnDisplay() {
  if (screen_order == 0) {
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
      tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
      tft.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
      // fix missing lane
      tft.drawFastVLine(g_prevX-6, g_prevY-6, 12, YELLOW);
      tft.drawFastHLine(g_prevX-6, g_prevY-6, 12, YELLOW);
      // store previous position
      g_prevX = g_cursorX;
      g_prevY = g_cursorY;

      uint8_t val = checkOccupied();
      if(val == 5) {
        displayError(5);
      }
      else if(val == 8) {
        displayError(8);
      }
      else{
        tft.fillCircle(g_prevX, g_prevY, 5, GREEN);
      }
    }
  }
  if (screen_order == 1){
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
      tft2.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
      tft2.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
      // fix missing lane
      tft2.drawFastVLine(g_prevX-6, g_prevY-6, 12, GREEN);
      tft2.drawFastHLine(g_prevX-6, g_prevY-6, 12, GREEN);
      // store previous position
      g_prevX = g_cursorX;
      g_prevY = g_cursorY;

      uint8_t val = checkOccupied();
      if(val == 5) {
        displayError(5);
      }
      else if(val == 8) {
        displayError(8);
      }
      else{
        tft2.fillCircle(g_prevX, g_prevY, 5, YELLOW);
      }
    }
  }
}
