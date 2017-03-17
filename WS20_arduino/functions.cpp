#include "functions.h"
#include <Arduino.h>
#define PHOTOTRANSISTOR A0
#define MOIST_SENSOR A1
#define SOIL_SENSOR A2

int right_button_pressed(int state) {
    if (state == 1) return 2;
    else if (state == 2) return 3;
    else if (state == 3) return 4;
    else if (state == 4) return 1;
    else return (-1);
}

int left_button_pressed(int state) {
    if (state == 1) return 4;
    else if (state == 2) return 1;
    else if (state == 3) return 2;
    else if (state == 4) return 3;
    else return (-1);
}
void state_fails(int state) {
    Serial.println("Something went wrong, state is out of bounds: ");
    Serial.println(state);
}
int read_light() {
    int light = analogRead(PHOTOTRANSISTOR);
    //high_low_light(light);
    Serial.println(light);
    // Print light-value to large LCD screen
    return light;
}

int read_moist() {
    int moist = analogRead(MOIST_SENSOR);
    Serial.println(moist);
    return moist;
}

int read_soil() {
    int soil = analogRead(SOIL_SENSOR);
    Serial.println(soil);
    return soil;
}
