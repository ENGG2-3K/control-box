#include "mega_info.h"
#include "lib.h"
#include "button.h"

unsigned long last_button_press_time;

// Global array of buttons
button buttons[4];

// Last pressed button - The one we are acting on now
button pressed_button;

// Represents the info we receive from the mega
mega_info rcvd_info;

bool new_info = false;
bool is_east = true;

// LiquidCrystal_PCF8574 lcd(0x27);

void setup()
{

    // To be able to print debug info
    

    // Initialises the buttons as input pullup and returns and fills the buttons array with button
    // structs
    init_buttons(buttons);

    // Initialise LCD in lcd.ino
    init_lcd();
}

void loop()
{

    // Update the LCD with the information we received from the mega and any info we want inputted
    // by the latest pressed button
    if (new_info == true)
    {
        // Update the lcd from lcd.ino
        update_lcd(pressed_button, rcvd_info, is_east);
    }

    // Check if the last pressed button was the emergency button or the mega has told us to go
    // into the emergency state.
    // No need to update the lcd here because we already do it at the beginning of loop()
    if (pressed_button.chars[0] == EMERGENCY_STOP_CHAR || rcvd_info.rcvd_char == RCV_EMERGENCY_CHAR)
    {
        // Endless while loop at the moment
        enter_emergency_state();
    }

    // If there has been a long enough timeout since the last button press, then update the last
    // button press time, get the newly pressed button and send the relevant char to the mega
    else if (abs(millis() - last_button_press_time > BUTTON_TIMEOUT))
    {
        // Updates the state of the button structs in buttons and returns the most recently pressed
        // button
        pressed_button = get_pressed_button(buttons);

        // Neat trick we can do with button.toggled since every button has two chars except the
        // emergency button
        char index_of_char = pressed_button.chars[pressed_button.toggled];

        // Send the correspending character to the mega
        send_char(pressed_button.chars[index_of_char]);

        // Reset last button press time
        // We do this after we send the data to the mega so their timing is as close to ours as
        // possible
        last_button_press_time = millis();
    }

    // Check if there is information from the mega that has been sent to us and store it into a
    // mega_info struct
    if (Serial.available())
    {
        new_info = true;
        rcvd_info = check_link_buffer();
    }
}
