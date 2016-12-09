/*
 * Main file to start the game
 * Created by Sida Feng and Chengyi Zhao
 * Please refer to the README file for more details
 */

// library
#include <Arduino.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library
#include "display.h"          // Update lcd display
#include "client.h"           // Communication between arduinos, client side

// LCD screen 1
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8
// LCD screen 2
#define DC 9
#define CS 13
#define RST 12
// LCD shared pin
#define SD_CS 5

int main() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  setup(); // initialize lcd display and joystick

  /*
   * main file for client
   * useful for a) Wait for ship initialization on arduino two
   *            b) Allow cursor movement on arduino one
   *            c) Determination of game end
   */
  clientMain();

  // end of program
  Serial.end();
  Serial3.end();
  return 0;
}
