#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void lcdint();
void StartMessage();
void checkGrid();
void checkShips();
void Black(); // draw a zone to display game status
void Ship(); // display current score
void Winner();
void updateShipGrid();
void clearGrid();
void SetupMessage();
void setupComplete();


#endif
