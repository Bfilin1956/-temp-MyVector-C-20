#include "plugin_loader.h"
#include <stdexcept>

PluginLoader::PluginLoader(std::string filter_path) : plugin_file_(std::move(filter_path)) {}

void PluginLoader::load_plugin() {
	dll = LoadLibraryA(plugin_file_.c_str());
	if (!dll) throw std::runtime_error("Failed to load DLL: " + plugin_file_);

	FARPROC proc = GetProcAddress(dll, "filter");
	if(!proc) throw std::runtime_error("Failed to get 'filter' symbol");

	filter_ = reinterpret_cast<FilterFunc>(proc);
}

PluginLoader::~PluginLoader() {
	if (dll) {
		FreeLibrary(dll);
	}
}

bool PluginLoader::apply_filter(const stLogEntry& entry) const {
	return filter_(entry);
}