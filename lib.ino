#include <LiquidCrystal.h>
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

void update_lcd(LiquidCrystal *lcd, button b, mega_info rcvd_info)
{
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd->setCursor(0, 1);
    // print the number of seconds since reset:
    lcd->print(millis() / 1000);

    switch (rcvd_info.rcvd_char)
    {
    case RCV_ACCELERATE_CHAR:
        Serial.println("rcvd: 'a' - Accelerating");
        break;
    case RCV_DECELERATE_CHAR:
        Serial.println("rcvd: 'd' - Decelerating");
        break;
    case RCV_CONSTANT_SPEED_CHAR:
        Serial.println("rcvd: 'n' - Constant speed");
        break;
    case RCV_STOPPED_CHAR:
        Serial.println("rcvd: 's' - Stopped");
        break;
    case RCV_EMERGENCY_CHAR:
        Serial.println("rcvd: 'x' - Emergency stop");
        break;
    }
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
mega_info check_link_buffer()
{
    char c;
    if (Serial.available())
    {
        c = Serial.read();
    }
    else
    {
        c = '_';
    }
    mega_info res = {.rcvd_char = c,
                     .weight = 0};

    return res;
}