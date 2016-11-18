#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define SD_CS 5
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

#define MILLIS_PER_FRAME 50 // 20fps

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int L_Button = 10;
int R_Button = 11;
int Up_Button = 3;
int Down_Button = 4;
int Select_Button = 2;


int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = -1; // previously drawn position of the cursor
int g_prevY = -1;

#define WHITE 0xFFFF

#define ORANGE 0xfa00
uint16_t g_colour = ST7735_GREEN;

const uint16_t MY_YELLOW = tft.Color565(0xff, 0xff, 0x00);

/**
  Scans the joystick for any user input, updates cursor position.
*/
void scanJoystick();

/**
  Draws the cursor.
*/
void updateDisplay();


int main() {
  init();
  Serial.begin(9600);

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

  Serial.println("initialized!");

  tft.fillScreen(ST7735_BLACK);
  int startTime = millis();

  for (int16_t y=36; y<157; y+=12) {
  tft.drawFastHLine(4, y, 120, WHITE);
  }

  for (int16_t x=4; x<125; x+=12){
  tft.drawFastVLine(x, 36, 120, WHITE);
  }
//  for (int16_t x=1)
  while (true) {

    scanJoystick();

    updateDisplay();

    int buttonValueS = digitalRead(Select_Button);

    if (buttonValueS == LOW) {
    Serial.print("Current Position: ");
    Serial.print(g_prevX); Serial.print(" "); Serial.println(g_prevY);
    }
    // create a variable delay, so that each loop iteration takes
    // MILLIS_PER_FRAME milliseconds.
    //
    // int curTime = millis();
    // if (curTime - startTime < MILLIS_PER_FRAME) {
    //   delay(MILLIS_PER_FRAME - (curTime-startTime));
    // }
    // startTime = millis();
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
  }

  if (buttonValueD == LOW) {
//    Serial.println("Down");
    // outside of deadzone
    g_cursorY = (g_cursorY + 12);
  }

  if (buttonValueL == LOW) {
    // outside of deadzone
    g_cursorX = (g_cursorX - 12);

  }

  if (buttonValueR == LOW) {
    // outside of deadzone
    g_cursorX = (g_cursorX + 12);
  }

  if (buttonValueS == LOW) {
     g_colour = ST7735_RED;
   }

  else {
    g_colour = ST7735_GREEN;
  }


  // Serial.print(v); Serial.print(" ");
  // Serial.print(h); Serial.print(" ");
  // Serial.println(select);

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
