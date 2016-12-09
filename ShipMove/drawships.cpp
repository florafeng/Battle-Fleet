#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "grid.h"
#include "config.h"
#include "display.h"


// Checks grid and leaves occupied spaces filled
void CheckShipGrid(int X, int Y) {
  int X0 = X - 6; int X1 = X+5;

  if (screen_order == 0) {
    for (X0; X0<X1; X0++) {
      if ((X0 <1) || (X0 > 10)) {;}
      else if (grid1[X0][Y] != 0) {
        g_prevX = (X0)*12 -2;
        g_prevY = (Y)*12 +30;
        Ship(1);
      }
    }
  }
  if (screen_order == 1) {
    for (X0; X0<X1; X0++) {
      if ((X0 <1) || (X0 > 10)) {;}
      else if (grid2[X0][Y] != 0) {
        g_prevX = (X0)*12 -2;
        g_prevY = (Y)*12 +30;
        Ship(1);
      }
    }
  }
}

// Generate ships (as cursors) for 5 different lengths
void Carrier() {
  Ship(5);
  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);

  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
    Black(5);
    CheckShipGrid(X, Y);

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;
    Ship(5);
  }
}
void Battleship() {
  Ship(4);
  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);

  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
    Black(4);
    CheckShipGrid(X, Y);

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;
    Ship(4);
  }
}
void Cruiser() {
  Ship(3);
  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);

  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
    Black(3);
    CheckShipGrid(X, Y);

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;
    Ship(3);
  }
}
void Destroyer() {
  Ship(2);
  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);

  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
    Black(2);
    CheckShipGrid(X, Y);
    g_prevX = g_cursorX;
    g_prevY = g_cursorY;
    Ship(2);
  }
}
