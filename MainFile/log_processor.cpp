#include "log_processor.h"
#include <stdexcept>
#include <filesystem>

LogProcessor::LogProcessor(std::string log_file, std::string out_log): log_file_(log_file), out_file_(out_log){}

LogProcessor::~LogProcessor(){
	if (fs_.is_open()) { fs_.close(); }
}

void LogProcessor::open_file() {
	fs_.open(log_file_);
	if (!fs_) {
		throw std::runtime_error("Log file dont open: " + log_file_);
	}
}

std::string LogProcessor::read_log() {
	std::string line;

	if (std::getline(fs_, line)) {
		return line;
	}
	else {
		return {};
	}
}

void LogProcessor::write_out_log() {
	std::ifstream file1("temp1", std::ios::binary);
	std::ifstream file2("temp2", std::ios::binary);
	std::ofstream output(out_file_, std::ios::binary);

	if (!file1.is_open() || !file2.is_open() || !output.is_open()) {
		throw std::runtime_error("Can't open one of the files");
	}

	output << file1.rdbuf();
	output << file2.rdbuf();

	file1.close();
	file2.close();
	output.close();

	std::remove("temp1");
	std::remove("temp2");
}
