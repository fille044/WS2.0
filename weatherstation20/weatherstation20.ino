/*Begining of Auto generated code by Atmel studio */
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  dht.begin();
}

void loop() {
  int temp = dht.readTemperature();
  Serial.println(temp);
  delay(2000);
}
