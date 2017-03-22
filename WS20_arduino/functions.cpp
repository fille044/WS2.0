#include "functions.h"
#include <Arduino.h>
#define PHOTOTRANSISTOR A0
#define MOIST_SENSOR A1
#define SOIL_SENSOR A2
#define SOIL_WARNING 650
#define SOIL_CRITICAL 850

int read_light() {
    int light = analogRead(PHOTOTRANSISTOR);
    //high_low_light(light);
    // Print light-value to large LCD screen
    return light;
}

int read_moist() {
    int moist = analogRead(MOIST_SENSOR);
    return moist;
}

int read_soil() {
    int soil = analogRead(SOIL_SENSOR);
    return soil;
}

void print_to_serial(int state, int light, int moist, int soil, int temp, int humidity){
    Serial.print("State: ");
    Serial.println(state);
    Serial.print("Light: ");
    Serial.println(light);
    Serial.print("Moist: ");
    Serial.println(moist);
    Serial.print("Temp: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Soil: ");
    Serial.println(soil);
    Serial.println(" ------------ ");
}
