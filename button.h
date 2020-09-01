// Each button will contain state saying whether:
// * It has been toggled or not
// * Which pin it is attached to
// * Which possible 2 characters it can send to the Mega
//
// The emergency stop button will only have one possible char it can send
typedef struct button
{
    char pin_number; // The pin that the button is assigned to
    char chars[2];   // The two possible characters than can be sent for this button
    char toggled;    // Whether the button has been toggled or not
} button;