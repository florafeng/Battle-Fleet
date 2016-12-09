#include <Arduino.h>
#include "config.h"
#include "select.h"

void Select_Init() {
  pinMode(SEL, INPUT);
  digitalWrite(SEL, HIGH);
}

int Select_Read() {
    select = digitalRead(SEL);
    if (select == LOW) {select = 0;}
    else if (select == HIGH) {select = 1;}
    return select;
}
