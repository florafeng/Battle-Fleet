#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"
#include "display.h"
#include "cursor.h"
#include "game.h"
#include "select.h"

void Setup_Screen() {
  while(screen_order == 0) {

    updateCursor();
    if ((select == 0) && (screen_order == 0)){
      ship_order++;
      fillGrid();
      delay(250);
    }
    if (ship_order == 5) {
      checkGrid();
      Serial.println(ship_counter);
      if (ship_counter < 17) {
        clearGrid();
        ship_order=0;
        screen_order=0;
        continue;
      }
      checkShips();

      ship_order = 0;
      screen_order = 1;
      break;
    }
  }

  g_cursorX = 58; g_cursorY = 90;
  // Set up Ships on Second Grid
  while(screen_order == 1) {
    updateCursor();
    if ((select == 0) && (screen_order == 1)){
      ship_order++;
      fillGrid();
      delay(250);
    }
    if (ship_order == 5) {
      delay(100);
      checkGrid();
      if (ship_counter < 17) {
        clearGrid();
        screen_order=1;
        ship_order=0;
        continue;
      }
      checkShips();
      screen_order = 2;
      break;
    }
  }
}
