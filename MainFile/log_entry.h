#pragma once
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "log_types.h"

class PluginLoader;
class LogProcessor;



class LogEntry {
    std::queue<std::string> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool done = false;
    LogProcessor *proc;
    PluginLoader* pl;

public:
    LogEntry(std::string log_file, std::string out_file, PluginLoader* inputpl);

    void load_queue();

    void make_out_file(short unsigned int);

    void filter_file();

};