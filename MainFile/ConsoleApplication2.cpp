#include <iostream>
#include <string>

#include "utils.h"
#include "plugin_loader.h"
#include "log_entry.h"

int main(int argc, char* argv[]) {
	try {
		if (argc < 2) {
			std::cout << "Usage: --log <file> --filter <dll> --output <file>\n";
			return 0;
		}

		std::string log_file, filter_path, output_file;

		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg == "--log" && i + 1 < argc) {
				log_file = argv[++i];
			}
			else if (arg == "--filter" && i + 1 < argc) {
				filter_path = argv[++i];
			}
			else if (arg == "--output" && i + 1 < argc) {
				output_file = argv[++i];
			}
		}

		auto verifier = std::make_unique<Verifier>(log_file, filter_path);
		if (!verifier->has_log_file()) {
			std::cerr << "Log file not found\n";
			return 1;
		}
		if (!verifier->has_plugin_file()) {
			std::cerr << "Filter DLL not found\n";
			return 1;
		}

		auto pluginloader = std::make_unique<PluginLoader>(filter_path);
		pluginloader->load_plugin();

		auto logentry = std::make_unique<LogEntry>(log_file, output_file, pluginloader.get());
		logentry->filter_file();
	}
	catch (const std::exception& e) {
		std::cerr << "Unhandled exception: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
