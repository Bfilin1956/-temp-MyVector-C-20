#pragma once

#include <string>

enum class LogLevel { INFO, WARNING, ERROR_ };

struct stLogEntry {
    std::string timestamp;
    LogLevel level;
    std::string message;
};
