#include "CommandHandler.h"

void CommandHandler::registerCommand(const String &command, CommandCallback callback) {
    commandMap[command] = callback;
}

void CommandHandler::handleCommand(const String &input) {
    String command = input.substring(0, input.indexOf(';')); // Extract command part
    String arguments = input.substring(input.indexOf(';') + 1); // Extract arguments part

    if (commandMap.find(command) != commandMap.end()) {
        commandMap[command](arguments); // Call the corresponding callback
    } else {
        Serial.println("#ERROR: Unknown Command");
    }
}
