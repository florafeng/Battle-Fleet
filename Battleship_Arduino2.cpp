#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define SD_CS 5 // Shared Pin
// Screen 1 Pins
#define TFT_CS 6
#define TFT_DC 7
#define TFT_RST 8
// Screen 2 Pins
#define DC 9
#define CS 13
#define RST 12

#define TFT_WIDTH 128
#define TFT_HEIGHT 160

#define MILLIS_PER_FRAME 50 // 20fps

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ST7735 tft2 = Adafruit_ST7735(CS, DC, RST);

//Initialize Global (Position) Variables
int g_cursorX = 58; // cursor pixel position
int g_cursorY = 90;
int g_prevX = 58; // previously drawn position of the cursor
int g_prevY = 90;
int X_coordinate = 0; // Grid coordinate position
int Y_coordinate = 0;
int grid1[11][11]; // Two Game Grids (using X, Y from 1 to 10)
int grid2[11][11];

//Initialize Global Counters
int ship_counter = 0;
int screen_order = 0;
int ship_order = 0;
int ships_down = 0;
int ships_remaining = 5;

//Colours to be used
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE   0xfa00
#define GREEN    0x07E0
uint16_t g_colour = GREEN;

//For State Machine
typedef enum {Listen, Initial, Wait_For_Pos, Send_Response, Wait_For_End, End} State;
char* stateNames[] = {"Listen", "Initial", "Wait_For_Pos", "Send_Response", "Wait_For_End", "End"};

//Joystick and Select Button Analog Inputs
const int VERT = 0;
const int HORIZ = 1;
const int SEL = 3;
int joystick_int_vert, joystick_int_horiz;
int select;

//Based on Grid, returns 2 counters for Ships on Grid

void checkGrid() {
// Prints Grid to Serial Mon
ships_down = 0; ships_remaining = 5;
ship_counter = 0;
  int counter5 = 5;  int counter4 = 4;
  int counter3 = 3;  int counter3_2 = 3;
  int counter2 = 2;

  for (int y0=0; y0<=10; y0++) {
      for (int x0 =0; x0 <=10; x0++) {
          if (screen_order == 0) {
              if (grid1[x0][y0] == 5) { counter5 = counter5 - 1; ship_counter++;}
              if (grid1[x0][y0] == 4) { counter4 = counter4 - 1; ship_counter++;}
              if (grid1[x0][y0] == 3) { counter3 = counter3 - 1; ship_counter++;}
              if (grid1[x0][y0] == 1) { counter3_2 = counter3_2 - 1; ship_counter++;}
              if (grid1[x0][y0] == 2) { counter2 = counter2 - 1; ship_counter++;}
          }
          if (screen_order == 1) {
              if (grid2[x0][y0] == 5) { counter5 = counter5 - 1; ship_counter++;}
              if (grid2[x0][y0] == 4) { counter4 = counter4 - 1; ship_counter++;}
              if (grid2[x0][y0] == 3) { counter3 = counter3 - 1; ship_counter++;}
              if (grid2[x0][y0] == 1) { counter3_2 = counter3_2 - 1; ship_counter++;}
              if (grid2[x0][y0] == 2) { counter2 = counter2 - 1; ship_counter++;}
          }
      }
  }
  if (counter5 == 5) { ships_remaining--; ships_down++;}
  if (counter4 == 4) { ships_remaining--; ships_down++;}
  if (counter3 == 3) { ships_remaining-- ;ships_down++;}
  if (counter3_2 == 3) { ships_remaining--; ships_down++;}
  if (counter2 == 2) { ships_remaining--; ships_down++;}
  // Serial.println(ships_remaining);
  // Serial.println(ships_down);
}

