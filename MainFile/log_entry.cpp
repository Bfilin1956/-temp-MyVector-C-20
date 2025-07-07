#include "log_entry.h"
#include "log_processor.h"
#include "plugin_loader.h"

#include <chrono>
#include <filesystem>
#include <unordered_map>
#include <iostream>

LogEntry::LogEntry(std::string log_file, std::string out_file, PluginLoader* inputpl) : pl(inputpl) {
	proc = new LogProcessor(log_file, out_file);
}

void LogEntry::load_queue() {
	proc->open_file();
	while (true) {
		
		std::string line = proc->read_log();
		
		if (line.empty()) {
			{
				std::lock_guard<std::mutex> lock(mtx);
				done = true;
			}
			cv.notify_all();
			break;
		}
		{
			std::unique_lock<std::mutex> lock(mtx);
			while (buffer.size() > 20) lock.unlock(), std::this_thread::sleep_for(std::chrono::milliseconds(50)), lock.lock();

			buffer.push(line);
		}
	}
    cv.notify_one();
}

LogLevel parse_level(const std::string& level_str) {
	static const std::unordered_map<std::string, LogLevel> level_map = {
		{"INFO", LogLevel::INFO},
		{"WARNING", LogLevel::WARNING},
		{"ERROR", LogLevel::ERROR_}
	};
	auto it = level_map.find(level_str);
	if (it == level_map.end()) {
		throw std::runtime_error("Unknown log level: " + level_str);
	}
	return it->second;
}

stLogEntry parse_line(const std::string& line) {
	stLogEntry entry;

	size_t pos1 = line.find('[');
	size_t pos2 = line.find(']');
	if (pos1 == std::string::npos || pos2 == std::string::npos) {
		throw std::runtime_error("Invalid format (timestamp)");
	}
	entry.timestamp = line.substr(pos1 + 1, pos2 - pos1 - 1);
	pos1 = line.find('[', pos2 + 1);
	pos2 = line.find(']', pos1);
	if (pos1 == std::string::npos || pos2 == std::string::npos) {
		throw std::runtime_error("Invalid format (level)");
	}
	std::string level_str = line.substr(pos1 + 1, pos2 - pos1 - 1);
	entry.level = parse_level(level_str);

	entry.message = line.substr(pos2 + 2); 

	return entry;
}

void LogEntry::make_out_file(short unsigned int i) {
	std::string tempfile = "temp" + std::to_string(i);
	
	if (!std::filesystem::exists(tempfile)) {
		std::ofstream create_file(tempfile);
		if (!create_file) {
			throw std::runtime_error("Can't create tempfile file");
		}
	}
	std::fstream fs;
	fs.open(tempfile);
	while (true) {
		std::string line;
		std::string templine;

		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [&] { return !buffer.empty() || done; });
			if (buffer.empty() && done) break;
			line = std::move(buffer.front());
			buffer.pop();
		}
		templine = line;
		
		stLogEntry entry = parse_line(line);

		if (pl->apply_filter(entry)) {

			fs << templine << '\n';
			fs.flush();
		}
	}
	fs.close();
}

void LogEntry::filter_file() {

	std::thread load(&LogEntry::load_queue, this);
	std::thread worker1(&LogEntry::make_out_file, this, 1);
	std::thread worker2(&LogEntry::make_out_file, this, 2);
	load.join();
	worker1.join();
	worker2.join();

	proc->write_out_log();
}
