#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <Arduino.h>
#include <functional>
#include <map>

// Define the CommandCallback type for clarity
using CommandCallback = std::function<void(const String &)>;

class CommandHandler
{
public:
    void registerCommand(const String &command, CommandCallback handler);
    void handleCommand(const String &input);

private:
    std::map<String, CommandCallback> commandMap;
};

#endif // COMMAND_HANDLER_H