//Updates Dashboard based after checking Ships Remaining/Down
void checkShips() {
    checkGrid();
    if (screen_order == 0) {
        tft.fillRect(0, 1, 128, 33, BLACK);
        tft.setCursor(4, 1); tft.setTextSize(1);
        tft.setTextColor(GREEN, BLACK);
        tft.print("Ships Remaining: "); tft.println(ships_remaining);
        tft.println(); tft.setCursor(4, 20);
        tft.print("Ships Down: "); tft.print(ships_down);
    }
    if (screen_order == 1) {
        tft2.fillRect(0, 1, 128, 33, BLACK);
        tft2.setCursor(4, 1);
        tft2.setTextColor(YELLOW, BLACK);
        tft2.print("Ships Remaining: "); tft2.println(ships_remaining);
        tft2.println(); tft2.setCursor(4, 20);
        tft2.print("Ships Down: "); tft2.print(ships_down);
    }
}

//Edge Case for each specific ship based on Cursor position
void ShipEdgeCases() {
    if (g_cursorY > 150) {g_cursorY = 42;}
    if (g_cursorY < 42) {g_cursorY = 150;}

    if (ship_order == 0){
        if (g_cursorX <34) {g_cursorX = 94;}
        if (g_cursorX > 94) {g_cursorX = 34;}
    }

    if (ship_order == 1){
        if (g_cursorX <34) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 34;}
    }

    if ((ship_order == 2) || (ship_order == 3)){
        if (g_cursorX <22) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 22;}
    }
    if (ship_order == 4){
        if (g_cursorX <10) {g_cursorX = 106;}
        if (g_cursorX > 106) {g_cursorX = 10;}
    }
}

void checkEdges() {
  if (screen_order == 0) {
      ShipEdgeCases();
  }
  if (screen_order == 1) {
      ShipEdgeCases();
  }
}

//Movement Along the grid in increments of 12 Pixels
void cursorMovement() {
    int joystick_vert = analogRead(VERT);
    int joystick_horiz = analogRead(HORIZ);

    if (joystick_vert >= 800) {
      g_cursorY += 12;}
    else if (joystick_vert <= 200) {
      g_cursorY -= 12;}
    else if (joystick_horiz <= 200) {
      g_cursorX -= 12;}
    else if (joystick_horiz >= 800) {
      g_cursorX += 12;}
}

//Remove Ship of (length) based on current grid position
void Black(int length) {
    //Actual Cursor always at C
    int start = g_prevX -4 +((length-2)*-12);
    if (length == 1) {start = g_prevX -4; }
    if ((start - g_prevX) <= -28) { start = g_prevX -28;}
    int end = g_prevX + 20;
    if(length != 5) {end = g_prevX + 16;}
    if (length ==1) {end = g_prevX +7;}

    for (start; start<= end; start+=12) {
        if (screen_order == 0) {
            tft.fillRect(start, g_prevY-4, 10, 10, BLACK);
        }
        if(screen_order == 1) {
            tft2.fillRect(start, g_prevY-4, 10, 10, BLACK);
        }
    }
}

//Create Ship of (length) based on current grid position
void Ship(int length) {
    int start = g_prevX -4 +((length-2)*-12);
    if (length == 1) {start = g_prevX -4; }
    if ((start - g_prevX) <= -28) { start = g_prevX -28;}
    int end = g_prevX + 20;
    if(length != 5) {end = g_prevX + 16;}
    if (length ==1) {end = g_prevX +7;}

    for (start; start<= end; start+=12) {
        if (screen_order == 0) {
            tft.fillRect(start, g_prevY-4, 9, 9, WHITE);
        }
        if(screen_order == 1) {
            tft2.fillRect(start, g_prevY-4, 9, 9, WHITE);
        }
    }
}

//Obtain XY grid coordinates based on current position
void getCoordinates() {
  X_coordinate = (g_prevX+2)/12;  Y_coordinate = (g_prevY-30)/12;
}

