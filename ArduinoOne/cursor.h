#ifndef CURSOR_H
#define CURSOR_H
#include <Arduino.h>

void cursorMovement(); // allow cursor movement for each player
uint8_t checkOccupied(); // check current position with arduino two, return this availability of this position

#endif
