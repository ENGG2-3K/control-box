#include <LiquidCrystal.h>
#include "mega_info.h"
#include "lib.h"
#include "button.h"

unsigned long last_button_press_time;

// Global array of buttons
button pressed_button;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Represents the info we receive from the mega
mega_info rcvd_info;

void setup()
{
    init_buttons(buttons);

    /*
    The circuit:
        * LCD RS pin to digital pin 12
        * LCD Enable pin to digital pin 11
        * LCD D4 pin to digital pin 5
        * LCD D5 pin to digital pin 4
        * LCD D6 pin to digital pin 3
        * LCD D7 pin to digital pin 2
        * LCD R/W pin to ground
        * LCD VSS pin to ground
        * LCD VCC pin to 5V
        * 10K resistor:
        * ends to +5V and ground
        * wiper to LCD VO pin (pin 3)
    */
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("hello, world!");
}

void loop()
{

    // Update the LCD with the information we received from the mega and any info we want inputted
    // by the latest pressed button
    update_lcd(&lcd, pressed_button, rcvd_info);

    // Check if the last pressed button was the emergency button or the mega has told us to go
    // into the emergency state.
    // No need to update the lcd here because we already do it at the beginning of loop()
    if (pressed_button.chars[0] == 'e' || rcvd_info.rcvd_char == RCV_EMERGENCY_CHAR)
    {
        enter_emergency_state();
    }
    // If there has been a long enough timeout since the last button press, then update the last
    // button press time, get the newly pressed button and send the relevant char to the mega
    else if (abs(millis() - last_button_press_time > BUTTON_TIMEOUT))
    {
        pressed_button = get_pressed_button(buttons);
        char index_of_char = pressed_button.chars[pressed_button.toggled];
        send_char(pressed_button.chars[index_of_char]);
        last_button_press_time = millis();
    }

    // Check if there is information from the mega that has been sent to us and store it into a
    // mega_info struct
    rcvd_info = check_link_buffer();
}
