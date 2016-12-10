/* All game related functions, mainly grid functions, cursor movement, 
and main Setup loops
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"
#include "cursor.h"
#include "display.h"
#include "drawships.h"
#include "game.h"


int grid1[11][11]; // Two Game Grids (using X, Y from 1 to 10)
int grid2[11][11];

// Global Counter Variable for tracking grid positions occupied
int ship_counter = 0;


//Fills Grids with 0s (Empty Arrays)
void gridInt() {
    for (int q = 0; q <= 10; q++) {
          for (int p = 0; p<=10; p++) {
                grid1[q][p] = 0;
                grid2[q][p] = 0;
          }
    }
}

//Full Setup with Confirmation Display Message
void Setup() {
    lcdInt();    // fill screen, draw grid and intial cursor
    joystickInt();
    gridInt();   // initialze position array
    SetupMessage();
}

//Based on Grid, returns 2 counters for tracking ships on Grid
void checkGrid() {
      ships_down = 0; ships_remaining = 5;
      ship_counter = 0;
      int counter5 = 5;  int counter4 = 4;
      int counter3 = 3;  int counter3_2 = 3;
      int counter2 = 2;

      for (int y0=0; y0<=10; y0++) {
          for (int x0 =0; x0 <=10; x0++) {
              if (screen_order == 0) {
                  if (grid1[x0][y0] == 5) { counter5 = counter5 - 1; ship_counter++;}
                  if (grid1[x0][y0] == 4) { counter4 = counter4 - 1; ship_counter++;}
                  if (grid1[x0][y0] == 3) { counter3 = counter3 - 1; ship_counter++;}
                  if (grid1[x0][y0] == 1) { counter3_2 = counter3_2 - 1; ship_counter++;}
                  if (grid1[x0][y0] == 2) { counter2 = counter2 - 1; ship_counter++;}
              }
              if (screen_order == 1) {
                  if (grid2[x0][y0] == 5) { counter5 = counter5 - 1; ship_counter++;}
                  if (grid2[x0][y0] == 4) { counter4 = counter4 - 1; ship_counter++;}
                  if (grid2[x0][y0] == 3) { counter3 = counter3 - 1; ship_counter++;}
                  if (grid2[x0][y0] == 1) { counter3_2 = counter3_2 - 1; ship_counter++;}
                  if (grid2[x0][y0] == 2) { counter2 = counter2 - 1; ship_counter++;}
              }
          }
      }
      if (counter5 == 5) { ships_remaining--; ships_down++;}
      if (counter4 == 4) { ships_remaining--; ships_down++;}
      if (counter3 == 3) { ships_remaining-- ;ships_down++;}
      if (counter3_2 == 3) { ships_remaining--; ships_down++;}
      if (counter2 == 2) { ships_remaining--; ships_down++;}
      // Serial.println(ships_remaining);
      //
      Serial.println(ships_down);
}


//Obtain XY grid coordinates based on current position
void getCoordinates() {
    X_coordinate = (g_prevX+2)/12;  Y_coordinate = (g_prevY-30)/12;
}

//Registers Ships to grid based on screen_order, position, and length
void RegisterGrid(int X_coordinate, int Y_coordinate, int Registry) {
    int X, X_upper;
    if (ship_order <= 2) {X = X_coordinate -2; X_upper = X_coordinate +2;}
    else if ((ship_order >2) && (ship_order <5)) {X = X_coordinate -1;}
    else if (ship_order == 5) {X = X_coordinate;}

    if (ship_order != 1) {X_upper = X_coordinate +1;}
    while(X <= X_upper) {
        if (screen_order == 0)  {grid1[X][Y_coordinate] = Registry;}
        else if (screen_order == 1){grid2[X][Y_coordinate] = Registry;}
        X = X++;
    }
}

// Registers each ship onto grid using RegisterGrid
void fillGrid() {
    if (ship_order == 1) {
        Ship(5);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 5);
    }
    if (ship_order == 2) {
        Ship(4);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 4);
    }
    if (ship_order == 3) { // 3 Tile ship
        Ship(3);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 3);
    }
    if (ship_order == 4) { // 3 Tile Ship
        Ship(3);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 1);
    }
    if (ship_order == 5) {
        Ship(2);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 2);
    }
}

// Cursor for Setup Screen with Ship as cursor
void updateCursor() {
  cursorMovement();
  delay(250);
  if (ship_order == 0) {Carrier();}
  if (ship_order == 1) {Battleship();}
  if ((ship_order == 2) || (ship_order == 3)) {Cruiser();}
  if (ship_order == 4) {Destroyer();}
}

// Main loop for setting up both screens with 5 ships
void Setup_Screen() {
  while(screen_order == 0) {
    select = digitalRead(SEL);
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
      setupComplete();
      checkShips();
      ship_order = 0;
      screen_order = 1;
      break;
    }
  }
  g_cursorX = 58; g_cursorY = 90;
  // Set up Ships on Second Grid
  while(screen_order == 1) {
    select = digitalRead(SEL);
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
      setupComplete();
      delay(250);
      StartMessage();
      checkShips();
      screen_order = 2;
      break;
    }
  }
}
