#include <LiquidCrystal_PCF8574.h>
#include "button.h"
#include "lib.h"

/*
    Button Mapping
    The char array in each Button contains 2 characters, representing the possible characters that should be sent corresponding to each function
    [0] - Emergency Stop
        [0][0] - E-stop - 'x'
        [0][1] - E-stop - 'x'
    [1] - East/West toggle
        [1][0] - East - 'e'
        [1][1] - West - 'w'
    [2] - Start/Stop toggle
        [2][0] - Start - 'p'
        [2][1] - Stop - 'q'
    [3] - Door Toggle
        [3][0] - Open - 'o' 
        [3][1] - Close - 'c'
*/

bool is_east = true;

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
    buttons[0].chars[0] = EMERGENCY_STOP_CHAR;
    buttons[0].chars[1] = EMERGENCY_STOP_CHAR;

    // East / West button
    buttons[1].chars[0] = EAST_CHAR;
    buttons[1].chars[1] = WEST_CHAR;

    // Start / Stop button
    buttons[2].chars[0] = START_CHAR;
    buttons[2].chars[1] = STOP_CHAR;

    // Doors open / close button
    buttons[3].chars[0] = DOOR_OPEN_CHAR;
    buttons[3].chars[1] = DOOR_CLOSE_CHAR;
}

void update_lcd(LiquidCrystal_PCF8574 *lcd, button b, mega_info rcvd_info)
{
    // Clear the lcd screen
    lcd->clear();

    switch (rcvd_info.rcvd_char)
    {
    case RCV_ACCELERATE_CHAR:
        Serial.println("rcvd: 'a' - Accelerating");
        lcd->print("Accelerating");
        break;
    case RCV_DECELERATE_CHAR:
        Serial.println("rcvd: 'd' - Decelerating");
        lcd->print("Decelerating");
        break;
    case RCV_CONSTANT_SPEED_CHAR:
        Serial.println("rcvd: 'n' - Constant speed");
        lcd->print("Constant speed");
        break;
    case RCV_STOPPED_CHAR:
        Serial.println("rcvd: 's' - Stopped");
        lcd->print("Stopped");
        break;
    case RCV_EMERGENCY_CHAR:
        Serial.println("rcvd: 'x' - Emergency stop");
        lcd->print("Emergency stop");
        break;
    }

    lcd->setCursor(0, 1);
    lcd->print(direction);
}

// Updates the global array of buttons and returns the updated pressed button

button get_pressed_button(button *buttons)
{
    if (Serial.available())
    {
        for (char i = 0; i < BUTTON_END_PIN - BUTTON_START_PIN; i++)
        {
            char button_pin = buttons[i].pin_number;

            if (Serial.read() == buttons[i].chars[0] || Serial.read() == buttons[i].chars[1])
            {
                buttons[i].toggled = !buttons[i].toggled;
                return buttons[i];
            }
        }
    }
    // for (char i = 0; i < BUTTON_END_PIN - BUTTON_START_PIN; i++)
    // {
    //     char button_pin = buttons[i].pin_number;

    //     if (digitalRead(button_pin) == LOW)
    //     {
    //         buttons[i].toggled = !buttons[i].toggled;
    //         return buttons[i];
    //     }
    // }
}

void enter_emergency_state()
{
    while (1)
    {
        Serial.println("Emergency stopped")
    }
}

// Link send char code goes here
void send_char(char c)
{
}

// Link receive info code goes here
mega_info check_link_buffer(char *direction)
{
    char c;

    if (Serial.available())
    {

        c = Serial.read();
    }

    if (c == 'm')
    {
        c = Serial.read();

        if (c == 'd')
        {
            is_east = !is_east;
            *direction = is_east ? "East" : "West";
        }

        mega_info res = {.rcvd_char = c,
                         .weight = 0};

        return res;
    }
    else
    {
        return rcvd_info;
    }
}