// All TFT drawings and functions.

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"
#include "cursor.h"
#include "game.h"
#include "server.h"
#include "display.h"
// #include "select.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7735 tft2 = Adafruit_ST7735(CS, DC, RST);

// Global Counter Variables for Ships on Grid
int ships_down = 0;
int ships_remaining = 5;

// Global Position Variables based on grid
int X_coordinate = 0;
int Y_coordinate = 0;

//Initiates 2 LCDs with 10x10 grids of color
void lcdInt() {
  tft.initR(INITR_BLACKTAB);
  tft2.initR(INITR_BLACKTAB);

  tft2.fillScreen(BLACK);
  tft.fillScreen(BLACK);

  //Draw 10x10 Grids of Yellow and Green Lines, respectively
  for (int16_t y = 36; y < 157; y += 12) {
    tft.drawFastHLine(4, y, 120, YELLOW);
    tft2.drawFastHLine(4, y, 120, GREEN);
  }
  for (int16_t x = 4; x < 125; x += 12){
    tft.drawFastVLine(x, 36, 120, YELLOW);
    tft2.drawFastVLine(x, 36, 120, GREEN);
  }
  delay(150);
}

// Message just before Game loop
void StartMessage() {
  tft.fillRect(0, 0, 128, 33, BLACK);
  tft.setCursor(1, 20); tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.println("GAME START!");
  tft2.fillRect(0, 0, 128, 33, BLACK);
  tft2.setCursor(1, 20); tft2.setTextSize(1);
  tft2.setTextColor(WHITE, BLACK);
  tft2.println("GAME START!");
  delay(500);
}

// Active Scoreboard tracking Ships on grid
void checkShips() {
  checkGrid();
  if (screen_order == 0) {
    tft.fillRect(0, 1, 128, 33, BLACK);
    tft.setCursor(4, 1); tft.setTextSize(1);
    tft.setTextColor(GREEN, BLACK);
    tft.print("Ships Remaining: "); tft.println(ships_remaining);
    tft.println(); tft.setCursor(4, 20);
    tft.print("Ships Down: "); tft.print(ships_down);
  }
  if (screen_order == 1) {
    tft2.fillRect(0, 1, 128, 33, BLACK);
    tft2.setCursor(4, 1);
    tft2.setTextColor(YELLOW, BLACK);
    tft2.print("Ships Remaining: "); tft2.println(ships_remaining);
    tft2.println(); tft2.setCursor(4, 20);
    tft2.print("Ships Down: "); tft2.print(ships_down);
  }
}

//Remove Ship of (length) based on current grid position
void Black(int length) {
  //Actual Cursor always at C
  int start = g_prevX -4 +((length-2)*-12);
  if (length == 1) {start = g_prevX -4; }
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  int end = g_prevX + 20;
  if(length != 5) {end = g_prevX + 16;}
  if (length ==1) {end = g_prevX +7;}

  for (start; start<= end; start+=12) {
    if (screen_order == 0) {
      tft.fillRect(start, g_prevY-4, 10, 10, BLACK);
    }
    if(screen_order == 1) {
      tft2.fillRect(start, g_prevY-4, 10, 10, BLACK);
    }
  }
}

//Create Ship of (length) based on current grid position
void Ship(int length) {
  int start = g_prevX -4 +((length-2)*-12);
  if (length == 1) {start = g_prevX -4; }
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  int end = g_prevX + 20;
  if(length != 5) {end = g_prevX + 16;}
  if (length ==1) {end = g_prevX +7;}

  for (start; start<= end; start+=12) {
    if (screen_order == 0) {
      tft.fillRect(start, g_prevY-4, 9, 9, WHITE);
    }
    if(screen_order == 1) {
      tft2.fillRect(start, g_prevY-4, 9, 9, WHITE);
    }
  }
}


