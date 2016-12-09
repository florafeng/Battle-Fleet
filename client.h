#ifndef CLIENT_H
#define CLIENT_H
#include <Arduino.h>

extern uint8_t g_cursorX;
extern uint8_t g_cursorY;
extern uint8_t g_prevX;
extern uint8_t g_prevY;
extern uint8_t grid1[11][11];
extern uint8_t grid2[11][11];
extern uint8_t screen_order;

void clientMain();

#endif
