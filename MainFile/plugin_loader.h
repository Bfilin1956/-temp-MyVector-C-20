#pragma once

#include "log_types.h"
#include <windows.h>



class PluginLoader final {
	std::string plugin_file_;
	HMODULE dll;
	using FilterFunc = bool(*)(const stLogEntry&);
	FilterFunc filter_;
public:
	PluginLoader(std::string filter_path);
	~PluginLoader();

	void load_plugin();
	bool apply_filter(const stLogEntry& entry) const;

};