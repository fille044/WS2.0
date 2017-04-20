#include "functions.h"
#include <Arduino.h>

/*
 * Simple functions that reads the value of all analog sensors and returns the value
 * Also the troubleshooting functions is here
*/

int read_soil() 
{
    int soil = analogRead(SOIL_SENSOR);
    return soil;
}

void print_to_serial(int state, int soil, int temp, int humidity, 
        int critical_soil, int warning_soil)
{
    Serial.print("State: ");
    Serial.println(state);
    Serial.print("Temp: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Soil: ");
    Serial.println(soil);
    Serial.print("Critical Soil level: ");
    Serial.println(critical_soil);
    Serial.print("Wardning Soil level: ");
    Serial.println(warning_soil);
    Serial.println(" ");
    Serial.println(" ------------ ");
    delay(2000);
}
