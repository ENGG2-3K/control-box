#include <LiquidCrystal.h>
#include "button.h"
#include "lib.h"

void init_buttons(button *buttons)
{
    // Sets the pin mode for each button and modifies an array of button structs representing the buttons
    for (int i = BUTTON_START_PIN; i < BUTTON_END_PIN; i++)
    {
        pinMode(i, INPUT_PULLUP);
        buttons[i - BUTTON_START_PIN].pin_number = i;
        buttons[i - BUTTON_START_PIN].toggled = 0;
    }

    // Emergency stop button
    buttons[0].chars[0] = 'x';
    buttons[0].chars[1] = 'x';

    // East / West button
    buttons[1].chars[0] = 'e';
    buttons[1].chars[1] = 'w';

    // Start / Stop button
    buttons[2].chars[0] = 'p';
    buttons[2].chars[1] = 'q';

    // Doors open / close button
    buttons[3].chars[0] = 'o';
    buttons[3].chars[1] = 'c';
}

void update_lcd(LiquidCrystal *lcd, button b, mega_info rcvd_info)
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd->setCursor(0, 1);
    // print the number of seconds since reset:
    lcd->print(millis() / 1000);
}

// Updates the global array of buttons and returns the updated pressed button
button get_pressed_button(button *buttons)
{
    for (char i = 0; i < BUTTON_END_PIN - BUTTON_START_PIN; i++)
    {
        char button_pin = buttons[i].pin_number;

        if (digitalRead(button_pin) == LOW)
        {
            buttons[i].toggled = !buttons[i].toggled;
            return buttons[i];
        }
    }
}

// IDK what to do here
void enter_emergency_state()
{
    while (1)
    {
        /* code */
    }
}

// Link send char code goes here
void send_char(char c)
{
}

// Link receive info code goes here
mega_info check_link_buffer() {}