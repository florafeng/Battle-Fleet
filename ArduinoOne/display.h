#ifndef DISPLAY_H
#define DISPLAY_H

/*
 * initial setup for a)LCD screen
 *                   b)Joystick
 *                   c)grid(useful for storing position info)
 */
void setup();

// use response from arduino two, update the grid position
void updateGridPos(bool occupied);
// when end of game is reached, display winner's info
void winner();
// after the switch of player's turn, update font on each screen
void updateFont();
// update screen display for each player
void updateOwnDisplay();

#endif
