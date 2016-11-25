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

int L_Button = 10;
int R_Button = 11;
int Up_Button = 3;
int Down_Button = 4;
int Select_Button = 2;


int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = -1; // previously drawn position of the cursor
int g_prevY = -1;

int h_cursorX = 58;
int h_cursorY = 90;
int h_prevX = -1;
int h_prevY = -1;

int Screen_order =0;

#define WHITE 0xFFFF
#define BLACK 0x0000
#define ORANGE 0xfa00
#define GREEN 0x07E0
uint16_t g_colour = ST7735_GREEN;
uint16_t h_colour = ST7735_GREEN;

const uint16_t MY_YELLOW = tft.Color565(0xff, 0xff, 0x00);

/**
  Scans the joystick for any user input, updates cursor position.
*/
void scanJoystick();

/**
  Draws the cursor.
*/

void Setup();
void CheckEdges();

void updateDisplay();
void updateDisplay2();

int main() {
  init();
  Serial.begin(9600);
  Setup();
  
  tft2.fillScreen(BLACK);
  tft.fillScreen(ST7735_BLACK);


  int startTime = millis();

  for (int16_t y=36; y<157; y+=12) {
  tft.drawFastHLine(4, y, 120, WHITE);
  tft2.drawFastHLine(4, y, 120, GREEN);  
  }

  for (int16_t x=4; x<125; x+=12){
  tft.drawFastVLine(x, 36, 120, WHITE);
  tft2.drawFastVLine(x, 36, 120, GREEN);
  }

  int Grid[10][10];
  int q; int p=0;
  for (q=0; q<10; q++) {
    Grid[q][p] = 0;
    p++;
  }

  while (true) {

    scanJoystick();

    CheckEdges();

    updateDisplay();

    int buttonValueS = digitalRead(Select_Button);

    if ((buttonValueS == LOW) && (Screen_order == 0)) {
    Screen_order = Screen_order +1;
    }
    if ((buttonValueS == LOW) && (Screen_order == 1)) {
    Screen_order = Screen_order -1;
    }
//if ((buttonValueS
    int X_coordinate = 0;
    int Y_coordinate = 0;
  if (buttonValueS == LOW) {
  Serial.print("Current Position: ");
    
    X_coordinate = (g_prevX-2)/12;

    Y_coordinate = (g_prevY-30)/12 - 1;

    Grid[X_coordinate][Y_coordinate] = 1;

    h_prevX = g_prevX; h_prevY = g_prevY;
updateDisplay2();    
}


    // create a variable delay, so that each loop iteration takes
    // MILLIS_PER_FRAME milliseconds.
    //
    // int curTime = millis();
    // if (curTime - startTime < MILLIS_PER_FRAME) {
    //   delay(MILLIS_PER_FRAME - (curTime-startTime));
    // }
    // startTime = millis()
   }

  Serial.end();
  return 0;
}

void scanJoystick() {


   int buttonValueU = digitalRead(Up_Button);
   int buttonValueD = digitalRead(Down_Button);
   int buttonValueL = digitalRead(L_Button);
   int buttonValueR = digitalRead(R_Button);
   int buttonValueS = digitalRead(Select_Button);

  // update g_cursorX & g_cursorY

  if (buttonValueU == LOW) {
    // outside of deadzone
  //  Serial.println("Up");
    g_cursorY = (g_cursorY - 12);

    // if (g_prevY < 48) {
    //   g_cursorY = 150;
    // }
  }

  if (buttonValueD == LOW) {
//    Serial.println("Down");
    // outside of deadzone
    g_cursorY = (g_cursorY + 12);

    // if (g_prevY > 144) {
    //   g_cursorY = 42;
    // }
  }

  if (buttonValueL == LOW) {
    // outside of deadzone
    g_cursorX = (g_cursorX - 12);

    // if (g_prevX > 16) {
    //   g_cursorX = 118;
    // }
  }

  if (buttonValueR == LOW) {
    // outside of deadzone
    g_cursorX = (g_cursorX + 12);

    // if (g_prevX < 112) {
    //   g_cursorX = 10;
    // }
  }

  else {
    g_colour = ST7735_GREEN;
  }


  // Serial.print(v); Serial.print(" ");
  // Serial.print(h); Serial.print(" ");
  // Serial.println(select);

}

void CheckEdges() {
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
void updateDisplay() {
  // tft.fillScreen(ST7735_BLACK); // BAD!

  tft.fillCircle(g_prevX, g_prevY, 5, g_colour);

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
  tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);

  tft.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);

  tft.drawFastVLine(g_prevX-6, g_prevY-6, 12, WHITE);
  tft.drawFastHLine(g_prevX-6, g_prevY -6, 12, WHITE);


  // tft.drawRect(g_prevX -6, g_prevY - 6, 12, 12, WHITE);
  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

  tft.fillCircle(g_prevX, g_prevY, 5, g_colour);
}
  // tft.drawLine(g_prevX-5, g_prevY, g_prevX+5, g_prevY, ST7735_BLACK);
  // tft.drawLine(g_prevX, g_prevY-5, g_prevX, g_prevY+5, ST7735_BLACK);
  // g_prevX = g_cursorX;
  // g_prevY = g_cursorY;
  // tft.drawLine(g_prevX-5, g_prevY, g_prevX+5, g_prevY, g_colour);
  // tft.drawLine(g_prevX, g_prevY-5, g_prevX, g_prevY+5, g_colour);
}

void updateDisplay2() {
  // tft.fillScreen(ST7735_BLACK); // BAD!

  tft2.drawCircle(h_prevX, h_prevY, 5, g_colour);

}
  // tft.drawLine(g_prevX-5, g_prevY, g_prevX+5, g_prevY, ST7735_BLACK);
  // tft.drawLine(g_prevX, g_prevY-5, g_prevX, g_prevY+5, ST7735_BLACK);
  // g_prevX = g_cursorX;
  // g_prevY = g_cursorY;
  // tft.drawLine(g_prevX-5, g_prevY, g_prevX+5, g_prevY, g_colour);
  // tft.drawLine(g_prevX, g_prevY-5, g_prevX, g_prevY+5, g_colour);

void Setup() {
    pinMode(Up_Button, INPUT);
  pinMode(Down_Button, INPUT);
  pinMode(L_Button, INPUT);
  pinMode(R_Button, INPUT);
  pinMode(Select_Button, INPUT);
  digitalWrite(Up_Button, HIGH);
  digitalWrite(Down_Button, HIGH);
  digitalWrite(L_Button, HIGH);
  digitalWrite(R_Button, HIGH);
  digitalWrite(Select_Button, HIGH);

  // Init TFT
  tft.initR(INITR_BLACKTAB);
  // Init joystick
  tft2.initR(INITR_BLACKTAB);

  Serial.println("initialized!");
}
