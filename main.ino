#include "mega_info.h"
#include "lib.h"

unsigned long last_button_press_time;

// Global array of buttons
button pressed_button;
mega_info rcvd_info;

void setup()
{
    init_buttons(buttons);

    init_lcd();
}

void loop()
{

    // Update the LCD with the information we received from the mega and any info we want inputted by the latest pressed button
    update_lcd(pressed_button, rcvd_info);

    // Check if the last pressed button was the emergency button or the mega has told us to go into the emergency state
    if (pressed_button.chars[0] == 'e' || rcvd_info.rcvd_char == RCV_EMERGENCY_CHAR)
    {
        enter_emergency_state();
    }
    // If there has been a long enough timeout since the last button press, then update the last button press time, get the newly pressed button and send the relevant char to the mega
    else if (millis() - last_button_press_time > BUTTON_TIMEOUT)
    {
        pressed_button = get_pressed_button(buttons);
        char index_of_char = pressed_button.chars[pressed_button.toggled];
        send_char(pressed_button.chars[index_of_char]);
        last_button_press_time = millis();
    }

    // Check if there is information from the mega that has been sent to us and store it into a mega_info struct
    rcvd_info = check_link_buffer();
}
