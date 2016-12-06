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
 int R_Button = 2;
 int Rotation=1;

int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = 58; // previously drawn position of the cursor
int g_prevY = 90;

int h_cursorX = 58;
int h_cursorY = 90;
int h_prevX = 1;
int h_prevY = 1;

int X_coordinate = 0;
int Y_coordinate = 0;
//uint8_t X_X; uint8_t Y_Y;


int grid1[10][10];
int grid2[10][10];
int screen_order = 0;
int ship_order = 0;
int Orientation = 0;
int hit_counter = 0;


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

// bool player1() {
//   bool player1 = false;
//   if (screen_order == 1) {
//     player1 = true;
//   }
//   return player1;
// }

void updateOwnDisplay() {

if (screen_order == 1){
  // Serial.println("update owndisplay screen2");
  // Serial.print("h_prevX: "); Serial.println(h_prevX);
  // Serial.print("h_cursorX: "); Serial.println(h_cursorX);
  if((h_prevX != h_cursorX) || (h_prevY != h_cursorY) ) {

    int prevX = h_prevX; int prevY = h_prevY;
    int X_coordinate, Y_coordinate;
    X_coordinate = (h_prevX+2)/12;
    Y_coordinate = (h_prevY-42)/12 +1;

    if (grid2[X_coordinate][Y_coordinate] == 11) {;}
    else if (grid2[X_coordinate][Y_coordinate] == 10) {;}

    else {
    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_BLACK);
    tft2.drawRect(h_prevX -6, h_prevY -6, 12, 12, ST7735_BLACK);
    }

    tft2.drawFastVLine(h_prevX-6, h_prevY-6, 12, WHITE);
    tft2.drawFastHLine(h_prevX-6, h_prevY-6, 12, WHITE);

    h_prevX = h_cursorX;
    h_prevY = h_cursorY;
    if (grid2[X_coordinate][Y_coordinate] == 11) {;}
    else if (grid2[X_coordinate][Y_coordinate] == 10) {;}
    else {
    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
    }
  }
}
}


void updateOtherDisplay() {
  int X_coordinate, Y_coordinate;

  if (screen_order == 1) {
    // Serial.println("player2 ");
    X_coordinate = (h_prevX+2)/12;
    Y_coordinate = (h_prevY-42)/12 +1;
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

//    tft2.fillCircle(h_prevX, h_prevY, 5, ST7735_GREEN);
  }
}

void checkGrid() {
  // Prints Grid to Serial Mon

  for (int y0=1; y0<=10; y0++) {
    for (int x0 =1; x0 <=10; x0++) {
//        Serial.print(grid1[x0][y0]); Serial.print(" ");
        if (screen_order == 1) {
          Serial.print(grid2[x0][y0]);
          if (grid2[x0][y0] == 11){ hit_counter = hit_counter +1;}
        }
    }
  //Serial.println();
  }
}

void ShipEdgeCases() {
  if (Orientation == 0) {
    if (g_cursorY > 150) {
        g_cursorY = 42;
    }
    if (g_cursorY < 42) {
        g_cursorY = 150;
    }

  if (ship_order == 0){
      if (g_cursorX <34) {
        g_cursorX = 94;
      }
      if (g_cursorX > 94) {
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
        g_cursorX = 106;
      }
      if (g_cursorX > 106) {
        g_cursorX = 22;
      }
  }
  if (ship_order == 4){
      if (g_cursorX <10) {
        g_cursorX = 106;
      }
      if (g_cursorX > 106) {
        g_cursorX = 10;
      }
    }
  }
  if (Orientation == 1) {
    if (g_cursorX > 118) {
        g_cursorX = 10;
    }
    if (g_cursorX < 10) {
        g_cursorX = 118;
    }

    if (ship_order == 0){
        if (g_cursorY <54) {
          g_cursorY = 114;
        }
        if (g_cursorY > 114) {
          g_cursorY = 54;
        }
      }
    if (ship_order == 1){
        if (g_cursorY <54) {
          g_cursorY = 126;
        }
        if (g_cursorY > 126) {
          g_cursorY = 54;
        }
    }
    if ((ship_order == 2) || (ship_order == 3)){
        if (g_cursorY <42) {
          g_cursorY = 126;
        }
        if (g_cursorY > 126) {
          g_cursorY = 42;
        }
    }
    if (ship_order == 4){
        if (g_cursorY <30) {
          g_cursorY = 126;
        }
        if (g_cursorY > 126) {
          g_cursorY = 30;
        }
      }
    }
}

