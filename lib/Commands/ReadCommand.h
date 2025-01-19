#ifndef READ_COMMAND_H
#define READ_COMMAND_H

#include <Arduino.h>
#include "../../main/main.h"
#include "../CommandHandler/CommandHandler.h"


extern float readTemperature(TemperatureUnit unit, int sensor);

void handleReadCommand(const String &)
{
    float temp1 = readTemperature(tempUnit, 1);
    float temp2 = readTemperature(tempUnit, 2);
    if (temp1 < 0 || temp2 < 0)
    {
        Serial.println("#ERROR: Invalid Temperature Data");
        return;
    }
    Serial.print("30.00,");
    Serial.print(temp1);
    Serial.print(",");
    Serial.print(temp2);
    Serial.println(",10.00,20.00");
}

#endif
