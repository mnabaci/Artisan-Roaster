#ifndef CHAN_COMMAND_H
#define CHAN_COMMAND_H

#include <Arduino.h>
#include "../CommandHandler/CommandHandler.h"

// Acks Artisan's channel-config handshake on connect. This board doesn't support
// per-channel selection, so there's nothing to apply beyond acknowledging it.
void handleChanCommand(const String &)
{
    Serial.println("#OK");
}

#endif
