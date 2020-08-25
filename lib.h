#include "button.h"
#include "mega_info.h"

const int EMERGENCY_CODE = 404;
const int BUTTON_TIMEOUT = 1000; // 1 second

button pressed_button;
mega_info rcvd_info;

void init_buttons();
void init_lcd();
void update_lcd(button b, mega_info rcvd_info);
button get_pressed_button();
void enter_emergency_state();
void send_char(char c);
mega_info check_link_buffer();