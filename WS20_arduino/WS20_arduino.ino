/*
 * This sketch with the including librarys together creates a complete
 * weatherstation.
 *
 * All of the defined values is stored in functions.h
 *
 * Featuring :
 * Indoor and outdoor measusement of both temperature and relative air humidity (RH)
 * Realtime clock for both time and date
 * Soil humidity with easy to use sensor
 * Easy to use navigation where you can choose what plant you have
 * Bright LCD-screen to show you just what you need.
 * Implemented Serial Monitor -function for easy troubleshooting.
 *
 * At the end of this page, there is two commented functions that when uncommented,
 * one will turn on screen sensitivity, and the other will print all values collected to Serial Monitor.
*/

#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Arduino.h>
#include <Wire.h>
#include "functions.h"
#include <Adafruit_DotStar.h>
#include <SPI.h>

Adafruit_DotStar LED = Adafruit_DotStar(LED_NUMPIXELS,
        LED_DATAPIN, LED_CLOCKPIN, DOTSTAR_BRG);
DHT dht_indoors(DHTPIN_INDOORS, DHTTYPE);
DHT dht_outdoors(DHTPIN_OUTDOORS, DHTTYPE);
DS3231  rtc(SDA, SCL);
// ADRESS OF LCD, DS, RW, E, D0, D1, D2, D3, D4
// This works for the LCD that was used in this project
LiquidCrystal_I2C lcd(LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7);

void setup()
{
    Serial.begin(9600);
    dht_indoors.begin();
    dht_outdoors.begin();
    rtc.begin();
    Wire.begin();
    lcd.begin (16,2); //  My LCD is 16x2 characters
    lcd.setBacklightPin(3, POSITIVE);
    LED.begin(); // Initialize pins for output
    LED.show();  // Turn all LEDs off ASAP

    /*
    * Uncomment to set date and time
    * 14 seconds to upload
    */
    //rtc.setDOW(FRIDAY);         // Set Day-of-Week to SUNDAY
    //rtc.setTime(11, 49, 30);     // Set the time to 12:00:00 (24hr format)
    //rtc.setDate(6, 4, 2017);    // Set the date to 16-03-2017
    Serial.println("60 second calibration started");
    delay(60000);
    Serial.println("Calibration finished");
}

/*
 * int state is for the navigation that has been implemented into the project
 * If the user is on page 2, state will be set to 2, and will show the content on that page.
 *
 * ex. if state = 1, print_clock will run and output time and date to LCD
 */

void print_Clock(int state)
{   // Print date and time to large LCD screen
    if (state == 1) {
        lcd.setCursor (0,0);
        lcd.print(rtc.getDateStr(FORMAT_LONG));
        lcd.print(" ");
        lcd.print(rtc.getDOWStr(FORMAT_SHORT));
        lcd.setCursor (0,1);
        lcd.print(rtc.getTimeStr());
        lcd.print(" ");
    }
}

int read_temp(int state)
{   // Print temp to large LCD screen
    if (state == 2) {
        int temp_in = dht_indoors.readTemperature();
        int temp_out = dht_outdoors.readTemperature();
        lcd.setCursor (0,0);
        lcd.print(temp_in);
        lcd.print("%C, ");
        lcd.setCursor (0,1);
        lcd.print(temp_out);
        lcd.print("%C, ");
        return temp_in;
    }
}

int read_humidity(int state)
{   // Print humidity to large LCD screen
    if (state == 2) {
        int humidity_in = dht_indoors.readHumidity();
        int humidity_out = dht_outdoors.readHumidity();
        lcd.setCursor (6,0);
        lcd.print(humidity_in);
        lcd.print("% RH in");

        lcd.setCursor (6,1);
        lcd.print(humidity_out);
        lcd.print("% RH out");
        return humidity_in;
    }
}

void light_LED(int colour)
{
    // Lights up the LED-strip in the correct colour.
    // Call 1 for RED, 2 for yellow and 3 for green.
    for (int x = 0; x < 6; x++ ){
        LED.setBrightness(100);
        if (colour == 1){
            LED.setPixelColor(x, RED); // 'On' pixel at head
            LED.setBrightness(255);
        }
        else if (colour == 2)
            LED.setPixelColor(x, YELLOW); // 'On' pixel at head
        else if (colour == 3)
            LED.setPixelColor(x, GREEN); // 'On' pixel at head
        LED.show(); // Refresh strip
    }
}

