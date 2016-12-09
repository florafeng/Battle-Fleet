#include <Arduino.h>
#include "client.h"
#include "joystick.h"
#include "cursor.h"
#include "display.h"

// avoid cursor moving out of boundary
void checkEdge() {
  if (g_cursorY > 150) {
    g_cursorY = 42;
  }
  if (g_cursorY < 42) {
    g_cursorY = 150;
  }
  if (g_cursorX < 10) {
    g_cursorX = 118;
  }
  if (g_cursorX > 118) {
    g_cursorX = 10;
  }
}

// check position with grid value
uint8_t checkOccupied() {
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;
  // check if current position if occupied
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
  joystickScan();
  checkEdge();
  updateOwnDisplay();
}
