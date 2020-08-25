#include "lib.h"

void setup()
{
    init_buttons();
    init_lcd();
}

void loop()
{
    update_lcd(pressed_button, rcvd_info);

    pressed_button = get_pressed_button();

    if (pressed_button.code == EMERGENCY_CODE)
    {
        enter_emergency_state();
    }
    else
    {
        send_char(pressed_button.button_char);
    }

    rcvd_info = check_link_buffer();
}