// Lag-free display that prints level and value without any lcd.clear() functions
void print_soil_LCD(int state, int current_soil, int critical_soil, int warning_soil)
{
    // Light up RED LED if very dry
    if (current_soil >= critical_soil) {
        light_LED(1);
        if (state == 3) {
            lcd.setCursor (0,0);
            lcd.print("Soil: CRITICAL");
            lcd.setCursor (0,1);
            lcd.print(current_soil);
        }
    }
    //Light up YELLOW LED if  dry
    else if (current_soil >= warning_soil && current_soil < critical_soil) {
        light_LED(2);
        if (state == 3) {
            lcd.setCursor (0,0);
            lcd.print("Soil: WARNING ");
            lcd.setCursor (0,1);
            //Prints current soil value
            lcd.print(current_soil);
            if(current_soil < 1000) {
                lcd.setCursor (3,1);
                lcd.print(" ");
            }
        }
    }
    // If moister than warning level, it says ok, and no LED
    else if (current_soil < warning_soil) {
        light_LED(3);
        if (state == 3) {
            lcd.setCursor (0,0);
            lcd.print("Soil: OK      ");
            lcd.setCursor (0,1);
            //Prints current soil value
            lcd.print(current_soil);
            if(current_soil < 1000) {
                lcd.setCursor (3,1);
                lcd.print(" ");
            }
        }
    }
}

// Navigation system that let´s the user choose which plant to adjust the critical level to.
// Set plant and levels in functions.h
int set_critical_soil(int state)
{
    if (state == 71) {
        lcd.setCursor(0,0);
        lcd.print(PLANT_ONE);
        return PLANT_ONE_CRITICAL;
    }
    else if (state == 72) {
        lcd.setCursor(0,0);
        lcd.print(PLANT_TWO);
        return PLANT_TWO_CRITICAL;
     }
     else if (state == 73) {
         lcd.setCursor(0,0);
         lcd.print(PLANT_THREE);
         return PLANT_THREE_CRITICAL;
      }
      else if (state == 74) {
          lcd.setCursor(0,0);
          lcd.print(PLANT_FOUR);
          return PLANT_FOUR_CRITICAL;
       }
}

// Navigation system that let´s the user choose which plant to adjust the critical level to.
int set_warning_soil(int state)
{
    if (state == 71) {
        return PLANT_ONE_WARNING;
    }
     else if (state == 72) {
         return PLANT_TWO_WARNING;
     }
     else if (state == 73) {
         return PLANT_THREE_WARNING;
     }
     else if (state == 74) {
         return PLANT_FOUR_WARNING;
     }
}

// Print the UI for the settings menu, accessed by pressing both buttons
void print_setting_LCD(int state)
{
    if (state == 5) {
        lcd.setCursor(0,0);
        lcd.print("Settings");
        lcd.setCursor(0,1);
        lcd.print("Double for back");
    }
    if (state == 6) {
        lcd.setCursor(0,0);
        lcd.print("Set clock, date");
        lcd.setCursor(0,1);
        lcd.print("Not until v2.0");
    }
    if (state == 7) {
        lcd.setCursor(0,0);
        lcd.print("Choose plant");
    }
}

int main_function(int state, int critical_soil, int warning_soil)
{
    if (digitalRead(RIGHT_BUTTON) == 0) {
        lcd.clear();
        state = right_button_pressed(state);
    }
    if (digitalRead(LEFT_BUTTON) == 0) {
        lcd.clear();
        state = left_button_pressed(state);
    }
    if (digitalRead(LEFT_BUTTON)==0 &&(digitalRead(RIGHT_BUTTON)==0)) {
        state = both_buttons_pressed(state);
    }
    // If something goes wrong with the statements above, they will return -1
    // Giving the user a hint that the system needs a reboot.
    if (state < 1) {
        state_fails(state);
        lcd.setCursor(0,0);
        lcd.print("Contact someone smart");
    }


    print_Clock(state);
    int temp = read_temp(state);
    int humidity = read_humidity(state);
    int soil = read_soil();
    Serial.println(soil);
    print_soil_LCD(state, soil, critical_soil, warning_soil);
    print_setting_LCD(state);

    //  Uncomment to print all valuable numbers to Serial monitor
    //print_to_serial(state, soil, temp, humidity, critical_soil, warning_soil);

    delay(200);
    return state;
}


void loop()
{
    int value = digitalRead(IR_SENSOR);
    delay(100);
    static int critical_soil = 1000;
    static int warning_soil = 300;
    // If the IR_sensor senses movement, it will run the main_function
    // which contains everything about the LCD.
    if (value == 1){
        lcd.setBacklight(HIGH);
        // 100 laps equals 23 seconds of watching the screen
        for (int timer = 0; timer < 200; timer++){
            static int state = 1;
            state = main_function(state, critical_soil, warning_soil);
            if (state > 70) {
                critical_soil = set_critical_soil(state);
                warning_soil = set_warning_soil(state);
            }
        }
    }
    // If no movement, only the soil sensor will update and show via the LED-strip.
    else{
        int soil = read_soil();
        print_soil_LCD(1, soil, critical_soil, warning_soil);
        lcd.clear();
        lcd.setBacklight(LOW);
        delay(2000);
    }
    
}
