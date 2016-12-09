#ifndef JOYSTICK_H
#define JOYSTICK_H

void joystickInt();  // initialize joystick settings
void joystickScan(); // use joystick input to control cursor position

extern int select;   // joystick button status, pressed -> LOW

#endif
