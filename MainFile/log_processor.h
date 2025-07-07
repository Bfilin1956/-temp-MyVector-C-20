#pragma once
#include <string>
#include <fstream>

class LogProcessor {
	std::string log_file_;
	std::string out_file_;
	std::fstream fs_;
	std::fstream out_fs_;
	
public:
	LogProcessor(std::string log_file, std::string out_log);
	~LogProcessor();
	std::string read_log();
	void write_out_log();
	void open_file();
};