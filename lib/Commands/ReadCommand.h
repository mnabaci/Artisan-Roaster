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
    // No heater output on this board (see HeatCommand.h); fan is a plain on/off relay.
    Serial.print("30.00,");
    Serial.print(temp1);
    Serial.print(",");
    Serial.print(temp2);
    Serial.print(",0.00,");
    Serial.println(digitalRead(PIN_FAN) ? "100.00" : "0.00");
}

#endif
