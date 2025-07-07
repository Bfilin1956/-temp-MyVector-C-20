#include "keyword_filter.h"

static const std::string keyword{ "ERROR" };

extern "C" __declspec(dllexport)
bool filter(const stLogEntry& entry) {
    return entry.message.find(keyword) != std::string::npos;
}