//Registers Ships to grid based on screen_order, position, and length
void RegisterGrid(int X_coordinate, int Y_coordinate, int Registry) {
    int X, X_upper;
    if (ship_order <= 2) {X = X_coordinate -2; X_upper = X_coordinate +2;}
    else if ((ship_order >2) && (ship_order <5)) {X = X_coordinate -1;}
    else if (ship_order == 5) {X = X_coordinate;}

    if (ship_order != 1) {X_upper = X_coordinate +1;}
    while(X <= X_upper) {
        if (screen_order == 0)  {grid1[X][Y_coordinate] = Registry;}
        else if (screen_order == 1){grid2[X][Y_coordinate] = Registry;}
        X = X++;
    }
}

void fillGrid() {
    if (ship_order == 1) {
        Ship(5);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 5);
    }
    if (ship_order == 2) {
        Ship(4);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 4);
    }
    if (ship_order == 3) { // 3 Tile ship
        Ship(3);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 3);
    }
    if (ship_order == 4) { // 3 Tile Ship
        Ship(3);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 1);
    }
    if (ship_order == 5) {
        Ship(2);
        getCoordinates();
        g_cursorX = 58; g_cursorY = 90;
        RegisterGrid(X_coordinate, Y_coordinate, 2);
    }
}


// Checks grid and leaves occupied spaces filled
void CheckShipGrid(int X, int Y) {
  int X0 = X - 6; int X1 = X+5;

  if (screen_order == 0) {
      for (X0; X0<X1; X0++) {
          if ((X0 <1) || (X0 > 10)) {;}
          else if (grid1[X0][Y] != 0) {
              g_prevX = (X0)*12 -2;
              g_prevY = (Y)*12 +30;
              Ship(1);
          }
      }
  }
  if (screen_order == 1) {
      for (X0; X0<X1; X0++) {
          if ((X0 <1) || (X0 > 10)) {;}
          else if (grid2[X0][Y] != 0) {
              g_prevX = (X0)*12 -2;
              g_prevY = (Y)*12 +30;
              Ship(1);
          }
      }
  }
}

// Generate ships (as cursors) for 5 different lengths
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

//Cursor change size based on ship_order
void updateCursor() {

    if (ship_order == 0) {Carrier();}
    if (ship_order == 1) {Battleship();}
    if ((ship_order == 2) || (ship_order == 3)) {Cruiser();}
    if (ship_order == 4) {Destroyer();}
}


//Winning Messages based on Player who wins
void Winner() {
    if (screen_order == 0) {
        tft2.fillRect(0, 0, 128, 33, BLACK);
        tft2.setCursor(1, 1); tft2.setTextSize(2);
        tft2.setTextColor(WHITE, BLACK);
        tft2.print("GAME OVER!");
        tft.fillRect(0, 0, 128, 33, BLACK);
        tft.setCursor(1, 1); tft.setTextSize(2);
        tft.setTextColor(WHITE, BLACK);
        tft.print("WINNER!");
        delay(750);
        tft.fillRect(0, 0, 128, 33, BLACK);
        tft.setCursor(1, 1); tft.setTextSize(2);
        tft.setTextColor(BLACK, WHITE);
        tft.print("PLAYER 1");
        delay(750);
        tft.fillRect(0, 0, 128, 33, BLACK);
        tft.setCursor(1, 1); tft.setTextSize(2);
        tft.setTextColor(WHITE, BLACK);
        tft.print("WINNER!");
        delay(750);
        tft.fillRect(0, 0, 128, 33, BLACK);
        tft.setCursor(1, 1); tft.setTextSize(2);
        tft.setTextColor(BLACK, WHITE);
        tft.print("PLAYER 1");
        delay(750);
        tft.setCursor(1, 25); tft.setTextSize(1);
        tft.setTextColor(WHITE, BLACK);
        tft.print("CONGRATULATIONS!");
    }
    if (screen_order == 1) {
        tft.fillRect(0, 0, 128, 33, BLACK);
        tft.setCursor(1, 1); tft.setTextSize(2);
        tft.setTextColor(WHITE, BLACK);
        tft.print("GAME OVER!");
        tft2.fillRect(0, 0, 128, 33, BLACK);
        tft2.setCursor(1, 1); tft2.setTextSize(2);
        tft2.setTextColor(WHITE, BLACK);
        tft2.print("WINNER!");
        delay(750);
        tft2.fillRect(0, 0, 128, 33, BLACK);
        tft2.setCursor(1, 1); tft2.setTextSize(2);
        tft2.setTextColor(BLACK, WHITE);
        tft2.print("PLAYER 2");
        delay(750);
        tft2.fillRect(0, 0, 128, 33, BLACK);
        tft2.setCursor(1, 1); tft2.setTextSize(2);
        tft2.setTextColor(WHITE, BLACK);
        tft2.print("WINNER!");
        delay(750);
        tft2.fillRect(0, 0, 128, 33, BLACK);
        tft2.setCursor(1, 1); tft2.setTextSize(2);
        tft2.setTextColor(BLACK, WHITE);
        tft2.print("PLAYER 2");
        delay(750);
        tft2.setCursor(1, 25); tft2.setTextSize(1);
        tft2.setTextColor(WHITE, BLACK);
        tft2.print("CONGRATULATIONS!");
    }
}

