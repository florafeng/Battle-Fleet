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


const int VERT = 0;  // analog input
const int HORIZ = 1; // analog input
const int SEL = 3;
int joystick_int_vert, joystick_int_horiz;
int select;

// int L_Button = 10;
// int R_Button = 11;
// int Up_Button = 3;
// int Down_Button = 4;
 int Menu_Button = 2;
 int buttonValueM=1;

int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = 58; // previously drawn position of the cursor
int g_prevY = 90;

int h_cursorX = 58;
int h_cursorY = 90;
int h_prevX = -1;
int h_prevY = -1;

int X_coordinate = 0;
int Y_coordinate = 0;

int grid1[10][10];
int grid2[10][10];
int screen_order = 0;
int ship_order = 0;

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


void cursorMovement();
void CheckEdges();
void updateCursor();

void lcdInt();
void gridInt();
void joystickInt();
void Setup();

void fillGrid();
void Carrier();
void Battleship();
void Cruiser();
void Destroyer();
void drawX();

void Battleship();

int main() {
  init();
  Serial.begin(9600);
  Setup();

  int startTime = millis();

g_prevX = 58; g_prevY = 90;

while(screen_order == 0) {

select = digitalRead(SEL);

cursorMovement();
updateCursor();

   if (select == 0){
     ship_order++;
     delay(250);
    }

   if (ship_order == 5) {
     delay(500);
     tft.fillRect(5, 20, 120, 5, BLACK);
     tft.setCursor(5, 20);
     tft.setTextColor(BLACK, WHITE);
     tft.print("Setup Complete!");
     screen_order = 1;
     break;
    }
  }

return 0;
Serial.end();
}


void fillGrid() {

  //  X_coordinate = (g_prevX-2)/12;
  //  Y_coordinate = (g_prevY-30)/12 - 1;

   if ((screen_order == 0) && (ship_order == 1)) {
     tft.setCursor(1, 5); tft.setTextColor(WHITE);
     tft.print("Carrier Down");

    Y_coordinate = (g_prevY-30)/12 - 1;

    X_coordinate = (g_prevX-2)/12;
    int x0 = X_coordinate-2; int x1 = X_coordinate +2;
    for (x0; x0 < x1; x0++) {
        grid1[X_coordinate][Y_coordinate] = 1;
    }

  }

  if ((screen_order==0) && (ship_order == 2)) {

   Y_coordinate = (g_prevY-30)/12 - 1;

   X_coordinate = (g_prevX-2)/12;
   int x0 = X_coordinate-2; int x1 = X_coordinate +1;
   for (x0; x0 < x1; x0++) {
       grid1[X_coordinate][Y_coordinate] = 1;
   }
    // int c = g_prev+49
    // for (g_prevX; g_prevX< c; g_prevX+=12) {

  }
  // while (ship_order == 3) {
  //   Cruiser();
  //
  //   if (select == 0) { break;}
  // }
  // while (ship_order == 4) {
  //   Cruiser();
  //
  //   if (select == 0) { break;}
  // }
  if (ship_order == 5) {
  }

}

void cursorMovement() {
 int joystick_vert = analogRead(VERT);
 int joystick_horiz = analogRead(HORIZ);

 // if (screen_order == 0) {
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
   CheckEdges();
}
 // else if (screen_order == 1)  {
 //   if (joystick_vert >= 700) {
 //     h_cursorY += 12;
 //   }
 //   else if (joystick_vert <= 300) {
 //     h_cursorY -= 12;
 //   }
 //   else if (joystick_horiz <= 300) {
 //     h_cursorX -= 12;
 //   }
 //   else if (joystick_horiz >= 700) {
 //     h_cursorX += 12;
 //   }


void CheckEdges() {
  if (g_cursorY > 150) {
    g_cursorY = 42;
  }
  if (g_cursorY < 42) {
    g_cursorY = 150;
  }

  int L; int R;


  if (ship_order == 0){

  L = 2*12; R = 2*12;

  if (g_cursorX <34) {
    g_cursorX = 94;
  }
  if (g_cursorX > 94) {
    g_cursorX = 34;
  }
  }

  if (ship_order == 1){
  L = 2*12; R = 12;

  if (g_cursorX <34) {
    g_cursorX = 106;
  }
  if (g_cursorX > 106) {
    g_cursorX = 34;
  }
}
// if (ship_order == 3){
//   L = 12; R = 12;
//
//   if (g_cursorX <(10+L)) {
//     g_cursorX = 118-R;
//   }
//   if (g_cursorX > 118-R) {
//     g_cursorX = 10+L;
//   }
// }

  // X_coordinate = (g_prevX-2)/12;
  // Y_coordinate = (g_prevY-30)/12 - 1;


}

