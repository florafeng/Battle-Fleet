#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "config.h"

void gridInt();
void Setup();
void checkGrid();
void getCoordinates();
void RegisterGrid();
void fillGrid();
void updateCursor();
void Setup_Screen();

extern int grid1[11][11]; // Two Game Grids (using X, Y from 1 to 10)
extern int grid2[11][11];
extern int ship_counter;


#endif