//Game Mode: Update screens based on Serial Communication
void updateShipGrid(uint8_t posX, uint8_t posY, uint8_t res) {
    uint8_t pixelX = posX*12 - 2;
    uint8_t pixelY = posY*12 + 30;

    if (screen_order == 0) {
        Serial.println("screen1 update grid");
        if(res == 0){ //miss
            grid1[X_coordinate][Y_coordinate] = 10; // occupied with guessed position
            tft.drawLine(pixelX-6, pixelY -6, pixelX+6, pixelY +6, WHITE);
            tft.drawLine(pixelX-6, pixelY +6, pixelX+6, pixelY -6, WHITE);
            delay(150);
        }
        else if(res != 0) { //hit
            grid1[X_coordinate][Y_coordinate] = 11; // occupied with guessed position
            tft.fillRect(pixelX -6, pixelY -6, 13, 13, BLACK);
            tft.drawRect(pixelX -6, pixelY -6, 13, 13, YELLOW);
            tft.fillCircle(pixelX, pixelY, 5, ORANGE);
            delay(150);
        }
    }
    else if (screen_order == 1) {
        Serial.println("two updating");
        if(res == 0){ //miss
            grid2[X_coordinate][Y_coordinate] = 10;
            tft2.drawLine(pixelX-6, pixelY -6, pixelX+6, pixelY +6, WHITE);
            tft2.drawLine(pixelX-6, pixelY +6, pixelX+6, pixelY -6, WHITE);
            delay(150);
        }
        else if(res != 0) { //hit
            grid2[X_coordinate][Y_coordinate] = 11; // occupied with guessed position
            tft2.fillRect(pixelX -6, pixelY -6, 13, 13, BLACK);
            tft2.drawRect(pixelX -6, pixelY -6, 13, 13, GREEN);
            tft2.fillCircle(pixelX, pixelY, 5, ORANGE);
            delay(150);
        }
    }
}

// Establish Communication Functions

// Send to Serial3
void uint8_to_serial3(uint8_t num) {
    Serial3.write((char) (num >> 0));
}

// Read from serial3
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


bool receive_pos() {
    if(wait_on_serial3(2, 1000) && (Serial3.read() == 'A')) {
        return true;
    }else {
        return false;
    }
}

bool receive_end() {
    if(wait_on_serial3(1, 1000) && (Serial3.read() == 'E')) {
        return true;
    }else {
        return false;
    }
}

void switchTurn() {
    if (screen_order == 0) {
        screen_order += 1;
    }
    else if (screen_order == 1) {
        screen_order -= 1;
    }
    delay(400);
}