//Winning Messages based on Player who wins
void Winner() {
  if (screen_order == 0) {
    tft2.fillRect(0, 0, 128, 33, BLACK);
    tft2.setCursor(1, 1); tft2.setTextSize(2);
    tft2.setTextColor(WHITE, BLACK);
    tft2.print("GAME OVER!");
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
    tft.fillRect(0, 0, 128, 33, BLACK);
    tft.setCursor(1, 1); tft.setTextSize(2);
    tft.setTextColor(WHITE, BLACK);
    tft.print("GAME OVER!");
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

// Message after Screens are Set up
void setupComplete() {
  if (screen_order ==0) {
  tft.fillRect(0, 0, 128, 25, BLACK);
  tft.setCursor(5, 20); tft.setTextSize(1);
  tft.setTextColor(BLACK, WHITE);
  tft.print("Setup Complete!");
  }
  if (screen_order == 1) {
  tft2.fillRect(0, 0, 128, 33, BLACK);
  tft2.setCursor(5, 20);
  tft2.setTextColor(BLACK, WHITE);
  tft2.print("Setup Complete!");
  }
}

//Game Mode: Update screens based on Serial Communication
void updateShipGrid(uint8_t posX, uint8_t posY, uint8_t res) {
  uint8_t pixelX = posX*12 - 2;
  uint8_t pixelY = posY*12 + 30;

  if (screen_order == 0) {
    // Serial.println("screen1 update grid");
    if(res == 0){ //miss
      grid1[X_coordinate][Y_coordinate] = 10; // occupied with guessed position
      tft.drawLine(pixelX-6, pixelY -6, pixelX+6, pixelY +6, WHITE);
      tft.drawLine(pixelX-6, pixelY +6, pixelX+6, pixelY -6, WHITE);
      delay(150);
    }
    else if(res != 0) { //hit
      grid1[X_coordinate][Y_coordinate] = 11; // occupied with guessed position
      tft.fillRect(pixelX -6, pixelY -6, 13, 13, BLACK);
      tft.drawRect(pixelX -6, pixelY -6, 13, 13, YELLOW);
      tft.fillCircle(pixelX, pixelY, 5, ORANGE);
      delay(150);
    }
  }
  else if (screen_order == 1) {
    if(res == 0){ //miss
      grid2[X_coordinate][Y_coordinate] = 10;
      tft2.drawLine(pixelX-6, pixelY -6, pixelX+6, pixelY +6, WHITE);
      tft2.drawLine(pixelX-6, pixelY +6, pixelX+6, pixelY -6, WHITE);
      delay(150);
    }
    else if(res != 0) { //hit
      grid2[X_coordinate][Y_coordinate] = 11; // occupied with guessed position
      tft2.fillRect(pixelX -6, pixelY -6, 13, 13, BLACK);
      tft2.drawRect(pixelX -6, pixelY -6, 13, 13, GREEN);
      tft2.fillCircle(pixelX, pixelY, 5, ORANGE);
      delay(150);
    }
  }
}

// Function to clear grid, with error message
void clearGrid() {
  gridInt();
  g_prevX = 10; g_prevY = 42;
  for (g_prevY; g_prevY<=150; g_prevY+= 12) {
    for (g_prevX = 10; g_prevX<= 118; g_prevX+=12) {
      Black(1);
    }
  }
  if (screen_order == 0) {
    tft.fillRect(0, 0, 128, 35, BLACK);
    tft.setCursor(5, 5);
    tft.setTextColor(WHITE);
    tft.print("Invalid: Please Try Again");
    delay(1000);
    tft.fillRect(0, 0, 128, 35, BLACK);
  }
  if (screen_order == 1) {
    tft2.fillRect(0, 0, 128, 35, BLACK);
    tft2.setCursor(5, 5);
    tft2.setTextColor(WHITE);
    tft2.print("Invalid: Please Try Again");
    delay(1000);
    tft2.fillRect(0, 0, 128, 35, BLACK);
  }
}

// Welcome message after initial set up is completed
void SetupMessage() {
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(WHITE, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(GREEN, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(GREEN, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(YELLOW, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 33, BLACK);
  tft.setCursor(1, 20); tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.println("Please Set Your Ships");
  tft2.fillRect(0, 0, 128, 33, BLACK);
}
