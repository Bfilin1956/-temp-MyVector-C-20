#include "utils.h"

Verifier::Verifier(fs::path file, fs::path dll) : log_file_(std::move(file)), plugin_file_(std::move(dll)) {}

bool Verifier::has_log_file() const {
	return fs::exists(log_file_) && fs::is_regular_file(log_file_);
}

bool Verifier::has_plugin_file() const {
	return fs::exists(plugin_file_) && fs::is_regular_file(plugin_file_);
}