/*
 * configuration of pin and global variable
 */
#ifndef CONFIG_H
#define CONFIG_H

// standard U of A library settings, assuming Atmel Mega SPI pins

#define SD_CS 5 // Shared Pin for SD card

// Screen 1 Pins for TFT; Display, Data/Command Line, Reset Line
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8

// Screen 2 Pins for TFT; Display, Data/Command Line, Reset Line
#define DC 9
#define CS 13
#define RST 12

// LCD display configuration
#define TFT_WIDTH 128
#define TFT_HEIGHT 160

//Colours to be used
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE   0xfa00

//Joystick and Select Button Pins
#define VERT = 0;
#define HORIZ = 1;
#define SEL = 3;

//Initialize Global (Position) Variables
int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = 58; // previously drawn position of the cursor
int g_prevY = 90;
int X_coordinate = 0; // Grid coordinate position
int Y_coordinate = 0;
int grid1[11][11]; // Two Game Grids (using X, Y from 1 to 10)
int grid2[11][11];

//Initialize Global Counters
int ship_counter = 0;
int screen_order = 0;
int ship_order = 0;
int ships_down = 0;
int ships_remaining = 5;

#endif
