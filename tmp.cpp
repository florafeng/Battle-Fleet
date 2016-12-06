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
int h_prevX = 58;
int h_prevY = 90;

int X_coordinate = 0;
int Y_coordinate = 0;

int grid1[10][10];
int grid2[10][10];
int screen_order = 0;
int ship_order = 0;
int counter = 0;

int randX, randY;

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

bool player1() {
  bool player1 = false;
  if (screen_order == 1) {
    player1 = true;
  }
  return player1;
}

void updateOwnDisplay() {

if (screen_order == 1){
  // Serial.println("update owndisplay screen2");
  // Serial.print("h_prevX: "); Serial.println(h_prevX);
  // Serial.print("h_cursorX: "); Serial.println(h_cursorX);
  if((h_prevX != h_cursorX) || (h_prevY != h_cursorY) ) {

    int prevX = h_prevX; int prevY = h_prevY;
    int X_coordinate, Y_coordinate;
    X_coordinate = (h_prevX-2)/12;
    Y_coordinate = (h_prevY-30)/12-1;

    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_BLACK);
    tft2.drawRect(h_prevX -6, h_prevY -6, 12, 12, ST7735_BLACK);

    tft2.drawFastVLine(h_prevX-6, h_prevY-6, 12, WHITE);
    tft2.drawFastHLine(h_prevX-6, h_prevY-6, 12, WHITE);

    if (grid2[X_coordinate][Y_coordinate] == 11) {
        tft2.drawCircle(h_prevX, h_prevY, 5, RED);
    }
    if (grid2[X_coordinate][Y_coordinate] == 10) {
      tft2.drawLine(h_prevX-6, h_prevY -6, h_prevX+6, h_prevY +6, WHITE);
      tft2.drawLine(h_prevX-6, h_prevY +6, h_prevX+6, h_prevY -6, WHITE);
    }

    h_prevX = h_cursorX;
    h_prevY = h_cursorY;
    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
  }
}
}


