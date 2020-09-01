#include "button.h"
#include "mega_info.h"

const char BUTTON_START_PIN = 2;
const char BUTTON_END_PIN = 5;
const int EMERGENCY_CODE = 404;
const int BUTTON_TIMEOUT = 1000; // 1 second

button buttons[4];
button pressed_button;
mega_info rcvd_info;

int init_buttons(button *buttons);
int init_lcd();
int update_lcd(button b, mega_info rcvd_info);
int enter_emergency_state();
int send_char(char c);
button get_pressed_button(button *buttons);
mega_info check_link_buffer();
