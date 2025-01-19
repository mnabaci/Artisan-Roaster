#ifndef UNITS_COMMAND_H
#define UNITS_COMMAND_H

#include <Arduino.h>
#include "../../main/main.h"
#include "../CommandHandler/CommandHandler.h"

extern TemperatureUnit tempUnit;

void handleUnitsCommand(const String &args)
{
    char unit = args.charAt(0);
    if (unit == 'F')
    {
        tempUnit = FAHRENHEIT;
        Serial.println("#OK Fahrenheit");
    }
    else if (unit == 'C')
    {
        tempUnit = CELSIUS;
        Serial.println("#OK Celsius");
    }
    else
    {
        Serial.println("#ERROR: Invalid Unit");
    }
}

#endif
