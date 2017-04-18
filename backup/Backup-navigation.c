#include "functions.h"
#include <Arduino.h>

int right_button_pressed(int state) {
    if (state == 1) return 2;
    else if (state == 2) return 3;
    else if (state == 3) return 4;
    else if (state == 4) return 1;
    else if (state == 5) return 6;
    else if (state == 6) return 7;
    else if (state == 7) return 5;
    else if (state == 61) return 62;
    else if (state == 62) return 63;
    else if (state == 63) return 64;
    else if (state == 64) return 65;
    else if (state == 65) return 61;
    else if (state == 71) return 72;
    else if (state == 72) return 73;
    else if (state == 73) return 74;
    else if (state == 74) return 71;
    else return (-1);
}

int left_button_pressed(int state) {
    if (state == 1) return 4;
    else if (state == 2) return 1;
    else if (state == 3) return 2;
    else if (state == 4) return 3;
    else if (state == 5) return 7;
    else if (state == 6) return 5;
    else if (state == 7) return 6;
    else if (state == 61) return 65;
    else if (state == 62) return 61;
    else if (state == 63) return 62;
    else if (state == 64) return 63;
    else if (state == 65) return 64;
    else if (state == 71) return 74;
    else if (state == 72) return 71;
    else if (state == 73) return 72;
    else if (state == 74) return 73;
    else return (-1);
}
void state_fails(int state) {
    Serial.println("Something went wrong, state is out of bounds: ");
    Serial.println(state);
}

int both_buttons_pressed(int state){
    if (state <= 4) return 5;
    else if (state == 6) return 61;
    else if (state == 7) return 71;
    else if (state == 61) return 611;
    else if (state == 62) return 621;
    else if (state >= 71) return 1;
    else if (state > 4) return 1;
}
