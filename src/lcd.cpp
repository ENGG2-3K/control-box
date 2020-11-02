#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "lib.h"

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

bool is_east = true;
bool started = false;
bool door_open = true;

String prev_message;
String prev_button_lcd_str;

unsigned long loop_count = 0;

void debug_print_button(button b);
void handle_direction_button(bool toggled);
void handle_start_stop_button(bool toggled);
void handle_door_button(bool toggled);
void handle_direction_rcv(char dir);
void clear_top_row();
void clear_bottom_row();

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
    lcd.print(prev_message);
    // prev_button_lcd_str = "East";
    // lcd.setCursor(0, 1);
    // lcd.print(prev_button_lcd_str);
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
    String to_print;

    // Clear the lcd screen
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();

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
        lcd.setCursor(0, 1);
        lcd.print(prev_button_lcd_str);
        break;
    }

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
    case RCV_WEST_DECEL_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: '1' - West Decelerate");
        to_print = "Decelerating";
        handle_direction_rcv('w');
        break;
    case RCV_WEST_ACEL_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: '2' - West Accelerate");
        to_print = "Accelerating";
        handle_direction_rcv('w');
        break;
    case RCV_WEST_CONST_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: '3' - West Constant");
        to_print = "Constant Speed";
        handle_direction_rcv('w');
        break;
    case RCV_EAST_DECEL_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: '4' - East Decelerate");
        to_print = "Decelerating";
        handle_direction_rcv('e');
        break;
    case RCV_EAST_ACEL_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: '5' - East Accelerate");
        to_print = "Accelerating";
        handle_direction_rcv('e');
        break;
    case RCV_EAST_CONST_CHAR:
        to_print = "Constant Speed";
        Serial.println("lcd.update_lcd:: rcvd: '6' - East Constant");
        handle_direction_rcv('e');
        break;
    case RCV_EAST_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'e' - East");
        handle_direction_rcv('e');
        break;
    case RCV_WEST_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'w' - West");
        handle_direction_rcv('w');
        break;
    case RCV_EMERGENCY_CHAR:
        Serial.println("lcd.update_lcd:: rcvd: 'x' - Emergency stop");
        to_print = "EMERGENCY STOP";
        break;
    default:
        to_print = prev_message;
        break;
    }

    prev_message = to_print;
    lcd.home();
    clear_top_row();
    lcd.print(to_print);

    if (b.chars[0] == EMERGENCY_STOP_CHAR) {
        lcd.home();
        lcd.clear();
        lcd.print("EMERGENCY STOP");
    }
}

void handle_direction_button(bool toggled)
{
    Serial.println("Direction button");

    is_east = !toggled;
}

void handle_start_stop_button(bool toggled)
{
    clear_bottom_row();
    lcd.setCursor(0, 1);
    if (started == true) {
        lcd.print("Stopping");
        prev_button_lcd_str = "Stopping";
    } else {
        lcd.print("Starting");
        prev_button_lcd_str = "Starting";
    }

    started = !toggled;
}

void handle_door_button(bool toggled)
{
    clear_bottom_row();
    lcd.setCursor(0, 1);
    if (door_open == true)
    {
        lcd.print("Door Opening");
        prev_button_lcd_str = "Door Opening";
    }
    else
    {
        lcd.print("Door Closing");
        prev_button_lcd_str = "Door Closing";
    }

    door_open = !toggled;
}

void handle_direction_rcv(char dir)
{
    String to_print = dir == 'e' ? "East" : "West";

    lcd.setCursor(0, 1);
    clear_bottom_row();
    lcd.print(to_print);

    is_east = !is_east;
}

void clear_bottom_row()
{
    lcd.setCursor(0, 1);
    lcd.print("                ");
}

void clear_top_row()
{
    lcd.setCursor(0, 0);
    lcd.print("                ");
}

void debug_print_button(button b) {
    char b_info[30];

    Serial.print("lcd.debug_print_button:: Pressed button: ");
    sprintf(b_info, "chars = [%c, %c]", b.chars[0], b.chars[1]);

    Serial.println(b_info);

}