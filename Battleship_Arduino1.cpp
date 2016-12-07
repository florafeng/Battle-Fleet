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
uint8_t g_cursorX = 58; // cursor pixel position
uint8_t g_cursorY = 90;
uint8_t g_prevX = -1; // previously drawn position of the cursor
uint8_t g_prevY = -1;
uint8_t grid1[11][11]; // Two Game Grids (using X, Y from 1 to 10)
uint8_t grid2[11][11];

//Initialize Global Counters
uint8_t screen_order = 0;
int hit_counter = 0; // tracks hits for win condition
int turn_counter = 1; // useful for displaying turn number

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE   0xfa00
#define GREEN    0x07E0
uint16_t g_colour = ST7735_GREEN;

const uint16_t MY_YELLOW = tft.Color565(0xff, 0xff, 0x00);
typedef enum {Position_Check,
              Wait_For_ACK,
              Wait_Ship_Int,
              Send_Response,
              Move_Cursor,
              Wait_For_End,
              End} State;
char* stateNames[] = {"Position_Check",
                      "Wait_For_ACK",
                      "Wait_Ship_Int",
                      "Send_Response",
                      "Move_Cursor",
                      "Wait_For_End",
                      "End"};

//Joystick and Select Button Analog Inputs
const int VERT = 0;  // analog input
const int HORIZ = 1; // analog input
const int SEL = 3;   // digital input
int joystick_int_vert, joystick_int_horiz;
int select;


void joystickScan() {
  int joystick_vert = analogRead(VERT);
  int joystick_horiz = analogRead(HORIZ);

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
  delay(150);
}

void displayError(uint8_t num) {
  while(true) {
    if(screen_order == 0) {
      tft.fillRect(1, 1, 130, 34, ST7735_BLACK);
      tft.fillCircle(g_prevX, g_prevY, 5, RED);
      delay(300);
      tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
      delay(300);
      joystickScan();
      Serial.print("cursorX: "); Serial.println(g_cursorX);
      Serial.print("prevX: "); Serial.println(g_prevX);
      if(g_cursorX != g_prevX || g_cursorY != g_prevY) {
        tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
        tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
        if(num == 5) {
        tft.fillCircle(g_prevX, g_prevY, 5, ORANGE);
        }
        else if(num == 8) {
          tft.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
          tft.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
        }
        g_prevX = g_cursorX;
        g_prevY = g_cursorY;
        break;
      }
    }
    else if(screen_order == 1) {
      tft2.drawRect(1, 1, 130, 34, ST7735_BLACK); // fill with black first
      tft2.fillCircle(g_prevX, g_prevY, 5, RED);
      delay(300);
      tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
      delay(300);
      joystickScan();
      if(g_cursorX != g_prevX || g_cursorY != g_prevY) {
        tft.fillRect(g_prevX-5, g_prevY-5, 11, 11, ST7735_BLACK);
        if(num == 5) {
        tft2.fillCircle(g_prevX, g_prevY, 5, ORANGE);
        }
        else if(num == 8) {
          tft2.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
          tft2.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
        }
        g_prevX = g_cursorX;
        g_prevY = g_cursorY;
        break;
      }
    }
  }
}

void fontUpdate() {
  tft.fillRect(1, 1, 130, 34, BLACK);
  tft2.fillRect(1, 1, 130, 34, BLACK);
  if(screen_order == 0) {
    tft.setCursor(28,8);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("Please take");
    tft.setCursor(32,18);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("your move!");
    tft2.setCursor(28,8);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("Please wait");
    tft2.setCursor(18,18);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("for your turn!");
  }
  else if(screen_order == 1) {
    tft2.setCursor(28,8);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("Please take");
    tft2.setCursor(32,18);
    tft2.setTextColor(GREEN, BLACK);
    tft2.print("your move!");
    tft.setCursor(28,8);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("Please wait");
    tft.setCursor(18,18);
    tft.setTextColor(YELLOW, BLACK);
    tft.print("for your turn!");
  }
  // tft.fillRect(1, 20, 120, 10, BLACK);
  // tft.setCursor(14,8);
  // tft.setTextColor(YELLOW, BLACK);
  // tft.print("Turn:1");
  // tft.setCursor(20,18);
  // tft.setTextColor(YELLOW, BLACK);
  // tft.print("T/F:F");
  //
  // tft.setCursor(75,8);
  // tft.setTextColor(YELLOW, BLACK);
  // tft.print("Hit:0");
  // tft.setCursor(69,18);
  // tft.setTextColor(YELLOW, BLACK);
  // tft.print("Miss:0");
  //
  // tft.fillRect(1, 20, 120, 10, BLACK);
  // tft.setCursor(14,8);
  // tft.setTextColor(YELLOW, BLACK);
  // tft.print("Turn:1");
}

uint8_t checkOccupied() {
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;
  // Serial.print("X: "); Serial.println(X_coordinate);
  // Serial.print("Y: "); Serial.println(Y_coordinate);
  uint8_t gridValue0 = grid1[X_coordinate][Y_coordinate];
  uint8_t gridValue1 = grid2[X_coordinate][Y_coordinate];

  if(screen_order == 0) {
    return gridValue0;
  }
  else if(screen_order == 1) {
    return gridValue1;
  }

}

