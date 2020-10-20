#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "lib.h"

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

bool is_east = true;
bool started = false;
bool door_open = true;

char *prev_message;

unsigned long loop_count = 0;

void debug_print_button(button b);
void handle_direction_button(bool toggled);
void handle_start_stop_button(bool toggled);
void handle_door_button(bool toggled);

void init_lcd()
{
    int error;
    Serial.println("Check for LCD");

    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    if (error != 0) {
        Serial.print("Error: ");
        Serial.print(error);
    }
    else
    {
        Serial.println("No LCD Error");
    }

    lcd.begin(16, 2); // initialize the lcd

    lcd.setBacklight(256);
    lcd.home();
    lcd.clear();
    prev_message = "Waiting for Mega...";
    lcd.print("Waiting for Mega...");
    lcd.setCursor(0, 1);
    lcd.print("East");
}

void lcd_scroll() {
    if (loop_count / 1000 == 0) {
        lcd.scrollDisplayLeft();
    }
    
    loop_count++;
}

void update_lcd(button b, mega_info rcvd_info)
{
    if (b.chars[0] >= 0) {
        debug_print_button(b); 
    }
    char *to_print;

    // Clear the lcd screen
    lcd.setBacklight(256);
    lcd.home();
    lcd.clear();

    switch (rcvd_info.rcvd_char)
    {
    case RCV_ACCELERATE_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'a' - Accelerating");
        to_print = "Accelerating";
        break;
    case RCV_DECELERATE_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'd' - Decelerating");
        to_print = "Decelerating";
        break;
    case RCV_CONSTANT_SPEED_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'n' - Constant speed");
        to_print = "Constant Speed";
        break;
    case RCV_STOPPED_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 's' - Stopped");
        to_print = "Stopped";
        break;
    case RCV_EMERGENCY_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'x' - Emergency stop");
        to_print = "Emergency Stop";
        break;
    default:
        to_print = prev_message;
        break;
    }

    prev_message = to_print;
    lcd.print(to_print);

    switch (b.chars[0])
    {
    case WEST_CHAR:
        handle_direction_button(b.toggled);
        break;
    case STOP_CHAR:
        handle_start_stop_button(b.toggled);
        break;
    case DOOR_CLOSE_CHAR:
        handle_door_button(b.toggled);
        break;
    default:
        Serial.println("lcd.update_lcd:: Pressed button chars don't match");
        break;
    }

    if (b.chars[0] == EMERGENCY_STOP_CHAR) {
        lcd.home();
        lcd.clear();
        lcd.print("EMERGENCY STOP");
    }
}

void handle_direction_button(bool toggled) {
    is_east = !toggled;

    Serial.println("Direction button");
    lcd.setCursor(0, 1);
    if (is_east == true) {
        lcd.print("East");
    } else {
        lcd.print("West");
    }
}

void handle_start_stop_button(bool toggled) {
    started = !toggled;

    lcd.setCursor(6, 1);
    if (started == true) {
        lcd.print("Stopping");
    } else {
        lcd.print("Starting");
    }
}

void handle_door_button(bool toggled) {
    door_open = !toggled;

    lcd.setCursor(6, 1);
    if (door_open == true) {
        lcd.print("Door Opening");
    } else {
        lcd.print("Door Closing");
    }
}

void debug_print_button(button b) {
    char b_info[30];

    Serial.print("lcd.debug_print_button:: Pressed button: ");
    sprintf(b_info, "chars = [%c, %c]", b.chars[0], b.chars[1]);

    Serial.println(b_info);

}