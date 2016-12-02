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


void updateCursor() {
  cursorMovement();
  if (ship_order == 0) {
    Carrier();
  }
  if (ship_order == 1) {
    Battleship();
    }
  if ((ship_order == 2) || (ship_order == 3)) {
    Cruiser();
  }
  if (ship_order == 4) {
    Destroyer();
  }
}

void checkGrid() {
  // Prints Grid to Serial Mon

  int y0 = 0;
  for (int y0; y0<10; y0++) {
    for (int x0 =0; x0 <10; x0++) {
        Serial.print(grid1[x0][y0]); Serial.print(" ");
    }
  Serial.println();
  }
}

void cursorMovement() {
 int joystick_vert = analogRead(VERT);
 int joystick_horiz = analogRead(HORIZ);

 // if (screen_order == 0)
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
      g_cursorY = 30;
  }
  if (g_cursorY < 30) {
      g_cursorY = 150;
  }

  if (ship_order == 0){
      if (g_cursorX <34) {
        g_cursorX = 106;
      }
      if (g_cursorX > 106) {
        g_cursorX = 34;
      }
  }

  if (ship_order == 1){
      if (g_cursorX <34) {
        g_cursorX = 106;
      }
      if (g_cursorX > 106) {
        g_cursorX = 34;
      }
  }
  if ((ship_order == 2) || (ship_order == 3)){
      if (g_cursorX <22) {
        g_cursorX = 118;
      }
      if (g_cursorX > 118) {
        g_cursorX = 22;
      }
  }
  if (ship_order == 4){
      if (g_cursorX <10) {
        g_cursorX = 106;
      }
      if (g_cursorX > 118) {
        g_cursorX = 10;
      }
  }
}

void Ship(int length) {
  // Generate (#Tile) Horizontal Ship
  int start = g_prevX -4 +((length-2)*-12);
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  int end = g_prevX + 20; if(length != 5) {end = g_prevX + 16;}
  for (start; start<= end; start+=12) {
      tft.fillRect(start, g_prevY+8, 9, 9, WHITE);
  }
}

void Black(int l) {
  // Blacken/Remove (#Tile) Horizontal Ship
  int start = g_prevX -4 +((l-2)*-12);
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  int end = g_prevX + 20; if(l != 5) {end = g_prevX + 16;}
  for (start; start<= end; start+=12) {
      tft.fillRect(start, g_prevY+8, 9, 9, BLACK);
  }
}

void drawX() {
  tft.drawLine(g_cursorX-6, g_cursorY -6, g_cursorX+6, g_cursorY +6, WHITE);
  tft.drawLine(g_cursorX-6, g_cursorY +6, g_cursorX+6, g_cursorY -6, WHITE);
}

void fillGrid() {
  // Stores ships to grid1 as 1s
   if ((screen_order == 0) && (ship_order == 1)) { //ship_order 1 == 5 Tile ship
    //  tft.setCursor(1, 5); tft.setTextColor(WHITE);
    //  tft.print("Carrier Down");
     Ship(5);
     getCoordinates();
     g_cursorX = 58; g_cursorY = 90;

     int X = X_coordinate -2; //int o = X_coordinate + 2;
         while(X <= (X_coordinate+2)) {
         Serial.println("Registering Grid");
         grid1[X][Y_coordinate] = 1;
         Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
         X = X++;
       }

  }

  if ((screen_order==0) && (ship_order == 2)) {
    // 4 Tile Ship
    Ship(4);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;
    int X = X_coordinate -2; //int o = X_coordinate + 2;
        while(X <= (X_coordinate+1)) {
        Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
        X = X++;
      }
  }
  if ((screen_order == 0) && (ship_order == 3)) { // 3 Tile ship
    Ship(3);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        X = X++;
      }
  }
  if ((screen_order == 0) && (ship_order == 4)) { // 3 Tile Ship
    Ship(3);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        X = X++;
      }
  }
  if ((screen_order == 0) && (ship_order == 5)) {
    Ship(2);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        Serial.println("Registering grid1");
        grid1[X][Y_coordinate] = 1;
        X = X++;
      }
  }
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

  for (int q = 0; q < 10; q++) {
    for (int p = 0; p<10; p++) {
    grid1[q][p] = 0;
    grid2[q][p] = 0;
    }
  }
  checkGrid();
}


void Setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
}

// Generate ships
void Carrier() {
  tft.fillRect(5, 20, 100, 10, BLACK);
  tft.setCursor(5, 20); tft.setTextColor(WHITE);
  tft.print("Place the Carrier");

  Ship(5);

  X_coordinate = (g_prevX-2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 + 1;
  int z = g_prevX;
  int v = g_prevY;
  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Black(5);

    if (grid1[X_coordinate][Y_coordinate] == 1) {
      g_prevX = z;
      g_prevY = v;
      Ship(5);
      }

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

  Ship(5);
}
}

void Battleship() {
  tft.fillRect(5, 20, 100, 5, BLACK);
  tft.setCursor(5, 20); tft.setTextColor(WHITE);
  tft.print("Place the Battleship");

  Ship(4);

  X_coordinate = (g_prevX-2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 + 1;
  int z = g_prevX;
  int v = g_prevY;

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {


    Black(4);
  if (grid1[X_coordinate][Y_coordinate] == 1) {
      g_prevX = z;
      g_prevY = v;
      Ship(4);
  }

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

  Ship(4);
}
}

void Cruiser() {
  Ship(3);

  X_coordinate = (g_prevX-2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 + 1;
  int z = g_prevX;
  int v = g_prevY;

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

  //Y_coordinate = (g_prevY-30)/12 - 1;

  Black(3);
  if (grid1[X_coordinate][Y_coordinate] == 1) {
    g_prevX = z;
    g_prevY = v;
    Ship(3);
    }

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

  Ship(3);
  }
}
void Destroyer() {
  Ship(2);

  X_coordinate = (g_prevX-2)/12;
  Y_coordinate = (g_prevY-30)/12 - 1;

  int c = g_prevX;

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Black(2);
    if (grid1[X_coordinate][Y_coordinate] == 1) {
      g_prevX = c;
      Ship(2);
      }

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;

    Ship(2);
  }
}

void getCoordinates() {
  X_coordinate = (g_prevX+2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 +1;
  Serial.print("X Position: "); Serial.print(g_prevX);
  Serial.print(" Y Position: "); Serial.println(g_prevY);

}

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
         fillGrid();
         delay(250);
       }

      if (ship_order == 5) {
         delay(500);
         tft.fillRect(5, 20, 120, 10, BLACK);
         tft.setCursor(5, 20);
         tft.setTextColor(BLACK, WHITE);
         tft.print("Setup Complete!");
         checkGrid();
         screen_order = 1;
         break;
       }
  }

return 0;
Serial.end();
}