void updateOwnDisplay() {
  if (screen_order == 0) {
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
      tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
      tft.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
      // fix missing lane
      tft.drawFastVLine(g_prevX-6, g_prevY-6, 12, YELLOW);
      tft.drawFastHLine(g_prevX-6, g_prevY-6, 12, YELLOW);
      // store previous position
      g_prevX = g_cursorX;
      g_prevY = g_cursorY;

      uint8_t val = checkOccupied();
      if(val == 5) {
        displayError(5);
      }
      else if(val == 8) {
        displayError(8);
      }
      else{
        tft.fillCircle(g_prevX, g_prevY, 5, g_colour);
      }
    }
  }
  if (screen_order == 1){
    if((g_prevX != g_cursorX) || (g_prevY != g_cursorY) ) {
      tft2.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
      tft2.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
      // fix missing lane
      tft2.drawFastVLine(g_prevX-6, g_prevY-6, 12, GREEN);
      tft2.drawFastHLine(g_prevX-6, g_prevY-6, 12, GREEN);
      // store previous position
      g_prevX = g_cursorX;
      g_prevY = g_cursorY;

      uint8_t val = checkOccupied();
      if(val == 5) {
        displayError(5);
      }
      else if(val == 8) {
        displayError(8);
      }
      else{
        tft2.fillCircle(g_prevX, g_prevY, 5, YELLOW);
      }
    }
  }
}

void checkGrid() {
  // Prints Grid to Serial Mon
  hit_counter = 0;
  for (int y0=1; y0<=10; y0++) {
    for (int x0 =1; x0 <=10; x0++) {
      if (screen_order == 0) {
        if (grid1[x0][y0] == 1) {hit_counter = hit_counter +1;}
      }
      if (screen_order == 1) {
        if (grid2[x0][y0] == 1){ hit_counter = hit_counter +1;}
      }
    }
  }
  //Serial.println(hit_counter);
}

void updateGridPos(bool occupied) {
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;

  if (screen_order == 0) {
    // Serial.println("Current Position: ");
    if(occupied && grid1[X_coordinate][Y_coordinate] != 1){
      tft.fillCircle(g_prevX, g_prevY, 5, ORANGE);
      grid1[X_coordinate][Y_coordinate] = 5; // hit
    }
    else if(!occupied && grid1[X_coordinate][Y_coordinate] != 1){
      tft.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
      tft.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
      grid1[X_coordinate][Y_coordinate] = 8; // miss
    }
  }
  else if (screen_order == 1) {
    if(!occupied && grid2[X_coordinate][Y_coordinate] != 1){
      tft2.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft2.drawLine(g_prevX-6, g_prevY -6, g_prevX+6, g_prevY +6, WHITE);
      tft2.drawLine(g_prevX-6, g_prevY +6, g_prevX+6, g_prevY -6, WHITE);
      grid2[X_coordinate][Y_coordinate] = 8; // miss
    }
    else if(occupied && grid2[X_coordinate][Y_coordinate] != 1){
      tft2.fillCircle(g_prevX, g_prevY, 5, BLACK);
      tft2.fillCircle(g_prevX, g_prevY, 5, ORANGE);
      grid2[X_coordinate][Y_coordinate] = 5; // hit
    }
  }
  g_cursorX = 58; g_cursorY = 90;
}

void updateTestGridPos() {
  uint8_t X_coordinate = (g_prevX-2)/12;
  uint8_t Y_coordinate = (g_prevY-30)/12-1;

  tft.fillCircle(g_prevX, g_prevY, 5, ST7735_BLACK);
  tft.drawRect(g_prevX -6, g_prevY -6, 12, 12, ST7735_BLACK);
  tft.drawFastVLine(g_prevX-6, g_prevY-6, 12, YELLOW);
  tft.drawFastHLine(g_prevX-6, g_prevY-6, 12, YELLOW);
  tft.drawLine(g_prevX-6, g_prevY-6, g_prevX+6, g_prevY+6, WHITE);
  tft.drawLine(g_prevX-6, g_prevY+6, g_prevX+6, g_prevY-6, WHITE);
  grid1[X_coordinate][Y_coordinate] = 5;
  Serial.print("setGrid:"); Serial.println(X_coordinate);
  g_cursorX = 58; g_cursorY = 90;
}

