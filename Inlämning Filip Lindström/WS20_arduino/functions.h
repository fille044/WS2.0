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
#define LED_NUMPIXELS 4 // Number of LEDs in strip
#define LED_DATAPIN    7
#define LED_CLOCKPIN   6
#define IR_SENSOR 9
#define SOIL_SENSOR A2

// Colors in hex
#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define RED 0x00FF00
#define BLUE 0x0000FF
#define GREEN 0xFF0000
#define YELLOW 0xFFFF00
#define PINK 0x00FFFF

// Plant names and levels
#define PLANT_ONE "Kaktus"
#define PLANT_ONE_WARNING 700
#define PLANT_ONE_CRITICAL 850
#define PLANT_TWO "Basilika"
#define PLANT_TWO_WARNING 500
#define PLANT_TWO_CRITICAL 700
#define PLANT_THREE "Timjan"
#define PLANT_THREE_WARNING 200
#define PLANT_THREE_CRITICAL 800
#define PLANT_FOUR "Valfri"
#define PLANT_FOUR_WARNING 900
#define PLANT_FOUR_CRITICAL 1080

int right_button_pressed(int state);
int left_button_pressed(int state);
int both_buttons_pressed(int state);
void state_fails(int state);
int read_soil();
void print_to_serial(int state, int soil, int temp, 
        int humidity, int critical_soil, int warning_soil);

