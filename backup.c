#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Arduino.h>
#include <Wire.h>

#define DHTPIN_INDOORS 8
#define DHTTYPE DHT11
#define PHOTOTRANSISTOR A0
#define MOIST_SENSOR A1
#define POSITIVE 0
#define LCD_ADDRESS 0x3F
#define RIGHT_BUTTON 12
#define LEFT_BUTTON 11

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
    lcd.setBacklight(HIGH);
    lcd.home (); // go home
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

int read_moist(int state) {
    int moist = analogRead(MOIST_SENSOR);
    Serial.println(moist);
    // Print moist-value to large LCD screen
    if (state == 3) {
        lcd.setCursor (0,0);
        lcd.print(moist);
        lcd.print(" moistness");
    }
    return moist;
}

int read_light(int state) {
    int light = analogRead(PHOTOTRANSISTOR);
    Serial.println(light);
    // Print light-value to large LCD screen
    if (state == 3) {
        lcd.setCursor (0,1);
        lcd.print(light);
        lcd.print(" lumens");
    }
    return light;
}

int right_button_pressed(int state) {
    if (state == 1) return 2;
    else if (state == 2) return 3;
    else if (state == 3) return 1;
    else return (-1);
}

int left_button_pressed(int state) {
    if (state == 1) return 3;
    else if (state == 2) return 1;
    else if (state == 3) return 2;
    else return (-1);
}
void state_fails(int state) {
    Serial.println("Something went wrong, state is out of bounds: ");
    Serial.println(state);
}

void loop() {
    static int state = 1;
    if (digitalRead(RIGHT_BUTTON) == 0) {
        state = right_button_pressed(state);
        if (state < 1) state_fails(state);
    }
    if (digitalRead(LEFT_BUTTON) == 0) {
        state = left_button_pressed(state);
        if (state < 1) state_fails(state);
    }
    print_Clock(state);
    read_temp(state);
    read_humidity(state);
    read_moist(state);
    read_light(state);


    Serial.println(state);
    delay(1000);
}
