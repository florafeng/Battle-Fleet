#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "game.h"
#include "config.h"
#include "display.h"
#include "joystick.h"
#include "display.h"
#include "server.h"
/*
 * Communication between two Arduino boards
 */
#include <Arduino.h>
#include "comm_server.h"

//For State Machine
typedef enum {Listen, Initial, Wait_For_Pos, Send_Response, Wait_For_End, End} State;
char* stateNames[] = {"Listen", "Initial", "Wait_For_Pos", "Send_Response", "Wait_For_End", "End"};

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
