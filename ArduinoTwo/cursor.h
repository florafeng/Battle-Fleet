#ifndef CURSOR_H
#define CURSOR_H

void joystickInt();
void ShipEdgeCases();
void checkEdges();
void cursorMovement();

extern int select;   // joystick button status, pressed = 0/LOW
extern uint8_t g_cursorX; // cursor pixel position
extern uint8_t g_cursorY;
extern uint8_t g_prevX;   // previous cursor pixel position
extern uint8_t g_prevY;
extern int screen_order;
extern int ship_order;
#endif