void CheckEdges() {
  if (screen_order == 0) {
      ShipEdgeCases();
  }
  if (screen_order == 1) {
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


void cursorMovement() {
 int joystick_vert = analogRead(VERT);
 int joystick_horiz = analogRead(HORIZ);

  if (screen_order == 0){
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
  }

 else if (screen_order == 1)  {
   if (joystick_vert >= 800) {
     h_cursorY += 12;
   }
   else if (joystick_vert <= 200) {
     h_cursorY -= 12;
   }
   else if (joystick_horiz <= 200) {
     h_cursorX -= 12;
   }
   else if (joystick_horiz >= 800) {
     h_cursorX += 12;
   }
   updateOwnDisplay();
   delay(200);
  }
 }

void Black(int length) {
  // Blacken/Remove (#Tile) Horizontal Ship
  int start = g_prevX -4 +((length-2)*-12);
  int starty = g_prevY + 8 + ((length -2)*  - 12);
  if (length == 1) {
    start = g_prevX -4;
    starty = g_prevY+8;
  }
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  if ((start - g_prevY) <= -28) { starty = g_prevY - 28;}
  int end = g_prevX + 20; int endy = g_prevY + 20;
  if(length != 5) {end = g_prevX + 16; endy = g_prevY + 15;}
  if (length ==1) {
    end = g_prevX +7; endy = g_prevY + 7;
  }

  if (Orientation == 0) {
  for (start; start<= end; start+=12) {
        tft.fillRect(start, g_prevY-4, 9, 9, BLACK);
  }
}
  if (Orientation == 1) {
  for (starty; starty<= endy; starty+=12) {
        tft.fillRect(g_prevX-4, starty, 9, 9, BLACK);
  }
  }
}

void Ship(int length) {
  // Generate (#Tile) Horizontal Ship
//  Black(length);
  int start = g_prevX -4 +((length-2)*-12);
  int starty = g_prevY + 8 + ((length -2)*  - 12);
  if (length == 1) {
    start = g_prevX -4;
    starty = g_prevY+8;
  }
  if ((start - g_prevX) <= -28) { start = g_prevX -28;}
  if ((start - g_prevY) <= -28) { starty = g_prevY - 28;}
  int end = g_prevX + 20; int endy = g_prevY + 20;
  if(length != 5) {end = g_prevX + 16; endy = g_prevY + 15;}
  if (length ==1) {
    end = g_prevX +7; endy = g_prevY + 7;
  }

  if (Orientation == 0) {
  for (start; start<= end; start+=12) {
        tft.fillRect(start, g_prevY-4, 9, 9, WHITE);
  }
}
  if (Orientation == 1) {
  for (starty; starty<= endy; starty+=12) {
        tft.fillRect(g_prevX-4, starty, 9, 9, WHITE);
  }
  }
}

void getCoordinates() {
  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  // Serial.print("X Position: "); Serial.print(g_cursorX);
  // Serial.print(" Y Position: "); Serial.println(g_cursorY);
}

void fillGrid() {
  // Stores ships to grid1 as 1s
   if ((screen_order == 0) && (ship_order == 1)) { //ship_order 1 == 5 Tile ship
    //  tft.setCursor(1, 5); tft.setTextColor(WHITE);
    //  tft.print("Carrier Down");
     Ship(5);
     getCoordinates();
     g_cursorX = 58; g_cursorY = 90;

     if(Orientation ==0) {
     int X = X_coordinate -2; //int o = X_coordinate + 2;
         while(X <= (X_coordinate+2)) {
        //  Serial.println("Registering Grid");
         grid1[X][Y_coordinate] = 1;
        //  Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
         X = X++;
       }
     }
     if(Orientation == 1) {
     int Y = Y_coordinate - 2;
        while( Y <= (Y_coordinate +2)) {
          // Serial.println("Registering Grid");
          grid1[X_coordinate][Y] = 1;
          // Serial.print("X: "); Serial.print(X_coordinate); Serial.print(" Y: "); Serial.println(Y);
          Y = Y++;
        }
     }
  }

  if ((screen_order==0) && (ship_order == 2)) {
    // 4 Tile Ship
    Ship(4);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    if (Orientation == 0) {
    int X = X_coordinate -2; //int o = X_coordinate + 2;
        while(X <= (X_coordinate+1)) {
        // Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
        X = X++;
      }
    }
    if(Orientation == 1) {
    int Y = Y_coordinate - 2;
       while( Y <= (Y_coordinate +1)) {
        //  Serial.println("Registering Grid");
         grid1[X_coordinate][Y] = 1;
        //  Serial.print("X: "); Serial.print(X_coordinate); Serial.print(" Y: "); Serial.println(Y);
         Y = Y++;
       }
     }
  }
  if ((screen_order == 0) && (ship_order == 3)) { // 3 Tile ship
    Ship(3);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    if (Orientation == 0) {
    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= (X_coordinate+1)) {
        // Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
        X = X++;
      }
    }
    if(Orientation == 1) {
    int Y = Y_coordinate - 1;
       while( Y <= (Y_coordinate +1)) {
        //  Serial.println("Registering Grid");
         grid1[X_coordinate][Y] = 1;
        //  Serial.print("X: "); Serial.print(X_coordinate); Serial.print(" Y: "); Serial.println(Y);
         Y = Y++;
       }
    }
  }
  if ((screen_order == 0) && (ship_order == 4)) { // 3 Tile Ship
    Ship(3);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

    if (Orientation == 0) {
    int X = X_coordinate -1; //int o = X_coordinate + 2;
        while(X <= (X_coordinate+1)) {
        // Serial.println("Registering Grid");
        grid1[X][Y_coordinate] = 1;
        // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
        X = X++;
      }
    }
    if(Orientation == 1) {
    int Y = Y_coordinate - 1;
       while( Y <= (Y_coordinate +1)) {
        //  Serial.println("Registering Grid");
         grid1[X_coordinate][Y] = 1;
        //  Serial.print("X: "); Serial.print(X_coordinate); Serial.print(" Y: "); Serial.println(Y);
         Y = Y++;
       }
    }
  }
  if ((screen_order == 0) && (ship_order == 5)) {
    Ship(2);
    getCoordinates();

    g_cursorX = 58; g_cursorY = 90;

      if (Orientation == 0) {
      int X = X_coordinate; //int o = X_coordinate + 2;
          while(X <= (X_coordinate+1)) {
          // Serial.println("Registering Grid");
          grid1[X][Y_coordinate] = 1;
          // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y_coordinate);
          X = X++;
        }
      }
      if(Orientation == 1) {
      int Y = Y_coordinate;
         while( Y <= (Y_coordinate +1)) {
          //  Serial.println("Registering Grid");
           grid1[X_coordinate][Y] = 1;
          //  Serial.print("X: "); Serial.print(X_coordinate); Serial.print(" Y: "); Serial.println(Y);
           Y = Y++;
         }
      }
  }
}


void CheckShipGrid(int X, int Y) {
  int X0 = X - 6; int X1 = X+5;
  int Y0 = Y-6; int Y1 = Y+5;

  if (Orientation == 0) {
      for (X0; X0<X1; X0++) {
        if ((X0 <1) || (X0 > 10)) {
          ;
        }
        else if (grid1[X0][Y] == 1) {
           //Serial.print("Saving Ship at ");
           //Serial.print("X: "); //Serial.print(X0); Serial.print(" Y: "); Serial.println(Y);
          g_prevX = (X0)*12 -2;
          g_prevY = (Y)*12 +30;
          Ship(1);
        }
        else if (grid1[X0][Y] == 0){;}
      }
    }
    else if (Orientation ==1) {
      for (Y0; Y0<Y1; Y0++) {
        if ((Y0 <1) || (Y0 > 10)) {
          ;
        }
        else if (grid1[X][Y0] == 1) {
          // Serial.print("Saving Ship at ");
          // Serial.print("X: "); Serial.print(X); Serial.print(" Y: "); Serial.println(Y0);
          g_prevX = (X)*12 -2;
          g_prevY = (Y0)*12 +30;
          Ship(1);
        }
        else if (grid1[X][Y0] == 0){;}
      }
    }

}
// Generate ships
void Carrier() {

  Ship(5);

  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;

  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);

  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
  Black(5);
  CheckShipGrid(X, Y);

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;
  Ship(5);
}
}

