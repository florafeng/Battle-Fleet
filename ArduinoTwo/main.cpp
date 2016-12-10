#include <Arduino.h>
#include "cursor.h"
#include "display.h"
#include "game.h"
#include "server.h"

int main() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  // Initial complete set up
  Setup();

  // Set up ships on 2 screens
  Setup_Screen();

  // Reset screen order for Active Game Loop
  StartMessage();
  screen_order = 0;

  // Active Game Loop
  while(true) {
    // at this stage, ship initialization is finished, sending complete to arduino one
    Serial3.print("C");
    server();
  }

  return 0;
  Serial3.end();
  Serial.end();
}