// Parses into XY coordinates, then checks/updates grid, and returns grid value
uint8_t checkCoor(uint8_t position) {
    uint8_t res1, res2; // Useful for storing response
    Serial.println(position);

    if (position == 1010) {X_coordinate= 10; Y_coordinate= 10;}
    if ((position > 99) && (position < 110)) {X_coordinate= 10; Y_coordinate= position%10;}
    else if (position > 109) {X_coordinate= position/1000; Y_coordinate= 10;}
    else if (position < 100) {X_coordinate= position/10; Y_coordinate= position%10;}

    if(screen_order == 0) {
        res1 = grid1[X_coordinate][Y_coordinate];
        updateShipGrid(X_coordinate, Y_coordinate, res1);
        return (res1);
    }
    else if (screen_order == 1){
        res2 = grid2[X_coordinate][Y_coordinate];
        updateShipGrid(X_coordinate, Y_coordinate, res2);
        return (res2);
    }
}

// Active Game Server for Communication between Arduinos
void server() {
    State server = Initial;
    uint8_t response;
    while(server != End) {
        if(server == Initial) {
            Serial3.write('C');
            server = Wait_For_Pos;
        }
        else if((server == Wait_For_Pos) && receive_pos()){
            uint8_t pos = uint8_from_serial3();
            response = checkCoor(pos);
            Serial.print("screen_order: "); Serial.println(screen_order);
            //Check Win Conditions
            checkShips();
            if (ships_down == 5) {
                Winner();
                break; // Only way to exit loop is through winning
            }
            server = Send_Response;
            switchTurn();
          }
        else if(server == Send_Response) {
            Serial3.write('K');
            uint8_to_serial3(response);
            server = Wait_For_End;
        }
        else if((server == Wait_For_End) && receive_end()){
            server = Wait_For_Pos;
        }
        else{
            server = Wait_For_Pos;
        }
    }
}

// Initial Setup

//Initiates Joystick Select Button
void joystickInt() {
    pinMode(SEL, INPUT);
    digitalWrite(SEL, HIGH);
}

//Fills Grids with 0s (Empty Arrays)
void gridInt() {
    for (int q = 0; q <= 10; q++) {
        for (int p = 0; p<=10; p++) {
            grid1[q][p] = 0;
            grid2[q][p] = 0;
        }
    }
}

void clearGrid() {
    gridInt();
    g_prevX = 10; g_prevY = 42;
    for (g_prevY; g_prevY<=150; g_prevY+= 12) {
        for (g_prevX = 10; g_prevX<= 118; g_prevX+=12) {
            Black(1);
        }
    }
    if (screen_order == 0) {
        tft.fillRect(0, 0, 128, 35, BLACK);
        tft.setCursor(5, 5);
        tft.setTextColor(WHITE);
        tft.print("Invalid: Please Try Again");
        delay(1000);
        tft.fillRect(0, 0, 128, 35, BLACK);
    }
    if (screen_order == 1) {
        tft2.fillRect(0, 0, 128, 35, BLACK);
        tft2.setCursor(5, 5);
        tft2.setTextColor(WHITE);
        tft2.print("Invalid: Please Try Again");
        delay(1000);
        tft2.fillRect(0, 0, 128, 35, BLACK);
    }
}

//Initiates 2 LCDs with 10x10 grids of color
void lcdInt() {
    tft.initR(INITR_BLACKTAB);
    tft2.initR(INITR_BLACKTAB);
    // fill screen
    tft2.fillScreen(BLACK);
    tft.fillScreen(BLACK);
    // draw grid
    // tft.fillRect(5, 37, 120, 120, BLUE);
    for (int16_t y = 36; y < 157; y += 12) {
        tft.drawFastHLine(4, y, 120, YELLOW);
        tft2.drawFastHLine(4, y, 120, GREEN);
    }
    for (int16_t x = 4; x < 125; x += 12){
        tft.drawFastVLine(x, 36, 120, YELLOW);
        tft2.drawFastVLine(x, 36, 120, GREEN);
    }
    delay(150);
}

