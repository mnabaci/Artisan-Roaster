#ifndef FAN_COMMAND_H
#define FAN_COMMAND_H

#include <Arduino.h>
#include "../CommandHandler/CommandHandler.h"

// Artisan sends a 0-100 duty value on its fan channel. This board's fan is a plain relay
// (no PWM), so any nonzero value just turns it on.
void handleFanCommand(const String &args)
{
    int value = args.toInt();
    if (value < 0 || value > 100)
    {
        Serial.println("#ERROR: Invalid Fan State");
        return;
    }
    digitalWrite(PIN_FAN, value > 0 ? HIGH : LOW);
    Serial.println(value > 0 ? "#OK Fan ON" : "#OK Fan OFF");
}

#endif
