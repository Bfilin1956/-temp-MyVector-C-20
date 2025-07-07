#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class Verifier final {
	fs::path log_file_;
	fs::path plugin_file_;

public:
	Verifier(fs::path log_file, fs::path plugin_file);

	bool has_log_file() const;
	bool has_plugin_file() const;
};