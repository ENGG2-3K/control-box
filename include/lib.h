#ifndef LIB_H
#define LIB_H
#include <SoftwareSerial.h>
#include "button.h"
#include "mega_info.h"

const bool DEBUG = true;
const char RCV_DEBUG_CHAR = 'm';
const char BUTTON_DEBUG_CHAR = 'b';

const int NUM_BUTTONS = 4;
const char BUTTON_START_PIN = 2;
const int BUTTON_TIMEOUT = 500; // 0.5 second
const char EMERGENCY_STOP_CHAR = 'x';
const char EAST_CHAR = 'e';
const char WEST_CHAR = 'w';
const char START_CHAR = 'p';
const char STOP_CHAR = 'q';
const char DOOR_OPEN_CHAR = 'o';
const char DOOR_CLOSE_CHAR = 'c';

const char RCV_EMERGENCY_CHAR = 'x';
const char RCV_DECELERATE_CHAR = 'd';
const char RCV_CONSTANT_SPEED_CHAR = 'n';
const char RCV_STOPPED_CHAR = 's';
const char RCV_ACCELERATE_CHAR = 'a';

void init_buttons(button *buttons);

void init_lcd();

void update_lcd(button b, mega_info rcvd_info);
void lcd_scroll();

void enter_emergency_state();

void send_char(char c, SoftwareSerial bt);

button get_pressed_button(button *buttons);
button debug_get_pressed_button(button *buttons, char *debug_buffer);

bool debug_link_info_available(char* debug_buffer);

mega_info debug_check_link_buffer(char debug_buffer[]);
mega_info check_link_buffer(SoftwareSerial &bt);

void debug_print_rcvd_info(mega_info rcvd_info);
#endif