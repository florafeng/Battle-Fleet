#include <Arduino.h>
#include "client.h"
#include "joystick.h"
#include "cursor.h"
#include "display.h"

// avoid cursor moving out of boundary
void checkEdge() {
  if (g_cursorX < 10) { // the left most grid
    g_cursorX = 118;
  }
  if (g_cursorX > 118) { // the right most grid
    g_cursorX = 10;
  }
  if (g_cursorY > 150) { // the bottom most grid
    g_cursorY = 42;
  }
  if (g_cursorY < 42) {  // the top most grid
    g_cursorY = 150;
  }
}

// check position with grid value
uint8_t checkOccupied() {
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;
  // check if current position is occupied
  // 1 -> taken, 0 -> available
  uint8_t gridValue0 = grid1[X_coordinate][Y_coordinate];
  uint8_t gridValue1 = grid2[X_coordinate][Y_coordinate];

  if(screen_order == 0) {
    return gridValue0;
  }
  else if(screen_order == 1) {
    return gridValue1;
  }
}

void cursorMovement() {
  joystickScan();     // read inputs from joystick
  checkEdge();        // avoid moving out of screen
  updateOwnDisplay(); // update cursor position on the current player's screen
}
