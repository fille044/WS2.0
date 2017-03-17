#define RIGHT_BUTTON 12
#define LEFT_BUTTON 11

void setup() {
    // put your setup code here, to run once:
    pinMode(RIGHT_BUTTON, INPUT);
    pinMode(LEFT_BUTTON, INPUT);
    Serial.begin(9600);
}
int right_button_pressed(int state) {
    if (state == 1)
        return 2;
    else if (state == 2)
        return 3;
    else if (state == 3)
        return 1;
    else return (-1);
}
int left_button_pressed(int state) {
    if (state == 1)
        return 3;
    else if (state == 2)
        return 1;
    else if (state == 3)
        return 2;
    else
        return (-1);
}
void state_fails(int state) {
    Serial.println("Something went wrong, state is out of bounds: ");
    Serial.println(state);
}
void loop() {
    // put your main code here, to run repeatedly:
    static int state = -1;
    if (digitalRead(RIGHT_BUTTON) == 0) {
        state = right_button_pressed(state);
        if (state < 1) state_fails(state);
    }
    if (digitalRead(LEFT_BUTTON) == 0) {
        state = left_button_pressed(state);
        if (state < 1) state_fails(state);
    }
    Serial.println(state);
    delay(200);


}