void Battleship() {

  Ship(4);

  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;

  // X_coordinate = (g_prevX+2)/12;
  // Y_coordinate = (g_prevY-30)/12 + 1;
  int X = X_coordinate; int Y = Y_coordinate;

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
  Black(4);

  CheckShipGrid(X, Y);

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;
  Ship(4);
}
}

void Cruiser() {
  Ship(3);

  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;

  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
  //Y_coordinate = (g_prevY-30)/12 - 1;
  Black(3);
  CheckShipGrid(X, Y);

  g_prevX = g_cursorX;
  g_prevY = g_cursorY;
  Ship(3);
  }
}
void Destroyer() {
  Ship(2);

  X_coordinate = (g_prevX+2)/12;
  Y_coordinate = (g_prevY-30)/12;
  int X = X_coordinate; int Y = Y_coordinate;
  delay(150);
  if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {

    Black(2);
    CheckShipGrid(X, Y);

    g_prevX = g_cursorX;
    g_prevY = g_cursorY;
    Ship(2);
  }
}

void updateCursor() {

  cursorMovement();
  CheckEdges();
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

void Scoreboard() {

  tft2.fillRect(1, 20, 120, 25, BLACK);
  tft2.setCursor(1, 5);
  tft2.setTextColor(BLACK, WHITE);
  tft2.print("2"); tft2.println(" Ships Knocked Down");
  tft2.setCursor(5, 20);
  tft2.print("5-2"); tft2.println(" Ships Remaining");


}

void joystickInt() {
  pinMode(SEL, INPUT);
  pinMode(R_Button, INPUT);
  digitalWrite(R_Button, HIGH);
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

void Parse(uint8_t v) {
  X_coordinate= v/10; Y_coordinate= v%10;

//   Serial.println("Parsing");
//
// if (sizeof(v) > 3) {
//   Serial.println("4 Digit");
//   X_X= 10; Y_Y= 10;
// }
//
// if ((sizeof(v) == 3) && (v%1000 == 10)) {
//   Serial
//   X_X= 10; Y_Y= v%10;
// }
// else if ((sizeof(v) == 3) && (v%1000 != 10)) {
//   X_X= v%1000; Y_Y= 10;
// }
// else if (sizeof(v) == 2) {
//  Serial.println("2 Digits");
  //}
}

void uint8_to_serial3(uint8_t num) {
  Serial3.write((char) (num >> 0));
}

// read from serial3
uint8_t uint8_from_serial3() {
  uint8_t num = 0;
  num = num | ((uint8_t) Serial3.read()) << 0;
  return num;
}

bool wait_on_serial3( uint8_t nbytes, long timeout ) {
  unsigned long deadline = millis() + timeout;
  while (Serial3.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1);
  }
  return Serial3.available()>=nbytes;
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


int main() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  Setup();

  tft.fillRect(1, 20, 120, 10, BLACK);
  tft.setCursor(1, 1);
  tft.setTextColor(WHITE, BLACK);
  tft.print("Welcome to Arduino Battleship");

  tft2.fillRect(1, 20, 120, 10, BLACK);
  tft2.setCursor(1, 1);
  tft2.setTextColor(WHITE, BLACK);
  tft2.print("Welcome to Arduino Battleship");

  tft.fillRect(0, 20, 128, 15, BLACK);
  tft.setCursor(1, 20); tft.setTextColor(WHITE);
  tft.print("Set Your 5 Ships");

  while(screen_order == 0) {

      select = digitalRead(SEL);
      Rotation = digitalRead(R_Button);

      updateCursor();

      if ((select == 0) && (screen_order == 0)){
         ship_order++;
         fillGrid();
         delay(250);
       }

      if ((Rotation == LOW) && (Orientation == 0)) {

          Orientation = 1;
          delay(250);
          continue;
      }

      if ((Rotation == LOW) && (Orientation == 1)) {
          Orientation = 0;
          delay(250);
          continue;
      }

      if (ship_order == 5) {
         delay(100);
         tft.fillRect(0, 20, 128, 15, BLACK);
         tft.setCursor(5, 20);
         tft.setTextColor(BLACK, WHITE);
         tft.print("Setup Complete!");
  //       checkGrid();
         // Initiate Serial Communication
         Serial3.print("C");
         screen_order = 1;
         break;
       }
     }

  while(screen_order == 1) {
  // 1. decode number
  uint8_t X1, Y1;
  Serial3.print("Setup Complete!");

  if (Serial3.available() != 0) {
    Serial.println("Incoming");
    delay(50);
  while (Serial3.available() != 0) {
    uint8_t XY = uint8_from_serial3();  //Reads the input from the Serial Monitor
    Serial.print("XY: "); Serial.println(XY);
    Parse(XY);
    X1 = X_coordinate; Serial.print("X: "); Serial.print(X1);
    Y1 = Y_coordinate; Serial.print(" Y: "); Serial.println(Y1);
    }
    Serial3.flush();
    Serial3.write('1');
    // uint8_to_serial3(1);
    if (grid1[X1][Y1] == 1) {
        Serial.println("HIT");
      }
    else if (grid1[X1][Y1] == 0) {
      Serial.println("MISS");
    }

    }
  }
  // 2. check grid


  // 3. write on Serial3

  // 4. wait for respomse


    // tft2.fillRect(5, 20, 120, 10, BLACK);
    // tft2.setCursor(5, 20);
    // tft2.setTextColor(WHITE, BLACK);
    // tft2.print("Game Start!");
    //
    // // Serial.println("Screen order 2");
    // select = digitalRead(SEL);
    // cursorMovement();
    // CheckEdges();
    //
    // if (select == LOW) {
    //
    //   updateOtherDisplay();
    // }


return 0;
Serial3.end();
Serial.end();
}
