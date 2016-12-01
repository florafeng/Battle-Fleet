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
int buttonValueS=1;

int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = -1; // previously drawn position of the cursor
int g_prevY = -1;

int h_cursorX = 58;
int h_cursorY = 90;
int h_prevX = -1;
int h_prevY = -1;

int Screen_order =0;
int Grid[10][10];

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE 0xfa00
#define GREEN 0x07E0
uint16_t g_colour = GREEN;


void scanJoystick();

void Setup();

void drawGrid();
void Carrier( int i);
void Battleship();
void Cruiser( int r);
void Destroyer(int w);
void drawX();

int main() {
  init();
  Serial.begin(9600);
  Setup();

  tft.fillScreen(ST7735_BLACK);
  int startTime = millis();

  drawGrid();
  int q; int p=0;
  for (q=0; q<10; q++) {
    Grid[q][p] = 0;
    p++;
  }

  Battleship();
while(true) {

  buttonValueS = digitalRead(Select_Button);

if (buttonValueS == 0) {


// g_cursorX = g_cursorX - 6;
  tft.fillRect(g_cursorX-16, g_cursorY-4, 9, 9, ST7735_BLACK);
  tft.fillRect(g_cursorX-4, g_cursorY-4, 9, 9, ST7735_BLACK);
  tft.fillRect(g_cursorX+8, g_cursorY-4, 9, 9, ST7735_BLACK);

}
//  while(true) {

    //scanJoystick();

}

return 0;
Serial.end();
}

void scanJoystick() {

   int buttonValueU = digitalRead(Up_Button);
   int buttonValueD = digitalRead(Down_Button);
   int buttonValueL = digitalRead(L_Button);
   int buttonValueR = digitalRead(R_Button);

  // update g_cursorX & g_cursorY

  if (buttonValueU == LOW) {
    g_cursorY = (g_cursorY - 12);
  }
  if (buttonValueD == LOW) {
    g_cursorY = (g_cursorY + 12);
  }
  if (buttonValueL == LOW) {
    g_cursorX = (g_cursorX - 12);
  }
  if (buttonValueR == LOW) {
    g_cursorX = (g_cursorX + 12);
  }
  else {
    g_colour = ST7735_GREEN;
  }
}

void Carrier( int i) {
  int c = i+49;
  for (i; i< c; i+=12) {
      g_cursorX = i;
      tft.fillRect(g_cursorX-28, g_cursorY-4, 9, 9, WHITE);
  }
}

void Battleship() {

//  tft.fillTriangle(g_cursorX-28, g_cursorY-4, 9, 9, WHITE);
  tft.fillTriangle(g_cursorX +8, g_cursorY-4, g_cursorX+8, g_cursorY+4, g_cursorX+16, g_cursorY, WHITE);
  tft.fillRect(g_cursorX-4, g_cursorY-4, 9, 9, WHITE);
  tft.fillRect(g_cursorX - 11, g_cursorY-4, 4, 9, WHITE);
  tft.fillTriangle(g_cursorX - 12, g_cursorY-4, g_cursorX-12, g_cursorY+4, g_cursorX-16, g_cursorY, WHITE);

  // Serial.println(buttonValueS);

  // if(buttonValueS !=1) {
  //   g_cursorY = j;
  //
  //   tft.fillTriangle(g_cursorX -4, g_cursorY+8, g_cursorX+4, g_cursorY+8, g_cursorX, g_cursorY+16, WHITE);
  //   tft.fillRect(g_cursorX-4, g_cursorY-4, 9, 9, WHITE);
  //   tft.fillRect(g_cursorX - 11, g_cursorY-4, 4, 9, WHITE);
  //   tft.fillTriangle(g_cursorX - 4, g_cursorY-12, g_cursorX+4, g_cursorY-12, g_cursorX, g_cursorY-16, WHITE);
  // }

}
void Cruiser(int r) {
  g_cursorX = r;

  tft.fillTriangle(g_cursorX +8, g_cursorY-4, g_cursorX+8, g_cursorY+4, g_cursorX+16, g_cursorY, WHITE);
  tft.fillRect(g_cursorX-4, g_cursorY-4, 9, 9, WHITE);
  tft.fillTriangle(g_cursorX - 8, g_cursorY-4, g_cursorX-8, g_cursorY+4, g_cursorX-12, g_cursorY, WHITE);

  tft.fillRect(g_cursorX - 11, g_cursorY-4, 4, 9, WHITE);
  tft.fillTriangle(g_cursorX - 12, g_cursorY-4, g_cursorX-12, g_cursorY+4, g_cursorX-16, g_cursorY, WHITE);

}

void Destroyer(int w) {
  g_cursorX = w;

  tft.fillTriangle(g_cursorX +8, g_cursorY-4, g_cursorX+8, g_cursorY+4, g_cursorX+16, g_cursorY, WHITE);
  tft.fillRect(g_cursorX - 11, g_cursorY-4, 4, 9, WHITE);
  tft.fillTriangle(g_cursorX - 4, g_cursorY-12, g_cursorX+4, g_cursorY-12, g_cursorX, g_cursorY-16, WHITE);

}

void drawX() {
  tft.drawLine(g_cursorX-6, g_cursorY -6, g_cursorX+6, g_cursorY +6, WHITE);
  tft.drawLine(g_cursorX-6, g_cursorY +6, g_cursorX+6, g_cursorY -6, WHITE);
}

void drawGrid() {
  for (int16_t y=36; y<157; y+=12) {
  tft.drawFastHLine(4, y, 120, WHITE);
  }

  for (int16_t x=4; x<125; x+=12){
  tft.drawFastVLine(x, 36, 120, WHITE);
  }
}

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

  Serial.println("initialized!");
}
