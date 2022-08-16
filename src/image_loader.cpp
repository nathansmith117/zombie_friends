#include "image_loader.h"

namespace imageLoader {
	std::vector<Fl_PNG_Image*> load_images(const char image_paths[][NAME_MAX], size_t n) {
		long int i, error_count = 0;
		std::vector<Fl_PNG_Image*> img_list;

		puts("\nLoading images.");

		for (i = 0; i < n; i++) {
			printf("Loading %s\n", image_paths[i]);

			img_list.push_back(new Fl_PNG_Image(image_paths[i]));

			// Check for errors.
			if (img_list[i]->fail()) {
				error_count++;
				fprintf(stderr, "Error loading %s\n", image_paths[i]);
			}
		}

		printf("Finished loading %ld image(s) with %ld error(s)\n", n, error_count);
		return img_list;
	}

	std::vector<Fl_PNG_Image*> load_images(const std::vector<std::string> image_paths) {
		long int i, error_count = 0;
		std::vector<Fl_PNG_Image*> img_list;

		puts("\nLoading images.");

		for (i = 0; i < image_paths.size(); i++) {
			printf("Loading %s\n", image_paths[i].c_str());

			img_list.push_back(new Fl_PNG_Image(image_paths[i].c_str()));

			// Check for errors.
			if (img_list[i]->fail()) {
				error_count++;
				fprintf(stderr, "Error loading %s\n", image_paths[i].c_str());
			}
		}

		printf("Finished loading %ld image(s) with %ld error(s)\n", image_paths.size(), error_count);
		return img_list;
	}

	std::vector<std::string> get_paths(const char * loader_file_path, CommandData search_com, bool get_all) {
		std::vector<std::string> img_paths;
		FILE * loader_file;
		struct stat file_stat;
		long int i;

		int curr_line_i = 0;
		char curr_char;
		char curr_line[NAME_MAX];
		bool is_commeted = false;
		bool load_stuff = false;
		CommandData com_data;

		if (get_all)
			load_stuff = true;

		memset(curr_line, 0, NAME_MAX);

		// Get stat.
		if (stat(loader_file_path, &file_stat) == -1) {
			fprintf(stderr, "Error getting a stat on %s\n", loader_file_path);
			return img_paths;
		}

		// Open file.
		loader_file = fopen(loader_file_path, "r");

		if (loader_file == NULL) {
			fprintf(stderr, "Error opening %s\n", loader_file_path);
			return img_paths;
		}

		for (i = 0; i < file_stat.st_size; i++) {
			curr_char = fgetc(loader_file);

			// Search for end of file.
			if (feof(loader_file)) {
				fprintf(stderr, "%s ended early\n", loader_file_path);
				break;
			}

			// New lines.
			if (curr_char == '\n') {

				// Load line into buffer.
				if ((load_stuff && curr_line[0] != '.') || get_all)
					img_paths.push_back(curr_line);

				// Commands.
				if (curr_line[0] == '.' && !get_all) {

					if (get_command(curr_line, &com_data) == -1) {
						fclose(loader_file);
						return img_paths;
					}

					if (com_data == search_com)
						load_stuff = true;
					if (com_data.command == imageLoader::END_COM)
						load_stuff = false;
				}

				// Reset vars.
				is_commeted = false;
				curr_line_i = 0;
				memset(curr_line, 0, NAME_MAX);
				continue;
			}

			// Commets.
			if (curr_char == '#')
				is_commeted = true;
			if (is_commeted)
				continue;

			// Get current line.
			curr_line[curr_line_i] = curr_char;
			curr_line_i++;

			if (curr_line_i - 1 >= NAME_MAX) {
				fprintf(stderr, "Lines in %s are to long\n", loader_file_path);
				break;
			}
		}

		fclose(loader_file);
		return img_paths;
	}

	int get_command(const char * command, CommandData * com_data) {
		int i, arg_count = 0, arg_i = 0;
		bool check_for_command = true;
		char command_buf[NAME_MAX];

		memset(command_buf, 0, sizeof(command_buf));

		for (i = 0; i < IL_MAX_ARGS; i++)
			memset(com_data->args[i], 0, NAME_MAX);

		// Check stuff.
		if (command == NULL) {
			fprintf(stderr, "Command shouldn't be null\n");
			return -1;
		} else if (command[0] != '.') {
			fprintf(stderr, "\"%s\" should start with a '.'\n", command);
			return -1;
		}

		for (i = 1; i < NAME_MAX; i++) {

			if (arg_count > IL_MAX_ARGS) {
				fprintf(stderr, "To many arguments in \"%s\"\n", command);
				return -1;
			}

			// Check for white space.
			switch (command[i]) {
				case ' ':
					if (check_for_command)
						check_for_command = false;
					else
						arg_count++;

					arg_i = 0;
					continue;
				case '\t':
					return -1;
				case '\n':
				case '\0':
					goto after_loop;
				default:
					break;
			}

			// Get command.
			if (check_for_command)
				command_buf[i - 1] = command[i];
			else
				com_data->args[arg_count][arg_i] = command[i];

			arg_i++;
		}

		after_loop:

		com_data->command = str_to_type(command_buf);
		com_data->arg_count = arg_count + 1;

		return 0;
	}

	COMMAND str_to_type(const char * command) {
		if (strncasecmp(command, "start", NAME_MAX) == 0)
			return START_COM;
		else if (strncasecmp(command, "end", NAME_MAX) == 0)
			return END_COM;
		else
			return -1;
	}

	void clear_command(CommandData * com_data) {
		com_data->command = NONE;
		com_data->arg_count = 0;

		for (auto i : com_data->args)
			memset(i, 0, NAME_MAX);
	}

	std::vector<std::string> add_path(const std::vector<std::string> image_paths, 
			const char * new_path) {
		std::vector<std::string> new_list;

		for (auto i : image_paths)
			new_list.push_back(new_path + i);

		return new_list;
	}
}
