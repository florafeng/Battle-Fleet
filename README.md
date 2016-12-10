Project: Arduino Battleship

Author: Sida Feng 1463129 Section A2
        Jack (Cheng Yi) Zhao 1431058 Section A2

Introduction
This project replicates Battleship, the classic grid-based board game, through two Arduinos and four LCD displays. Each player will have two 10×10 LCD grids. On one grid the player arranges ships and records the shots by the opponent. On the other grid the player records their own shots. Initially, each player arranges their five ships on their primary grid, the ships cannot overlap or move beyond the grid. After the ships have been positioned, the game proceeds in a series of rounds. In each round, each player takes a turn to announce a target square in the opponent's grid which is to be shot at. The opponent announces whether or not the square is occupied by a ship, and if it is a "miss", the player marks their primary grid with a white cross; if a "hit" they mark this on their own primary grid with a orange circle. If all of a player's ships have been sunk, the game is over and their opponent wins.

Accessories:
* Arduino Mega Board (AMG) x 2
* Joystick with clickable button x 2
* Adafruit 2.8' 128x160 TFT LCD Display x 4
* USB cables x 2
* Wires x 85

Wiring instructions 

Wiring method for LCDs, Joystick, and Serial Communication are adapted from standard University of Alberta Tangible Computing Library.

LCD: Both displays share MOSI (Master Out Slave In), SCK (Clock),
MISO (Master In Slave Out), and CARD_CS (Card Chip Select) pins (51, 52, 50, 5, respectively), and should be shared between the two. GND, VCC, and Backlite connect to ground and positive on the breadboard, respectively.

For Reset, Date/Command, and TFT Chip Select, first LCD should be connected to digital pins 8, 7, and 6, while the second should be connected to 12, 9, and 13, respectively.

Joystick: VCC and GND connect to positive and GND on the breadboard.
VERT and HOR should be connected to analog pins A0 and A1, respectively, while SEL connects to digital pin 9.

Communication:
Arduino_1 GND <—->  Arduino_2 GND
Arduino_1 TX3 14 <-->  Arduino_2 RX3 15
Arduino_2 TX3 14 <-->  Arduino_1 RX3 15

Wiring/Pins are the same for both Arduinos.

Procedure
1. Uncompress the compressed folder or download the source file by running 'git clone git@github.com:florafeng/Arduino-Battleship.git' in your terminal.
2. Go into the ArduinoOne folder.
3. Connect the first arduino to your computer through USB cable.
4. Upload to the first arduino
4. Repeat step 1-4 with the ArduinoTwo folder on a second arduino.

Instructions:
Wire each arduino as shown in the "Wiring Instructions" image.
On one Arduino, upload everything in Arduino2. This contains the Ship screens, including the setup. On a second Arduino, upload everything in Arduino1. Complete the setup procedure by setting 5 ships on both screens. Ensure that Arduino1 has fully uploaded, and that communication pins are fully connected prior to setting the final ship. When Setup is complete on Arduino2, communication will begin
and Arduino1 will be prompted to play. During the game, each button press will trigger a turn change, flipping the active screen back and forth. For the game players grids are color-coded and lined up so Yellow grid plays based on Yellow grid, vice-versa with Green grids. First player to completely hit down all ships on their grid wins, prompting the end of the game.

Extra info
1. Pin settings can be edited in config.h file, in case you have a different arduino or prefer some other wirings.
2. This program is designed to upload two different set of code to two arduinos. This means you will upload the code from each folders on two computers. Alternately, switching ports will enable uploading different code to different arduinos with a single computer. To achieve this, run 'arduino-port-select' and choose port one and port two respectively for each arduinos.
3. Buidling with a Makefile through the Terminal is recommended.
4. If you have encountered any problems, you are more than welcome to contact the author by emailing to sfeng3@ualberta.ca.


