#include "button.h"
#include "mega_info.h"

const char BUTTON_START_PIN = 2;
const char BUTTON_END_PIN = 5;
const int EMERGENCY_CODE = 404;
const int BUTTON_TIMEOUT = 1000; // 1 second

button pressed_button;
mega_info rcvd_info;

void init_buttons();
void init_lcd(button *buttons);
void update_lcd(button b, mega_info rcvd_info);
button get_pressed_button(button *buttons);
void enter_emergency_state();
void send_char(char c);
mega_info check_link_buffer();

void init_buttons(button *buttons)
{
    // Sets the pin mode for each button and modifies an array of button structs representing the buttons
    for (size_t i = BUTTON_START_PIN; i < BUTTON_END_PIN; i++)
    {
        pinMode(i, INPUT_PULLUP);
    }

    buttons[0] = {.pin_number = BUTTON_START_PIN, .chars = {'x'}, .toggled = 0};
    buttons[1] = {.pin_number = BUTTON_START_PIN + 1, .chars = {'e', 'w'}, .toggled = 0};
    buttons[2] = {.pin_number = BUTTON_START_PIN + 2, .chars = {'p', 'q'}, .toggled = 0};
    buttons[3] = {.pin_number = BUTTON_START_PIN + 3, .chars = {'o', 'c'}, .toggled = 0};
}

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