void updateCursor() {

  cursorMovement();

  if (ship_order == 0) {
  tft.fillRect(5, 20, 100, 5, BLACK);
  tft.setCursor(5, 20); tft.setTextColor(WHITE);
  tft.print("Place the Carrier");

  Carrier();
  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

  Y_coordinate = (g_prevY-30)/12 - 1;

  int c = g_prevX + 49;
  for (g_prevX; g_prevX< c; g_prevX+=12) {
      tft.fillRect(g_prevX-28, g_prevY-4, 9, 9, BLACK);
  }

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

  Carrier();

  // for (int x0 = 10; x0 < 118 ; x0 += 12 ) {
  //     X_coordinate = (g_prevX-2)/12;
  //     if (grid1[x0][Y_coordinate] == 1) {    ;
  //
  //       tft.fillRect(x0, g_prevY - 4, 9, 9, WHITE);
  //     }
  //   }
  }
  }
  if (ship_order == 1) {
    tft.fillRect(5, 20, 100, 5, BLACK);
    tft.setCursor(5, 20); tft.setTextColor(WHITE);
    tft.print("Place the Battleship");

    Battleship();
    delay(150);
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Y_coordinate = (g_prevY-30)/12 - 1;

    int c1 = g_prevX +37;
    for (g_prevX; g_prevX< c1; g_prevX+=12) {
        tft.fillRect(g_prevX-28, g_prevY-4, 9, 9, BLACK);
    }

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;

    Battleship();


    // int x0 = 10;
    // for (x0; x0<118; x0 +=12 ) {
    //   X_coordinate = (g_prevX-2)/12;
    //     if (grid1[x0][Y_coordinate] == 1) {    ;
    //         tft.fillRect(x0, g_prevY - 4, 9, 9, WHITE);
    //     }
    //   }
    }
  }

  if (ship_order == 2) {
    Cruiser();
    delay(150);
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    //Y_coordinate = (g_prevY-30)/12 - 1;

    int c = g_prevX + 25;
    for (g_prevX-12; g_prevX< c; g_prevX+=12) {
        tft.fillRect(g_prevX-16, g_prevY-4, 9, 9, BLACK);
    }

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;

    Cruiser();
    }
  }

  if (ship_order == 3) {

    Cruiser();
    delay(150);
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    //Y_coordinate = (g_prevY-30)/12 - 1;

    int c = g_prevX + 25;
    for (g_prevX-12; g_prevX< c; g_prevX+=12) {
        tft.fillRect(g_prevX-16, g_prevY-4, 9, 9, BLACK);
    }

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;

    Cruiser();
    }
  }

  if (ship_order == 4) {

    Destroyer();
    delay(150);
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Y_coordinate = (g_prevY-30)/12 - 1;


    for (int i =0; i<2; i++) {
        tft.fillRect(g_prevX-4, g_prevY-4, 9, 9, BLACK);
        g_prevX+=12;
    }

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;

    Destroyer();

    }

  }

}

void Carrier() {
  int i = g_prevX;
  int c = g_prevX + 49;
  for (i; i< c; i+=12) {
      tft.fillRect(i-28, g_prevY-4, 9, 9, WHITE);
  }
}

void Battleship() {

//  tft.fillTriangle(g_cursorX-28, g_cursorY-4, 9, 9, WHITE);
  tft.fillTriangle(g_prevX +8, g_prevY-4, g_prevX+8, g_prevY+4, g_prevX+16, g_prevY, WHITE);
  tft.fillRect(g_prevX-4, g_prevY-4, 9, 9, WHITE);
  tft.fillRect(g_prevX-16, g_prevY-4, 9, 9, WHITE);
  tft.fillRect(g_prevX - 23, g_prevY-4, 4, 9, WHITE);
  tft.fillTriangle(g_prevX - 23, g_prevY-4, g_prevX-23, g_prevY+4, g_prevX-28, g_prevY, WHITE);


}
void Cruiser() {

  tft.fillTriangle(g_cursorX +8, g_cursorY-4, g_cursorX+8, g_cursorY+4, g_cursorX+16, g_cursorY, WHITE);
  tft.fillRect(g_cursorX-4, g_cursorY-4, 9, 9, WHITE);
  tft.fillTriangle(g_cursorX - 8, g_cursorY-4, g_cursorX-8, g_cursorY+4, g_cursorX-12, g_cursorY, WHITE);

  tft.fillRect(g_cursorX - 11, g_cursorY-4, 4, 9, WHITE);
  tft.fillTriangle(g_cursorX - 12, g_cursorY-4, g_cursorX-12, g_cursorY+4, g_cursorX-16, g_cursorY, WHITE);

}

void Destroyer() {

  tft.fillTriangle(g_cursorX +8, g_cursorY-4, g_cursorX+8, g_cursorY+4, g_cursorX+16, g_cursorY, WHITE);
  tft.fillRect(g_cursorX+1 , g_cursorY-4, 4, 9, WHITE);
  tft.fillTriangle(g_cursorX+1, g_cursorY-4, g_cursorX, g_cursorY+4, g_cursorX-4, g_cursorY, WHITE);

}

void drawX() {
  tft.drawLine(g_cursorX-6, g_cursorY -6, g_cursorX+6, g_cursorY +6, WHITE);
  tft.drawLine(g_cursorX-6, g_cursorY +6, g_cursorX+6, g_cursorY -6, WHITE);
}


void joystickInt() {
  pinMode(SEL, INPUT);
  pinMode(Menu_Button, INPUT);
  digitalWrite(Menu_Button, HIGH);
  digitalWrite(SEL, HIGH);

  // joystick_int_vert = analogRead(VERT);
  // joystick_int_horiz = analogRead(HORIZ);
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


void gridInt() {
  int p = 0;
  for (int q = 0; q < 10; q++) {
    grid1[q][p] = 0;
    grid2[q][p] = 0;
    p++;
  }
}


void Setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
}
