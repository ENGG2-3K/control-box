#include "lib.h"

unsigned long last_button_press_time;

void setup()
{
    init_buttons();
    init_lcd();
}

void loop()
{
    update_lcd(pressed_button, rcvd_info);

    pressed_button = get_pressed_button();

    send_char(pressed_button.button_char);

    if (pressed_button.code == EMERGENCY_CODE)
    {
        enter_emergency_state();
    }
    else if (millis() - last_button_press_time > BUTTON_TIMEOUT)
    {
        last_button_press_time = millis();
    }

    rcvd_info = check_link_buffer();

    if (rcvd_info.rcvd_char == RCV_EMERGENCY_CHAR)
    {
        enter_emergency_state();
    }
}
