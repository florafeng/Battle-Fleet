#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define SD_CS 5
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8
#define DC 9

#define CS 13
#define RST 12

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

#define MILLIS_PER_FRAME 50 // 20fps

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7735 tft2 = Adafruit_ST7735(CS, DC, RST);

int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = -1; // previously drawn position of the cursor
int g_prevY = -1;

int h_cursorX = 58;
int h_cursorY = 90;
int h_prevX = 58;
int h_prevY = 90;

int grid1[10][10];
int grid2[10][10];
int screen_order = 0;

#define WHITE 0xFFFF
#define BLACK 0x0000
#define ORANGE 0xfa00
#define GREEN 0x07E0
uint16_t g_colour = ST7735_GREEN;
uint16_t h_colour = ST7735_GREEN;

const uint16_t MY_YELLOW = tft.Color565(0xff, 0xff, 0x00);

// Joystick
const int VERT = 0;  // analog input
const int HORIZ = 1; // analog input
const int SEL = 3;   // digital input
int joystick_int_vert, joystick_int_horiz;
int select;

bool player1() {
  bool player1 = false;
  if (screen_order == 0) {
    player1 = true;
  }
  return player1;
}

// bool player2() {
//   bool player2 = false;
//   if (!player1) {
//     player2 = true;
//   }
//   return player2;
// }

void updateOwnDisplay() {
  if (player1) {
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
      tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
      tft.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
      // fix missing lane
      tft.drawFastVLine(g_prevX-6, g_prevY-6, 12, WHITE);
      tft.drawFastHLine(g_prevX-6, g_prevY-6, 12, WHITE);
      // store previous position
      g_prevX = g_cursorX;
      g_prevY = g_cursorY;
      tft.fillCircle(g_prevX, g_prevY, 5, g_colour);
    }
  }
  if (screen_order == 1){
    Serial.println("in!");
    if((h_prevX != h_cursorX) || (h_prevY != h_cursorY) ) {
      tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_BLACK);
      tft2.drawRect(h_prevX -6, h_prevY -6, 12, 12, ST7735_BLACK);
      tft2.drawFastVLine(h_prevX-6, h_prevY-6, 12, ST7735_GREEN);
      tft2.drawFastHLine(h_prevX-6, h_prevY-6, 12, ST7735_GREEN);
      h_prevX = h_cursorX;
      h_prevY = h_cursorY;
      tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
    }
  }
}

void updateOtherDisplay() {
  int X_coordinate, Y_coordinate;
  if (player1) {
    Serial.println("Current Position: ");
    X_coordinate = (g_prevX-2)/12;
    Y_coordinate = (g_prevY-30)/12-1;
    grid2[X_coordinate][Y_coordinate] = 1;

    h_prevX = g_prevX;
    h_prevY = g_prevY;
    tft2.drawCircle(h_prevX, h_prevY, 5, g_colour);
  }else {
    Serial.println("player2 ");
    X_coordinate = (g_prevX-2)/12;
    Y_coordinate = (g_prevY-30)/12-1;
  }
}

void lcdInt() {
  tft.initR(INITR_BLACKTAB);
  tft2.initR(INITR_BLACKTAB);
  // fill screen
  tft2.fillScreen(BLACK);
  tft.fillScreen(ST7735_BLACK);
  // draw grid
  for (int16_t y = 36; y < 157; y += 12) {
  tft.drawFastHLine(4, y, 120, WHITE);
  tft2.drawFastHLine(4, y, 120, GREEN);
  }
  for (int16_t x = 4; x < 125; x += 12){
  tft.drawFastVLine(x, 36, 120, WHITE);
  tft2.drawFastVLine(x, 36, 120, GREEN);
  }
  tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
  delay(150);
}

void joystickInt() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);
  joystick_int_vert = analogRead(VERT);
  joystick_int_horiz = analogRead(HORIZ);
}

void gridInt() {
  int p = 0;
  for (int q = 0; q < 10; q++) {
    grid1[q][p] = 0;
    grid2[q][p] = 0;
    p++;
  }
}

void setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
}

void checkEdge() {
  if (screen_order == 0) {
    if (g_cursorY > 150) {
      g_cursorY = 42;
    }
    if (g_cursorY < 42) {
      g_cursorY = 150;
    }
    if (g_cursorX <10) {
      g_cursorX = 118;
    }
    if (g_cursorX > 118) {
      g_cursorX = 10;
    }
  }
  else if (screen_order == 1) {
    if (h_cursorY > 150) {
      h_cursorY = 42;
    }
    if (h_cursorY < 42) {
      h_cursorY = 150;
    }
    if (h_cursorX <10) {
      h_cursorX = 118;
    }
    if (h_cursorX > 118) {
      h_cursorX = 10;
    }
  }
}

void corsorMovement() {
 int joystick_vert = analogRead(VERT);
 int joystick_horiz = analogRead(HORIZ);

 if (screen_order == 0) {
   if (joystick_vert >= 700) {
     g_cursorY += 12;
   }
   else if (joystick_vert <= 300) {
     g_cursorY -= 12;
   }
   else if (joystick_horiz <= 300) {
     g_cursorX -= 12;
   }
   else if (joystick_horiz >= 700) {
     g_cursorX += 12;
   }
 }
 else if (screen_order == 1)  {
   if (joystick_vert >= 700) {
     h_cursorY += 12;
   }
   else if (joystick_vert <= 300) {
     h_cursorY -= 12;
   }
   else if (joystick_horiz <= 300) {
     h_cursorX -= 12;
   }
   else if (joystick_horiz >= 700) {
     h_cursorX += 12;
   }
 }

  checkEdge();
  updateOwnDisplay();
  delay(250);
}

void switchTurn() {
  if (screen_order == 0) {
    screen_order += 1;
  }
  else if (screen_order == 1) {
    screen_order -= 1;
  }
}



int main() {
  init();
  Serial.begin(9600);
  setup();

  int startTime = millis();

  while (true) {
    // bool bo = player1();
    // Serial.print("player1: "); Serial.println(bo);
    corsorMovement();

    select = digitalRead(SEL);
    if (select == LOW) {
      updateOtherDisplay();
      switchTurn();
      Serial.print("screen: ");Serial.println(screen_order);
    }
   }

  Serial.end();
  return 0;
}
