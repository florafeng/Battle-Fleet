#include <Arduino.h>
#include "config.h"
#include "joystick.h"

// constructor
// Joystick::Joystick(int joystick_horiz, int joystick_vert) {
//     joystick_vert = joystick_vert;
//     joystick_horiz = joystick_horiz;
// }

//Movement Along the grid in increments of 12 Pixels
//Edge Case for each specific ship based on Cursor position
void ShipEdgeCases() {
    if (g_cursorY > 150) {g_cursorY = 42;}
    if (g_cursorY < 42) {g_cursorY = 150;}

    if (ship_order == 0){
        if (g_cursorX <34) {g_cursorX = 94;}
        if (g_cursorX > 94) {g_cursorX = 34;}
    }

    if (ship_order == 1){
        if (g_cursorX <34) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 34;}
    }

    if ((ship_order == 2) || (ship_order == 3)){
        if (g_cursorX <22) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 22;}
    }
    if (ship_order == 4){
        if (g_cursorX <10) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 10;}
    }
}

void checkEdges() {
  if (screen_order == 0) {
      ShipEdgeCases();
  }
  if (screen_order == 1) {
      ShipEdgeCases();
  }
}

void cursorMovement() {
  joystick_vert = analogRead(VERT);
  joystick_horiz = analogRead(HORIZ);

    if (joystick_vert >= 800) {
      g_cursorY += 12;}
    else if (joystick_vert <= 200) {
      g_cursorY -= 12;}
    else if (joystick_horiz <= 200) {
      g_cursorX -= 12;}
    else if (joystick_horiz >= 800) {
      g_cursorX += 12;}
}
