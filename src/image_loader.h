#pragma once

#include "program_data.h"

#define IL_MAX_ARGS 3

namespace imageLoader {
	enum {
		NONE,
		START_COM,
		END_COM,
	};

	typedef unsigned int COMMAND;

	struct CommandData {
		COMMAND command;
		char args[IL_MAX_ARGS][NAME_MAX];
		int arg_count;

		bool operator==(const CommandData& other) const {
			int i;

			// Varibles
			if (command != other.command || arg_count != other.arg_count)
				return false;

			// Argument list.
			for (i = 0; i < IL_MAX_ARGS; i++)
				if (strncmp(args[i], other.args[i], NAME_MAX) != 0)
					return false;

			return true;
		}
	};

	std::vector<Fl_PNG_Image*> load_images(const char image_paths[][NAME_MAX], size_t n);
	std::vector<Fl_PNG_Image*> load_images(const std::vector<std::string> image_paths);

	std::vector<std::string> get_paths(const char * loader_file_path, CommandData search_com, bool get_all=false);
	int get_command(const char * command, CommandData * com_data);

	COMMAND str_to_type(const char * command);
	void clear_command(CommandData * com_data);

	std::vector<std::string> add_path(const std::vector<std::string> image_paths, const char * new_path);
}
