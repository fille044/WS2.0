#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Arduino.h>
#include <Wire.h>
#include "functions.h"

#define DHTPIN_INDOORS 8
#define DHTTYPE DHT11
#define PHOTOTRANSISTOR A0
#define MOIST_SENSOR A1
#define POSITIVE 0
#define LCD_ADDRESS 0x3F
#define RIGHT_BUTTON 12
#define LEFT_BUTTON 11
#define SOIL_SENSOR A2

DHT dht_indoors(DHTPIN_INDOORS, DHTTYPE);
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7);  // Set the LCD I2C address

void setup() {
    Serial.begin(9600);
    dht_indoors.begin();
    rtc.begin();
    Wire.begin();
    lcd.begin (16,2); //  <<----- My LCD was 16x2
    // Turn on the backlight
    lcd.setBacklightPin(3, POSITIVE);
    pinMode(RIGHT_BUTTON, INPUT);
    pinMode(LEFT_BUTTON, INPUT);

    /*
    * Uncomment to set date and time
    rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
    rtc.setTime(10, 35, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(17, 3, 2017);   // Set the date to 16-03-2017
    */
}

/*
 * int state is a preparation for the navigation that may be implemented into the project
 * If the user is in page 2, state will be set to 2, and may or may not show the content.
 */

void print_Clock(int state) {
    // Send day of week
    Serial.print(rtc.getDOWStr());
    Serial.print(" ");
    // Send date
    Serial.print(rtc.getDateStr());
    Serial.print(" -- ");
    // Send time
    Serial.println(rtc.getTimeStr());
    // Print date and time to large LCD screen
    if (state == 1) {
        lcd.setCursor (0,0);
        lcd.print(rtc.getDateStr());
        lcd.setCursor (0,1);
        lcd.print(rtc.getTimeStr());
        lcd.print(" ");
        lcd.print(rtc.getDOWStr());
    }
}

int read_temp(int state) {
    int temp = dht_indoors.readTemperature();
    Serial.println(temp);
    // Print temp to large LCD screen
    if (state == 2) {
        lcd.setCursor (0,0);
        lcd.print(temp);
        lcd.print(" C");
    }
    return temp;
}

int read_humidity(int state) {
    int humidity = dht_indoors.readHumidity();
    Serial.println(humidity);
    // Print humidity to large LCD screen
    if (state == 2) {
        lcd.setCursor (0,1);
        lcd.print(humidity);
        lcd.print(" % RH");

    }
    return humidity;
}

void high_low_light(int light){
    if (light > 250)
        lcd.setBacklight(HIGH);
    else if (light <= 250)
        lcd.setBacklight(LOW);
}

void print_light_moist_LCD(int state, int light, int moist){
    high_low_light(light);

    if (state == 3) {
        lcd.setCursor (0,0);
        lcd.print(moist);
        lcd.print(" moistness");
        lcd.setCursor (0,1);
        lcd.print(light);
        lcd.print(" lumens");
    }
}

void print_soil_LCD(int state, int soil){
    if (state == 4) {
        lcd.setCursor (6,0);
        lcd.print("OH SHIT");
        lcd.setCursor (0,1);
        lcd.print("Page for soil");
    }
}

void loop() {
    static int state = 1;
    if (digitalRead(RIGHT_BUTTON) == 0) {
        lcd.clear();
        state = right_button_pressed(state);
    }
    if (digitalRead(LEFT_BUTTON) == 0) {
        lcd.clear();
        state = left_button_pressed(state);
    }
    if (state < 1) {
        state_fails(state);
        lcd.print(state);
    }

    print_Clock(state);
    read_temp(state);
    read_humidity(state);
    int moist = read_moist();
    int light = read_light();
    int soil = read_soil();
    print_light_moist_LCD(state, light, moist);
    print_soil_LCD(state, soil);
    Serial.println(state);
    delay(200);
}
