#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Arduino.h>
#include <Wire.h>
#include "functions.h"

#define DHTPIN_INDOORS 8
#define DHTPIN_OUTDOORS 5
#define DHTTYPE DHT11
#define POSITIVE 0
#define LCD_ADDRESS 0x3F
#define RIGHT_BUTTON 12
#define LEFT_BUTTON 11
#define YELLOW_LED 2
#define RED_LED 3
#define ON HIGH
#define OFF LOW
#define CACTUS_WARNING 700
#define CACTUS_CRITICAL 850
#define JULSTJARNA_WARNING 500
#define JULSTJARNA_CRITICAL 700
#define HORTENSIA_WARNING 200
#define HORTENSIA_CRITICAL 800
#define KRYSANTEMUM_WARNING 900
#define KRYSANTEMUM_CRITICAL 1080

DHT dht_indoors(DHTPIN_INDOORS, DHTTYPE);
DHT dht_outdoors(DHTPIN_OUTDOORS, DHTTYPE);
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7);    // Set the LCD I2C address

void setup() {
    Serial.begin(9600);
    dht_indoors.begin();
    dht_outdoors.begin();
    rtc.begin();
    Wire.begin();
    lcd.begin (16,2); //  My LCD is 16x2 characters
    // Turn on the backlight
    lcd.setBacklightPin(3, POSITIVE);

    /*
    * Uncomment to set date and time
    rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY*/
    //rtc.setTime(9, 24, 00);     // Set the time to 12:00:00 (24hr format)
    /*rtc.setDate(17, 3, 2017);   // Set the date to 16-03-2017
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
    // Print temp to large LCD screen
    if (state == 2) {
        int temp_in = dht_indoors.readTemperature();
        int temp_out = dht_outdoors.readTemperature();
        lcd.setCursor (0,0);
        lcd.print(temp_in);
        lcd.print("%C, ");
        lcd.setCursor (0,1);
        //lcd.print("34");
        lcd.print(temp_out);
        lcd.print("%C, ");
        return temp_in;
    }
}

int read_humidity(int state) {
    // Print humidity to large LCD screen
    if (state == 2) {
        int humidity_in = dht_indoors.readHumidity();
        int humidity_out = dht_outdoors.readHumidity();
        lcd.setCursor (6,0);
        lcd.print(humidity_in);
        lcd.print("% RH in");

        lcd.setCursor (6,1);
        lcd.print(humidity_out);
        //lcd.print(humidity_out);
        lcd.print("% RH out");
        return humidity_out;
    }
}

void high_low_light(int light){
    if (light > 100)
        lcd.setBacklight(HIGH);
    else if (light <= 100)
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

void print_soil_LCD(int state, int current_soil, int critical_soil, int warning_soil){
    if (current_soil>critical_soil) {
        digitalWrite(RED_LED, ON);
        if (state == 4) {
            lcd.clear();
            lcd.setCursor (0,0);
            lcd.print("Soil: CRITICAL");
            lcd.setCursor (0,1);
            lcd.print(current_soil);

        }
    }
    else if (current_soil>warning_soil && current_soil <= critical_soil) {
        digitalWrite(YELLOW_LED, ON);
        if (state == 4) {
            lcd.clear();
            lcd.setCursor (0,0);
            lcd.print("Soil: WARNING");
            lcd.setCursor (0,1);
            lcd.print(current_soil);

        }
    }
    else if (current_soil <= warning_soil) {
        if (state == 4) {
            lcd.clear();
            lcd.setCursor (0,0);
            lcd.print("Soil: OK");
            lcd.setCursor (0,1);
            lcd.print(current_soil);
            digitalWrite(YELLOW_LED, OFF);
            digitalWrite(RED_LED, OFF);
        }

    }
}

int set_critical_soil(int state){
    if (state == 71){
        lcd.setCursor(0,0);
        lcd.print("Cactus");
        return CACTUS_CRITICAL;
    }
    else if (state == 72){
        lcd.setCursor(0,0);
        lcd.print("Julstjarna");
        return JULSTJARNA_CRITICAL;
     }
     else if (state == 73){
         lcd.setCursor(0,0);
         lcd.print("Hortensia");
         return HORTENSIA_CRITICAL;
      }
      else if (state == 74){
          lcd.setCursor(0,0);
          lcd.print("Krysantemum");
          return KRYSANTEMUM_CRITICAL;
       }
        /*CACTUS_WARNING 700
        CACTUS_CRITICAL 850
        FLOWER_WARNING 500
        FLOWER_CRITICAL 700*/
}

int set_warning_soil(int state){
    if (state == 71){
        return CACTUS_WARNING;
    }
     else if (state == 72){
         return JULSTJARNA_WARNING;
     }
     else if (state == 73){
         return HORTENSIA_WARNING;
     }
     else if (state == 74){
         return KRYSANTEMUM_WARNING;
     }

        /*CACTUS_WARNING 700
        CACTUS_CRITICAL 850
        FLOWER_WARNING 500
        FLOWER_CRITICAL 700*/
}

void print_setting_LCD(int state){
    if (state == 5){
        lcd.setCursor(0,0);
        lcd.print("Settings");
        lcd.setCursor(0,1);
        lcd.print("Double for back");
    }
    if (state == 6){
        lcd.setCursor(0,0);
        lcd.print("Set clock and date");
    }
    if (state == 7){
        lcd.setCursor(0,0);
        lcd.print("Choose plant");
    }
}

void loop() {
    static int state = 1;
    static int critical_soil = 1000;
    static int warning_soil = 300;
    if (digitalRead(RIGHT_BUTTON) == 0) {
        lcd.clear();
        state = right_button_pressed(state);
    }
    if (digitalRead(LEFT_BUTTON) == 0) {
        lcd.clear();
        state = left_button_pressed(state);
    }
    if (digitalRead(LEFT_BUTTON)==0 &&(digitalRead(RIGHT_BUTTON)==0)){
        state = both_buttons_pressed(state);
    }
    if (state < 1) {
        state_fails(state);
        lcd.print(state);
    }

    //Serial.println(critical_soil);
    //Serial.println(warning_soil);
    //Serial.println(state);
    if (state > 70){
        critical_soil = set_critical_soil(state);
        warning_soil = set_warning_soil(state);
    }
    print_Clock(state);
    int temp = read_temp(state);
    int humidity = read_humidity(state);
    int moist = read_moist();
    int light = read_light();
    int soil = read_soil();
    print_light_moist_LCD(state, light, moist);
    print_soil_LCD(state, soil, critical_soil, warning_soil);
    print_setting_LCD(state);
    print_to_serial(state, light, moist, soil, temp, humidity);
    delay(200);
}
