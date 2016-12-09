#ifndef CLIENT_H
#define CLIENT_H
#include <Arduino.h>

// global variables
extern uint8_t g_cursorX; // cursor pixel position
extern uint8_t g_cursorY;
extern uint8_t g_prevX;   // previous cursor pixel position
extern uint8_t g_prevY;
extern uint8_t grid1[11][11]; // store available position on the screen
extern uint8_t grid2[11][11];
extern uint8_t screen_order;  // current player

/*
 * main function for client
 * useful for a) Wait for ship initialization on arduino two
 *            b) Allow cursor movement on arduino one
 *            c) Determination of game end
 * note: after ships are initialized on arduino two, we go into clientSub function to
 *       allow players place their guesses. More info about this function can be found
 *       in client.cpp file.
 */
void clientMain();

#endif
