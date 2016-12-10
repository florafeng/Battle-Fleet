/* Controls all cursor/joystick variables,
including edge cases and select button
*/

#include <Arduino.h>
#include "config.h"
#include "cursor.h"

//Joystick Variables
int joystick_int_vert;
int joystick_int_horiz;

int select; // joystick button status, pressed -> LOW

//Global Position Variables
uint8_t g_cursorX = 58; // cursor pixel position
uint8_t g_cursorY = 90;
uint8_t g_prevX = -1;   // previous cursor pixel position
uint8_t g_prevY = -1;

//Global Counter Varialbes
int screen_order = 0;
int ship_order = 0;

// initialize joystick
void joystickInt() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);
  joystick_int_vert = analogRead(VERT);
  joystick_int_horiz = analogRead(HORIZ);
}

// Read Joystick
void joystickScan() {
  int joystick_vert = analogRead(VERT);
  int joystick_horiz = analogRead(HORIZ);

  if (joystick_vert >= 800) {
    g_cursorY += 12;
  }
  else if (joystick_vert <= 200) {
    g_cursorY -= 12;
  }
  else if (joystick_horiz <= 200) {
    g_cursorX -= 12;
  }
  else if (joystick_horiz >= 800) {
    g_cursorX += 12;
  }
}

// Edge Cases
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

// Implement Edge Cases
void checkEdges() {
  if (screen_order == 0) {
      ShipEdgeCases();
  }
  if (screen_order == 1) {
      ShipEdgeCases();
  }
}

// Full cursor implementation
void cursorMovement() {
  joystickScan();     // read inputs from joystick
  checkEdges();        // avoid moving out of screen
}
