#ifndef HEAT_COMMAND_H
#define HEAT_COMMAND_H

#include <Arduino.h>
#include "../CommandHandler/CommandHandler.h"

// OT1 is Artisan's heater channel. This board has no heater output wired up, so there's
// nothing to drive; ack the command so Artisan's heater slider doesn't read as a comms error.
void handleHeatCommand(const String &)
{
    Serial.println("#OK No heater output on this board");
}

#endif
