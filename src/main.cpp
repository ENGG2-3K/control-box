#include <Arduino.h>
#include "mega_info.h"
#include "lib.h"
#include "button.h"
#include <SoftwareSerial.h>

#define BT_RX 8
#define BT_TX 9

int new_link_info_available();
mega_info check_link_buffer();

SoftwareSerial BTSerial(BT_RX, BT_TX); // (RX, TX)

unsigned long last_button_press_time;
char last_sent_char = '_';

// Global array of buttons
button buttons[NUM_BUTTONS];

// Last pressed button - The one we are acting on now
button pressed_button;

// Represents the info we receive from the mega
mega_info rcvd_info;

bool new_info = false;

/* ------DEBUG------- */
char debug_buffer[2];
int LOOP_NUM = 0;

void init_debug_buf();
void print_debug_buffer();
void get_debug_chars();
/* ---------------- */

void setup()
{
    Serial.begin(9600);

    // wait on Serial to be available on Leonardo
    while (!Serial)
    {
        ;
    }

    // Initialises the buttons as input pullup and returns and fills the buttons array with button
    // structs
    init_buttons(buttons);

    // Initialise LCD in lcd.ino
    init_lcd();

    init_debug_buf();

    BTSerial.begin(9600);
}

void loop()
{
    get_debug_chars();
    // Update the LCD with the information we received from the mega and any info we want inputted
    // by the latest pressed button
    if (new_info == true)
    {
        Serial.print("main.loop:: LOOP_NUM = ");
        Serial.println(LOOP_NUM);

        // Update the lcd from lcd.ino
        update_lcd(pressed_button, rcvd_info);
        new_info = false;
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
        pressed_button = debug_get_pressed_button(buttons, debug_buffer);
        // pressed_button = get_pressed_button(buttons);

        // Neat trick we can do with button.toggled since every button has two chars except the
        // emergency button
        char char_to_send = pressed_button.chars[pressed_button.toggled];
        if (char_to_send != '_' && char_to_send != NULL) {
            // Send the correspending character to the mega
            // Serial.print("main:: char_to_send = ")
            if (char_to_send != last_sent_char)
            {
                send_char(char_to_send, BTSerial);
                last_sent_char = char_to_send;
                new_info = true;
            }
        }

        // Reset last button press time
        // We do this after we send the data to the mega so their timing is as close to ours as
        // possible
        last_button_press_time = millis();
    }

    // Check if there is information from the mega that has been sent to us and store it into a
    // mega_info struct
    if (debug_link_info_available(debug_buffer) == true)
    {
        Serial.println("main.loop:: Debug info is available");

        new_info = true;
        rcvd_info = debug_check_link_buffer(debug_buffer);
        debug_print_rcvd_info(rcvd_info);
        Serial.println();
    }

    LOOP_NUM++;
}

void print_debug_buffer()
{
    char out[10];

    sprintf(out, "[%c, %c]", debug_buffer[0], debug_buffer[1]);

    Serial.println(out);
}

void get_debug_chars()
{
    if (Serial.available() >= 2) {
        char c = Serial.read();
        if (c == BUTTON_DEBUG_CHAR || c == RCV_DEBUG_CHAR) {
            debug_buffer[0] = c;
            c = Serial.read();

            Serial.print("Second char ");
            Serial.println(c, DEC);
            debug_buffer[1] = c;

            print_debug_buffer();
        }
    }
}
   
void init_debug_buf() {
    debug_buffer[0] = '_';
    debug_buffer[1] = '_';
}

mega_info check_link_buffer()
{
    Serial.println("lib.check_link_buffer:: Reading debug link buffer char.");
    char c = BTSerial.read();

    Serial.print("lib.check_link_buffer:: Read link buffer char: ");
    Serial.println(c);

    mega_info res = {c, 0};

    while (BTSerial.available() > 0)
    {
        BTSerial.read();
    }

    return res;
}