void updateOtherDisplay() {
  int X_coordinate, Y_coordinate;

  if (screen_order == 1) {
    // Serial.println("player2 ");
    X_coordinate = (h_prevX-2)/12;
    Y_coordinate = (h_prevY-30)/12-1;
    if (grid1[X_coordinate][Y_coordinate] == 1) {

      tft.fillRect(h_prevX -6, h_prevY -6, 12, 12, BLACK);
      tft.drawRect(h_prevX -6, h_prevY -6, 12, 12, WHITE);
      tft.fillCircle(h_prevX, h_prevY, 5, ORANGE);

      grid2[X_coordinate][Y_coordinate] = 11;
      tft2.drawCircle(h_prevX, h_prevY, 5, RED);

    }
    else if (grid1[X_coordinate][Y_coordinate] == 0) {

      grid2[X_coordinate][Y_coordinate] = 10;
      tft.drawLine(h_prevX-6, h_prevY -6, h_prevX+6, h_prevY +6, WHITE);
      tft.drawLine(h_prevX-6, h_prevY +6, h_prevX+6, h_prevY -6, WHITE);

      tft2.drawLine(h_prevX-6, h_prevY -6, h_prevX+6, h_prevY +6, WHITE);
      tft2.drawLine(h_prevX-6, h_prevY +6, h_prevX+6, h_prevY -6, WHITE);
    }
    // g_prevX = h_prevX;
    // g_prevY = h_prevY;
    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
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

void ShipEdgeCases() {
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
void CheckEdges() {
  // if (screen_order = 0) {
      if (g_cursorY > 150) {
          g_cursorY = 42;
      }
      if (g_cursorY < 42) {
          g_cursorY = 150;
      }

  ShipEdgeCases();
  }

void cursorMovement() {
 int joystick_vert = analogRead(VERT);
 int joystick_horiz = analogRead(HORIZ);

  // if (screen_order == 0){
   if (joystick_vert >= 800) {
     g_cursorY += 12;
   }
   else if (joystick_vert <= 200) {
     g_cursorY -= 12;
   }
   else if (joystick_horiz <= 200) {
     g_cursorX -= 12;
   }
   else if (joystick_horiz >= 800) {
     g_cursorX += 12;
   }
  //  updateOwnDisplay();

 CheckEdges();
}

void Ship(int length) {
  // Generate (#Tile) Horizontal Ship
  // if (screen_order == 0) {
    int start = g_prevX -4 +((length-2)*-12);
    if ((start - g_prevX) <= -28) { start = g_prevX -28;}
    int end = g_prevX + 20; if(length != 5) {end = g_prevX + 16;}
    for (start; start<= end; start+=12) {
      if (screen_order == 0) {
        tft.fillRect(start, g_prevY+8, 9, 9, WHITE);
      }else {
        tft2.fillRect(start, g_prevY+8, 9, 9, WHITE);
      }

    }
}

void Black(int l) {
  // Blacken/Remove (#Tile) Horizontal Ship
  int start = g_prevX -4 +((l-2)*-12);
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  int end = g_prevX + 20; if(l != 5) {end = g_prevX + 16;}
  for (start; start<= end; start+=12) {
    if(screen_order == 0) {
      tft.fillRect(start, g_prevY+8, 9, 9, BLACK);
    }else {
      tft2.fillRect(start, g_prevY+8, 9, 9, BLACK);
    }
  }
}

void getCoordinates() {
  X_coordinate = (g_prevX+2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 +1;
  // Serial.print("X Position: "); Serial.print(g_prevX);
  // Serial.print(" Y Position: "); Serial.println(g_prevY);
}

void fillGrid() {
  // Stores ships to grid1 as 1s
   if (ship_order == 1) { //ship_order 1 == 5 Tile ship
    //  tft.setCursor(1, 5); tft.setTextColor(WHITE);
    //  tft.print("Carrier Down");
     Ship(5);
     getCoordinates();
     g_cursorX = 58; g_cursorY = 90;

     int X = X_coordinate -2;

     while(X <= (X_coordinate+2)) {
    //  Serial.println("Registering Grid");
      if (screen_order == 0) {
          grid1[X][Y_coordinate] = 1;
      }else {
        grid2[X][Y_coordinate] = 1;
      }
      X = X++;
     }
  }

  if (ship_order == 2) {
    // 4 Tile Ship
    Ship(4);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;
    int X = X_coordinate -2; //int o = X_coordinate + 2;

    while(X <= (X_coordinate+1)) {
    // Serial.println("Registering Grid");
    if (screen_order == 0) {
        grid1[X][Y_coordinate] = 1;
    }else {
      grid2[X][Y_coordinate] = 1;
    }
    // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
    X = X++;
  }
  }
  if (ship_order == 3) { // 3 Tile ship
    Ship(3);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        // Serial.println("Registering Grid");
        if (screen_order == 0) {
          grid1[X][Y_coordinate] = 1;
        }else {
          grid2[X][Y_coordinate] = 1;
        }
        X = X++;
      }
  }
  if (ship_order == 4) { // 3 Tile Ship
    Ship(3);
    // getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        // Serial.println("Registering Grid");
        if (screen_order == 0) {
            grid1[X][Y_coordinate] = 1;
        }else {
          grid2[X][Y_coordinate] = 1;
        }
        X = X++;
      }
  }

  if (ship_order == 5) {
    Ship(2);
    // getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    int X = X_coordinate; //int o = X_coordinate + 2;
        while(X <= X_coordinate+1) {
        // Serial.println("Registering grid1");
        if (screen_order == 0) {
            grid1[X][Y_coordinate] = 1;
        }else {
          grid2[X][Y_coordinate] = 1;
        }
        X = X++;
      }
  }
}

// Generate ships
void Carrier() {
  // tft.fillRect(5, 20, 100, 10, BLACK);
  // tft.setCursor(5, 20); tft.setTextColor(WHITE);
  // tft.print("Place the Carrier");

  Ship(5);

  X_coordinate = (g_prevX-2)/12 -1;
  Y_coordinate = (g_prevY-42)/12 + 1;
  int z = g_prevX;
  int v = g_prevY;
  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Black(5);

    if (screen_order == 0 && grid1[X_coordinate][Y_coordinate] == 1) {
      g_prevX = z;
      g_prevY = v;
      Ship(5);
     }
    //  if (screen_order == 1 && grid1[X_coordinate][Y_coordinate] == 1) {

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

void updateCursor() {
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
  tft2.drawFastHLine(4, y, 120, WHITE);
  }
  for (int16_t x = 4; x < 125; x += 12){
  tft.drawFastVLine(x, 36, 120, WHITE);
  tft2.drawFastVLine(x, 36, 120, WHITE);
  }
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

void grid3Int() {
  for (int q = 0; q < 10; q++) {
    for (int p = 0; p<10; p++) {
    grid3[q][p] = 0;
    }
  }
}

int calcLSpace(int positionX, int positionY, int dir) {
  // bool isValid = true;
  counter = 0;
  if(dir == 0) { // horizontal
    while(true) {
      if(grid3[positionX-1][positionY] == 0) {
        counter++;
        positionX--;
      }else {
        break;
      }
    }
  }else{ // vertical
    while(true) {
      if(grid3[positionX][positionY-1] == 0) {
        counter++
        positionY--;
      }
      else {
        break;
      }
    }
  }
  return counter;
}

int calcRSpace(int positionX, int positionY, int dir) {
  counter = 0;
  if(dir == 0) {// horizontal
    while(true) {
      if(grid3[positionX+1][positionY] == 0) {
        counter++;
        positionX++;
      }
      else {
        break;
      }
    }
  }else{ // vertical
    while(true) {
      if(grid3[positionX][positionY+1] == 0) {
        counter++
        positionY++;
      }
      else {
        break;
      }
    }
  }
  return counter;
}

void symmetricShip(int positionX, int positionY, int dir, int length) {
  if(dir == 0){ // horizontal
    for(int index = positionX; index > (positionX-1-length/2); positionX--){
      grid3[positionX][positionY] = 1;
    }
    for(int index = positionX; index < (positionX+1+length/2); positionX++){
      grid3[positionX][positionY] = 1;
    }
  }
  else if (dir == 1) { // vertical
    for(int index = positionY; index > (positionY-1-length/2); positionY--){
      grid3[positionX][positionY] = 1;
    }
    for(int index = positionY; index < (positionY+1+length/2); positionY++){
      grid3[positionX][positionY] = 1;
    }
  }
  ship_order += 1;
}

void nonSymmetricShip(int positionX, int positionY, int dir, int length) {
}


void placeShip(randX, randY) {
  int dir = random(0,1);
  leftSpace = calcLSpace(randX, randY, dir);
  rightSpace = calcRSpace(randX, randY, dir);
  if(ship_order == 0) { // carrier: length 5
    if(leftSpace >= 2 && rightSpace >= 2) {
      symmetricShip(randX, randY, dir, 4);
      ship_order++;
    }
    else if((leftSpace + rightSpace) < 4) {
      continue;
    }
    else{
      nonSymmetricShip(randX, randY, dir, 4);
    }
  }
  else if(ship_order == 1) { // battleship: length 4
    if((leftSpace + rightSpace) >= 3) {
      nonSymmetricShip(randX, randY, dir, 3)
    }
    else{
      continue;
    }
  }
  else if(ship_order == 2 || ship_order == 3) { // cruiser: length 3
    if(leftSpace >= 1 && rightSpace >= 1) {
      symmetricShip(randX, randY, dir, 2);
      ship_order++;
    }
    else if((leftSpace + rightSpace) < 2) {
      continue;
    }
    else{
      nonSymmetricShip(randX, randY, dir, 2);
    }
  }
  else if(ship_order == 4) { // Destroyer: length 2
    if((leftSpace + rightSpace) >= 1) {
      nonSymmetricShip(randX, randY, dir, 1)
    }
    else{
      continue;
    }
  }

}


void Setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
}


int main() {
  init();
  Serial.begin(9600);
  Setup();
  grid3Int();

  int startTime = millis();
//  bool isValid = true;
  // int test = random(0, 9);
  // Serial.println(test);

  while(true) {
    randX = random(0,9); randY = random(0,9);
    if (grid3[randX][randY] != 1) {
      placeShip(randX, randY);
    }

      select = digitalRead(SEL);
      // Serial.print("Screen order: "); Serial.println(screen_order);
      cursorMovement();
      updateCursor();

      if (select == 0){
         ship_order++;
         fillGrid();
         delay(250);
       }

      if (ship_order == 5) {
         delay(100);
         tft.fillRect(5, 20, 120, 10, BLACK);
         tft.setCursor(5, 20);
         tft.setTextColor(BLACK, WHITE);
         tft.print("Setup Complete!");
         // Initialize for screen 2
         if (screen_order < 2) {
           g_cursorX = 58;
           g_cursorY = 90;
           ship_order = 0;
           screen_order++;
         }else {

           Serial.println("screen 2 done!");
           // set up complete, communication with other arduino can be started
         }
       }

      //  if ((select == 0) && (screen_order == 1)) {
      //     updateOtherDisplay();
      //   }
  }
// while(screen_order == 1) {
//   Serial.println("Screen order 2");
//   select = digitalRead(SEL);
//   cursorMovement();
//   if (select == LOW) {
//
//
//   }
// }

return 0;
Serial.end();
}
