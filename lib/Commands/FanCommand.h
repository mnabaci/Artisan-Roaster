#ifndef FAN_COMMAND_H
#define FAN_COMMAND_H

#include <Arduino.h>
#include "../CommandHandler/CommandHandler.h"

void handleFanCommand(const String &args)
{
    char state = args.charAt(0);
    if (state == '0')
    {
        digitalWrite(PIN_FAN, LOW); // Turn off fan
        Serial.println("#OK Fan OFF");
    }
    else if (state == '1')
    {
        digitalWrite(PIN_FAN, HIGH); // Turn on fan
        Serial.println("#OK Fan ON");
    }
    else
    {
        Serial.println("#ERROR: Invalid Fan State");
    }
}

#endif
