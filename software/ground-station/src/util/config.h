#pragma once

#include <charconv>
#include <format>
#include <string>
#include <system_error>
#include <fstream>
#include "defines.h"
#include "src/util/logger.h"

namespace Config {
	// CONFIG VARIABLES
	inline std::string config_file_path;
	inline u32 window_width = 800;
	inline u32 window_height = 500;

	namespace {
        inline std::string trim(std::string_view sv) {
            auto start = sv.find_first_not_of(" \t\r\n");
            auto end   = sv.find_last_not_of(" \t\r\n");
            return (start == std::string_view::npos) ? "" : std::string(sv.substr(start, end - start + 1));
        }

        inline int parse_int(const std::string& val, const std::string& key) {
            int result{};
            auto [ptr, ec] = std::from_chars(val.data(), val.data() + val.size(), result);
            if (ec != std::errc{})
                throw std::runtime_error(std::format("Config: '{}' expects an integer, got '{}'", key, val));
            return result;
        }
	}

	// CONFIG FUNCS
	inline void init(const std::string& path) {
		// first loads admin files for config path
		std::ifstream file("res/admin.txt");
		if (!file.is_open()) {
			Log::fatal("admin.txt file was deleted, app cant start");
			abort();
		}
		
		(void)path;
		
		std::string line;
		std::getline(file, line);
#if defined(__APPLE__)
		std::getline(file, line);
#elif defined(_WIN32)
		std::getline(file, line);
		std::getline(file, line);
#elif defined(__linux__)
		std::getline(file, line);
		std::getline(file, line);
		std::getline(file, line);
#endif

		config_file_path = line;
		file.close();

		// loading in toml config file
		file.open(config_file_path);


		file.close();
	}

	inline void save(void) {
		
	}

	inline void reset(void) {
		init(config_file_path);
	}
}
