#include <Arduino.h>
#include "config.h"
#include "joystick.h"
#include "client.h"

int joystick_int_vert, joystick_int_horiz; // joystick initialization
int select; // joystick button status, pressed -> LOW

// initialize joystick
void joystickInt() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);
  joystick_int_vert = analogRead(VERT);
  joystick_int_horiz = analogRead(HORIZ);
}

// allow movement with joystick
void joystickScan() {
  // read from joystick pins
  int joystick_vert = analogRead(VERT);
  int joystick_horiz = analogRead(HORIZ);
  // change cursor position
  if (joystick_vert >= 700) {
    g_cursorY += 12; // one grid to the bottom
  }
  else if (joystick_vert <= 300) {
    g_cursorY -= 12; // one grid to the top
  }
  else if (joystick_horiz <= 300) {
    g_cursorX -= 12; // one grid to the left
  }
  else if (joystick_horiz >= 700) {
    g_cursorX += 12; // one grid to the right
  }
  delay(150);
}
