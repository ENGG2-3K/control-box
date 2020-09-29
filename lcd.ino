#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include "lib.h"

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display
int show = -1;

void init_lcd()
{
    int error;

    Serial.begin(9600);
    Serial.println("LCD...");

    // wait on Serial to be available on Leonardo
    while (!Serial)
    {
        ;
    }

    Serial.println("Dose: check for LCD");

    Wire.begin();
    Wire.beginTransmission(0x27);
    error = Wire.endTransmission();
    Serial.print("Error: ");
    Serial.print(error);

    if (error == 0)
    {
        Serial.println(": LCD found.");
        show = 0;
        lcd.begin(16, 2); // initialize the lcd
    }
    else
    {
        Serial.println(": LCD not found.");
    } // if
}

void update_lcd(button b, mega_info rcvd_info, bool is_east)
{
    // Clear the lcd screen
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");

    //    switch (rcvd_info.rcvd_char)
    //    {
    //    case RCV_ACCELERATE_CHAR:
    //        Serial.println("rcvd: 'a' - Accelerating");
    //        lcd.print("Accelerating");
    //        break;
    //    case RCV_DECELERATE_CHAR:
    //        Serial.println("rcvd: 'd' - Decelerating");
    //        lcd.print("Decelerating");
    //        break;
    //    case RCV_CONSTANT_SPEED_CHAR:
    //        Serial.println("rcvd: 'n' - Constant speed");
    //        lcd.print("Constant speed");
    //        break;
    //    case RCV_STOPPED_CHAR:
    //        Serial.println("rcvd: 's' - Stopped");
    //        lcd.print("Stopped");
    //        break;
    //    case RCV_EMERGENCY_CHAR:
    //        Serial.println("rcvd: 'x' - Emergency stop");
    //        lcd.print("Emergency stop");
    //        break;
    //    }
    //
    //    lcd.setCursor(0, 1);
    //
    //    if (is_east == true)
    //    {
    //        lcd.print("East");
    //    }
    //    else
    //    {
    //        lcd.print("West");
    //    }
}