void lcdInt() {
  tft.initR(INITR_BLACKTAB);
  tft2.initR(INITR_BLACKTAB);
  // fill screen
  tft2.fillScreen(BLACK);
  tft.fillScreen(ST7735_BLACK);
  // draw grid
  for (int16_t y = 36; y < 157; y += 12) {
  tft.drawFastHLine(4, y, 120, YELLOW);
  tft2.drawFastHLine(4, y, 120, GREEN);
  }
  for (int16_t x = 4; x < 125; x += 12){
  tft.drawFastVLine(x, 36, 120, YELLOW);
  tft2.drawFastVLine(x, 36, 120, GREEN);
  }
  tft2.fillCircle(g_prevX, g_prevY, 5, ST7735_GREEN);
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

void fontInt() {
  tft.fillRect(1, 20, 120, 10, BLACK);
  tft.setCursor(13,8);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("Please initialze");
  tft.setCursor(32,18);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("your ships!");

  tft2.fillRect(1, 20, 120, 10, BLACK);
  tft2.setCursor(13,8);
  tft2.setTextColor(GREEN, BLACK);
  tft2.print("Please initialze");
  tft2.setCursor(32,18);
  tft2.setTextColor(GREEN, BLACK);
  tft2.print("your ships!");
}

void setup() {
  lcdInt();    // fill screen, draw grid and intial cursor
  joystickInt();
  gridInt();   // initialze position array
  fontInt();
}

void checkEdge() {
    if (g_cursorY > 150) {
      g_cursorY = 42;
    }
    if (g_cursorY < 42) {
      g_cursorY = 150;
    }
    if (g_cursorX < 10) {
      g_cursorX = 118;
    }
    if (g_cursorX > 118) {
      g_cursorX = 10;
    }
}

void cursorMovement() {
  joystickScan();
  checkEdge();
  updateOwnDisplay();
}

void Winner() {
  if (screen_order == 0) {
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

// send pressed coordinate to server;
uint8_t sendPosition() {
  uint8_t ret;
  if(screen_order == 0) {
    ret = (g_cursorX+2)/12*10 + (g_cursorY-30)/12; // convertion to grid index
  }
  else if(screen_order == 1) {
    ret = (g_cursorX+2)/12*10 + (g_cursorY-30)/12;
  }
  return ret;
}

// each two times, increment turn count and display
void updateTurnCount() {
  tft.fillRect(1, 20, 120, 10, BLACK);
  tft.setCursor(14,8);
  tft.setTextColor(YELLOW, BLACK);
  tft.print("Turn:1");

  tft2.fillRect(1, 20, 120, 10, BLACK);
  tft2.setCursor(14,8);
  tft2.setTextColor(YELLOW, BLACK);
  tft2.print("Turn:1");
}

void switchTurn() {
  if (screen_order == 0) {
    screen_order += 1;
  }
  else if (screen_order == 1) {
    screen_order -= 1;
    turn_counter++;
    // updateTurnCount();
  }
  delay(200);
}

// write to serial3
void uint8_to_serial3(uint32_t num) {
  Serial3.write((char) (num >> 0));
}

// read from serial3
uint8_t uint8_from_serial3() {
  uint8_t num = 0;
  num = num | ((uint8_t) Serial3.read()) << 0;
  return num;
}

// wait to grab data
bool wait_on_serial3( uint8_t nbytes, long timeout ) {
  unsigned long deadline = millis() + timeout;
  while (Serial3.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1);
  }
  return Serial3.available()>=nbytes;
}

// check the returning value(0 -> MISS)
bool checkResp(uint8_t response) {
  if(response == 0){
    return false;
  }else{
    return true;
  }
}

// indicating whether screens are set up on arduino two
bool receive_CR() {
	if(wait_on_serial3(1, 1000) && Serial3.read() == 'C') {
		return true;
	}else {
		return false;
	}
}

// 'OK', you may continue to next state
bool receive_ACK() {
  if(wait_on_serial3(2, 1000) && (Serial3.read() == 'K')) {
    return true;
  }else {
    return false;
  }
}

// sub state machine, communicate with server
bool client(uint8_t pos) {
  bool occupied;
  // Serial.println("Inside client loop: ");
  State client = Position_Check;

  while(client != End) {
    if(client == Position_Check) {
      Serial3.write('A'); // send ack to server
      uint8_to_serial3(pos);
      client = Wait_For_ACK;
    }
    else if((client == Wait_For_ACK) && receive_ACK()) {
      uint8_t response = uint8_from_serial3();
      occupied = checkResp(response);
      Serial3.write('E'); // send end message
      client = End;
      checkGrid();
      // if (hit_counter == 17) {
      //     Winner();
      //     break;
      // }
      break;
    }
    else {
      client = Position_Check;
      // Serial.print("State change to: ");Serial.println("Position_Check");
    }
  }
  return occupied;
}


int main() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  setup();
  // state machine one: useful for ship setup check & placement of guessing & determination of game end
  State current = Wait_Ship_Int;
  while (true) {
    select = digitalRead(SEL);
    if(current == Wait_Ship_Int && receive_CR()){ // setup complete on arduino two
      current = Move_Cursor;
    }
    else if(current == Move_Cursor) { // move cursor to take the guess
      cursorMovement();
      if(select == LOW){
        uint8_t curr_pos = sendPosition();
        bool occupied = client(curr_pos);
        updateGridPos(occupied);
        checkGrid();
        if (hit_counter == 17) {
            Winner();
            break;
          }
        switchTurn();
        fontUpdate();
        delay(150);
      }
    }
    else{
      current = Wait_Ship_Int;
      // Serial.println("wait ship int");
    }
}

  Serial.end();
  Serial3.end();
  return 0;
}