void SetupMessage() {
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(WHITE, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(WHITE, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(GREEN, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(GREEN, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 30, BLACK);
  tft.setCursor(13, 1); tft.setTextSize(2);
  tft.setTextColor(YELLOW, BLACK);
  tft.println("Arduino"); tft.print("Battleship");
  tft2.fillRect(0, 0, 128, 30, BLACK);
  tft2.setCursor(13, 1); tft.setTextSize(2);
  tft2.setTextColor(YELLOW, BLACK);
  tft2.println("Arduino"); tft.print("Battleship");
  delay(500);
  tft.fillRect(0, 0, 128, 33, BLACK);
  tft.setCursor(1, 20); tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.println("Please Set Your Ships");
  tft2.fillRect(0, 0, 128, 33, BLACK);

}
//Full Setup with Confirmation Display Message
void Setup() {
    lcdInt();    // fill screen, draw grid and intial cursor
    joystickInt();
    gridInt();   // initialze position array

}

void StartMessage() {
  tft.fillRect(0, 0, 128, 33, BLACK);
  tft.setCursor(1, 20); tft.setTextSize(1);
  tft.setTextColor(WHITE, BLACK);
  tft.println("GAME START!");
  tft2.fillRect(0, 0, 128, 33, BLACK);
  tft2.setCursor(1, 20); tft2.setTextSize(1);
  tft2.setTextColor(WHITE, BLACK);
  tft2.println("GAME START!");
  delay(500);
}
int main() {
    init();
    Serial.begin(9600);
    Serial3.begin(9600);
    Setup();

    // Set up ships on First Grid
    while(screen_order == 0) {

        select = digitalRead(SEL);
        cursorMovement();
        checkEdges();
        updateCursor();
        if ((select == 0) && (screen_order == 0)){
            ship_order++;
            fillGrid();
            delay(250);
        }
        if (ship_order == 5) {
            tft.fillRect(0, 0, 128, 25, BLACK);
            tft.setCursor(5, 20); tft.setTextSize(1);
            tft.setTextColor(BLACK, WHITE);
            tft.print("Setup Complete!");
            checkGrid();
              Serial.println(ship_counter);
            if (ship_counter < 17) {
              clearGrid();
              ship_order=0;
              screen_order=0;
              continue;
            }
            checkShips();

            ship_order = 0;
            screen_order = 1;
            break;
        }
    }
    // Recenter cursor
    tft2.fillRect(0, 0, 128, 30, BLACK);
    tft2.setCursor(1, 20); tft2.setTextSize(1);
    tft2.setTextColor(WHITE, BLACK);
    tft2.println("Please Set Your Ships");

    g_cursorX = 58; g_cursorY = 90;
    // Set up Ships on Second Grid
    while(screen_order == 1) {
        select = digitalRead(SEL);
        cursorMovement();
        checkEdges();
        updateCursor();
        if ((select == 0) && (screen_order == 1)){
            ship_order++;
            fillGrid();
            delay(250);
        }
        if (ship_order == 5) {
            delay(100);
            tft2.fillRect(0, 0, 128, 33, BLACK);
            tft2.setCursor(5, 20);
            tft2.setTextColor(BLACK, WHITE);
            tft2.print("Setup Complete!");
            checkGrid();
            if (ship_counter < 17) {
              clearGrid();
              screen_order=1;
              ship_order=0;
              continue;
            }
            checkShips();
            screen_order = 2;
            break;
        }
    }
    // Reset screen order for Active Game Loop
    StartMessage();
    screen_order = 0;
    // Active Game Loop
    while(true) {
        Serial.print("Sending CR");
        Serial3.print("C");
        server();
        //Should Never Break unless through Winner
    }

    return 0;
    Serial3.end();
    Serial.end();
}
