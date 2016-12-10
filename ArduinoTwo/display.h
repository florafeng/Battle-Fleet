#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void lcdInt();
void StartMessage();
void checkShips();
void Black(int length);
void Ship(int length);
void Winner();
void updateShipGrid(uint8_t posX, uint8_t posY, uint8_t res);
void clearGrid();
void SetupMessage();
void setupComplete();

extern int ships_down;
extern int ships_remaining;
extern int X_coordinate;
extern int Y_coordinate;
#endif
