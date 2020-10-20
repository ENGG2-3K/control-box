#include <Arduino.h>
#include <SoftwareSerial.h>
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
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        pinMode(i + BUTTON_START_PIN, INPUT_PULLUP);
        buttons[i].pin_number = i + BUTTON_START_PIN;
        buttons[i].toggled = false;
    }

    // Emergency stop button
    buttons[0].chars[0] = EMERGENCY_STOP_CHAR;
    buttons[0].chars[1] = EMERGENCY_STOP_CHAR;

    // East / West button
    buttons[1].chars[0] = WEST_CHAR;
    buttons[1].chars[1] = EAST_CHAR;

    // Start / Stop button
    buttons[2].chars[0] = STOP_CHAR;
    buttons[2].chars[1] = START_CHAR;

    // Doors open / close button
    buttons[3].chars[0] = DOOR_CLOSE_CHAR;
    buttons[3].chars[1] = DOOR_OPEN_CHAR;
}

// Updates the global array of buttons and returns the updated pressed button
button get_pressed_button(button *buttons)
{
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        int button_pin = buttons[i].pin_number;

        if (digitalRead(button_pin) == LOW)
        {
            buttons[i].toggled = !buttons[i].toggled;
            return buttons[i];
        }
    }

    return button{-1, {-1, -1}, -1};
}

button debug_get_pressed_button(button buttons[], char debug_buffer[])
{
    char c = debug_buffer[0];
    
    if (c != BUTTON_DEBUG_CHAR)
    {
        return button{-1, {-1, -1}, -1};
    }

    Serial.println("lib.debug_get_pressed_button:: DEBUG - Checking for DEBUG_CHAR");
    Serial.print("lib.debug_get_pressed_button:: DEBUG - Got '");
    Serial.print(c);
    Serial.println("'");

    c = debug_buffer[1];
    Serial.print("lib.debug_get_pressed_button:: DEBUG - Received char '");
    Serial.print(c);
    Serial.println("'");

    // Check all the buttons to see if they are depressed
    int b_index;
    
    for (int i = 0; i < NUM_BUTTONS; i++)
    {
        if (c == buttons[i].chars[0] || c == buttons[i].chars[1])
        {
            Serial.print("lib.debug_get_pressed_button:: DEBUG - Pressed button pin number: ");
            Serial.println(buttons[i].pin_number);
            buttons[i].toggled = !buttons[i].toggled;

            b_index = i;
        }
    }

    debug_buffer[0] = '_';
    debug_buffer[1] = '_';
    
    return buttons[b_index];
}

void enter_emergency_state()
{
    while (1)
        ;
}

// Link send char code goes here
void send_char(char c, SoftwareSerial bt)
{
    bt.write(c);
    Serial.print("lib.send_char:: Sending char '");
    Serial.print(c);
    Serial.println("'");
}

bool debug_link_info_available(char* debug_buffer)
{
    if (debug_buffer[0] == RCV_DEBUG_CHAR)
    {
        return true;

    }
    else
    {
        return false;
    }
}

// Link receive info code goes here
mega_info debug_check_link_buffer(char debug_buffer[])
{
    if (debug_buffer[0] == RCV_DEBUG_CHAR)
    {
        Serial.println("lib.debug_check_link_buffer:: Reading debug link buffer char.");
        char c = debug_buffer[1];

        mega_info res = {c, 0};

        debug_buffer[0] = '_';
        debug_buffer[1] = '_';
        
        return res;
    }
    else
    {
        Serial.println("lib.check_link_buffer:: Didn't see debug char.");
        mega_info empty_res = {'_', 0};

        return empty_res;
    }
}

mega_info check_link_buffer(SoftwareSerial &bt)
{
    Serial.println("lib.check_link_buffer:: Reading debug link buffer char.");
    char c = bt.read();

    Serial.print("lib.check_link_buffer:: Read link buffer char: ");
    Serial.println(c);

    mega_info res = {c, 0};

    while (bt.available() > 0)
    {
        bt.read();
    }

    return res;
}

void debug_print_rcvd_info(mega_info rcvd_info)
{
    Serial.print("lib.debug_print_rcvd_info:: RCVD CHAR: ");
    Serial.println(rcvd_info.rcvd_char);
    Serial.println();
    // Serial.println(rcvd_info.rcvd_char);
}