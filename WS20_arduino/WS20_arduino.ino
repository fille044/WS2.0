#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <DHT.h>
#include <Arduino.h>
#include <Wire.h>

#define DHTPIN 8
#define DHTTYPE DHT11
#define PHOTOTRANSISTOR A0
#define MOIST_SENSOR A1
#define POSITIVE 1

DHT dht(DHTPIN, DHTTYPE);
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup() {
    Serial.begin(9600);
    dht.begin();
    rtc.begin();
    Wire.begin();
    lcd.begin(16,2);
    lcd.backlight();

    lcd.setCursor(0,1); //Start at character 4 on line 0
    lcd.print("Hello, world!");

    /*
     * Uncomment to set date and time
     * rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
     * rtc.setTime(21, 35, 0);     // Set the time to 12:00:00 (24hr format)
     * rtc.setDate(16, 3, 2017);   // Set the date to 16-03-2017
     */
}

/*
 * int state is a preparation for the navigation that may be implemented into the project
 * If the user is in page 2, state will be set to 2, and may or may not show the content.
 */

int print_Clock(int state){
    // Send day of week
    Serial.print(rtc.getDOWStr());
    Serial.print(" ");
    // Send date
    Serial.print(rtc.getDateStr());
    Serial.print(" -- ");
    // Send time
    Serial.println(rtc.getTimeStr());
}
int read_temp(int state){
    int temp = dht.readTemperature();
    Serial.println(temp);
    return temp;
}
int read_humidity(int state){
    int humidity = dht.readHumidity();
    Serial.println(humidity);
    return humidity;
}
int read_moist(int state){
    int moist = analogRead(MOIST_SENSOR);
    Serial.println(moist);
    return moist;
}
int read_light(int state){
    int light = analogRead(PHOTOTRANSISTOR);
    Serial.println(light);
    return light;
}

void loop() {
    print_Clock(1);
    read_temp(1);
    read_humidity(1);
    read_moist(1);
    read_light(1);

    delay(5000);
}

