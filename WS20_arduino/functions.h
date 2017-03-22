int right_button_pressed(int state);
int left_button_pressed(int state);
int both_buttons_pressed(int state);
void state_fails(int state);
int read_light();
int read_moist();
int read_soil();
void print_to_serial(int state, int light, int moist, int soil, int temp, int humidity);
