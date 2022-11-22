#include "game_commands.h"
#include "game_tools.h"
#include "player.h"
#include <linux/limits.h>

PRINT_TYPE get_print_type_from_str(const char * str) {
	if (str == NULL)
		return PRINT_UNKNOWN;

	if (strncmp(str, PRINT_STDOUT_ARG, NAME_MAX) == 0) // Stdout.
		return PRINT_STDOUT;
	else if (strncmp(str, PRINT_STDERR_ARG, NAME_MAX) == 0) // Stderr.
		return PRINT_STDERR;
	else if (strncmp(str, PRINT_CHATBOX_ARG, NAME_MAX) == 0) // Chat box.
		return PRINT_CHATBOX;
	else // Unknown.
		return PRINT_UNKNOWN;
}

int print_to(PRINT_TYPE type, const char * message, ChatBox * chat_box) {
	if (message == NULL)
		return -1;

	switch (type) {
		case PRINT_UNKNOWN:
			return -1;
		case PRINT_STDOUT:
			printf("%s\n", message);
			break;
		case PRINT_STDERR:
			fprintf(stderr, "%s\n", message);
			break;
		case PRINT_CHATBOX:
			if (chat_box != NULL)
				chat_box->printline(message);

			break;
		default:
			fprintf(stderr, "Unknown print type '%d'\n", type);
			return -1;
	}

	return 0;
}

int say_command(COM_CB_ARGS) {
	size_t print_buf_size = NAME_MAX * arg_count;
	char print_buf[print_buf_size];

	// Print new line if no args.
	if (arg_count <= 1) {
		chat_box->printline("");
		return 0;
	}

	memset(print_buf, 0, print_buf_size);

	// Get message from 'args'. Start at 1 to skip command.
	if (gameTools::str_array_to_str(args, NAME_MAX, arg_count, " ", print_buf, print_buf_size, 1) == -1) {
		fputs("Error getting message\n", stderr);
		return -1;
	}

	chat_box->printline(print_buf);
	return 0;
}

int exit_command(COM_CB_ARGS) {
	exit(0);
	return 0;
}

// Warning messy eye straining code!
int load_command(COM_CB_ARGS) {
	int i;
	int res;
	bool use_image_folder_location = false;
	gameTools::GAME_DATA_TYPE data_type;

	char file_ext[NAME_MAX];
	char file_location[NAME_MAX];

	unsigned char load_location;
	const char * script_location = NULL;

	size_t image_folder_size = NAME_MAX + MAIN_DIR_SIZE;
	char image_folder[image_folder_size];

	char load_file_path[NAME_MAX];
	char load_file_name[NAME_MAX];

	char ** dir_args = NULL;

	const char * image_folder_locat = NULL;

	if (arg_count < 2) {
		fputs("At least one argument required\n", stderr);
		return -1;
	}

	// Is a directory.
	struct stat stat_buf;

	// Skip if something goes wrong.
	// File not found errors will be cought later on.
	if (stat(args[1], &stat_buf) != 0)
		goto skip_checking_is_dir;

	if (S_ISDIR(stat_buf.st_mode)) {
		// Format 'load_file_path'. directory/init.cms
		memset(load_file_path, 0, NAME_MAX);
		snprintf(load_file_path, NAME_MAX, "%s%sinit.cms", args[1], PSLASH);

		// Create arguments.
		dir_args = gameTools::create_2d_array<char>(NAME_MAX, arg_count);

		if (dir_args == NULL) {
			fputs("Error getting memory for 'dir_args'\n", stderr);
			return -1;
		}

		// Copy 'args'.
		for (i = 0; i < arg_count; i++) {
			memset(dir_args[i], 0, NAME_MAX);
			strncat(dir_args[i], args[i], NAME_MAX);
		}

		// set file path.
		memset(dir_args[1], 0, NAME_MAX);
		strncat(dir_args[1], load_file_path, NAME_MAX);

		// Run 'load_command' with 'dir_args'
		res = load_command(arg_count, (const char**)dir_args, mdata, chat_box, variable_list, d);
		gameTools::delete_2d_array<char>(dir_args, NAME_MAX, arg_count);
		return res;
	}

skip_checking_is_dir:

	// get 'load_file_path'.
	memset(load_file_path, 0, NAME_MAX);
	strncat(load_file_path, args[1], NAME_MAX - 1);

	if (gameTools::get_file_name(args[1], load_file_name) == -1) {
		fprintf(stderr, "Error getting file name of %s\n", args[1]);
		return -1;
	}

	// Get load location.
	if (arg_count < 3) {
		load_location = LOAD_MAIN_DIR;
		goto skip_getting_load_location;
	}

	if (strncmp(args[2], LOAD_MAIN_DIR_ARG, NAME_MAX) == 0) // MAIN_DIR
		load_location = LOAD_MAIN_DIR;
	else if (strncmp(args[2], LOAD_SCRIPT_LOCATION_ARG, NAME_MAX) == 0) // Script location
		load_location = LOAD_SCRIPT_LOCATION;
	else // Default
		load_location = LOAD_MAIN_DIR;

skip_getting_load_location:

	// File type handling.
	memset(file_ext, 0, NAME_MAX);

	if (gameTools::get_file_ext(args[1], file_ext, NAME_MAX) == -1) {
		fprintf(stderr, "Error getting file type for %s\n", args[1]);
		return -1;
	}

	data_type = gameTools::file_ext_to_datatype(file_ext);

	// Format script location into 'load_file_path'.
	if (load_location == LOAD_SCRIPT_LOCATION && data_type != gameTools::GAME_COMMAND_SCRIPT) {
		script_location = chat_box->get_script_location();

		if (script_location[0] == '\0')
			goto load_map_branch;

		snprintf(load_file_path, NAME_MAX, "%s%s", script_location, args[1]);
	}

	// image folder buf.
	memset(image_folder, 0, image_folder_size);

	// Skip getting image folder.
	if (arg_count == 2)
		goto load_map_branch;

	use_image_folder_location = true;

	// Get image folder location.
	switch (load_location) {
		case LOAD_MAIN_DIR:
			image_folder_locat = mdata->MAIN_DIR;
			break;
		case LOAD_SCRIPT_LOCATION:
			image_folder_locat = chat_box->get_script_location();
			break;
		default:
			image_folder_locat = mdata->MAIN_DIR;
			break;
	}

	// Script location not set.
	if (chat_box->get_script_location()[0] == '\0')
		image_folder_locat = mdata->MAIN_DIR;

	// format image_folder.
	snprintf(image_folder, image_folder_size, "%simages%s", image_folder_locat, PSLASH);

load_map_branch:

	// Load map.
	res = gameTools::load_file(mdata, load_file_path, use_image_folder_location ? image_folder : NULL);

	if (res == -1)
		fprintf(stderr, "Error loading %s\n", load_file_path);

	return res;
}

int do_nothing_command(COM_CB_ARGS) {
	// Do nothing.
	return 0;
}

int popup_command(COM_CB_ARGS) {
	size_t message_buf_size = NAME_MAX * arg_count;
	char message_buf[message_buf_size];

	if (arg_count <= 1) {
		fputs("Message required\n", stderr);
		return -1;
	}

	memset(message_buf, 0, message_buf_size);

	// Get message from 'args'. Start at 1 to skip command.
	if (gameTools::str_array_to_str(args, NAME_MAX, arg_count, " ", message_buf, message_buf_size, 1) == -1) {
		fputs("Error getting message\n", stderr);
		return -1;
	}

	fl_message(message_buf);
	return 0;
}

int print_command(COM_CB_ARGS) {
	size_t print_buf_size = NAME_MAX * arg_count;
	char print_buf[print_buf_size];
	PRINT_TYPE print_stream_type;

	// Two arguments
	if (arg_count < 3) {
		fputs("At least two aguments for printing\n", stderr);
		return -1;
	}

	// Get output stream.
	print_stream_type = get_print_type_from_str(args[1]);

	if (print_stream_type == PRINT_UNKNOWN) {
		fputs("Error getting print type\n", stderr);
		return -1;
	}

	// Get message from 'args'.
	memset(print_buf, 0, print_buf_size);

	// Start at 2 to skip command and print stream.
	if (gameTools::str_array_to_str(args, NAME_MAX, arg_count, " ", print_buf, print_buf_size, 2) == -1) {
		fputs("Error getting message\n", stderr);
		return -1;
	}

	// Print message to stream.
	if (print_to(print_stream_type, print_buf, chat_box) == -1) {
		fprintf(stderr, "Error printing '%s' to '%s'\n", print_buf, args[1]);
		return -1;
	}

	return 0;
}

int run_sequence_command(COM_CB_ARGS) {
	int i, j;
	bool at_least_two_commands = false;
	std::vector<std::string> command_list;

	if (arg_count < 4) {
		fputs("At least two commands required\n", stderr);
		return -1;
	}

	// Get commands.  Skip first argument.
	command_list.push_back(std::string());
	j = 0;

	for (i = 1; i < arg_count; i++) {
		if (strncmp(args[i], "|", NAME_MAX) == 0) { // Next command.
			command_list.push_back(std::string());
			at_least_two_commands = true;
			j++;
			continue;
		}

		// Add command and args with space.
		if (command_list[j].empty())
			command_list[j] += std::string(args[i]);
		else
			command_list[j] += " " + std::string(args[i]);
	}

	if (!at_least_two_commands) {
		fputs("At least two commands required\n", stderr);
		return -1;
	}

	// Run commands.
	for (auto c : command_list) {
		if (c.empty()) {
			fputs("Error running sequence\n", stderr);
			return -1;
		}

		if (chat_box->run_command(c.c_str()) == -1) {
			fprintf(stderr, "Error running '%s'\n", c.c_str());
			return -1;
		}
	}

	return 0;
}

int set_command(COM_CB_ARGS) {
	int i;
	VariableData curr_var;
	
	size_t name_and_value_size = arg_count * NAME_MAX;
	char name_and_value[name_and_value_size];

	if (arg_count < 3) {
		fputs("Variable name and value required\n", stderr);
		return -1;
	}

	// Get name and value. Skip command name.
	if (gameTools::str_array_to_str(args, NAME_MAX, arg_count, " ", name_and_value, name_and_value_size, 1) == -1) {
		fputs("Error getting variable name and value from 'args'\n", stderr);
		return -1;
	}

	// Get variable data.
	if (ChatBox::variable_str_to_data(name_and_value, name_and_value_size, &curr_var) == -1) {
		fprintf(stderr, "Error getting variable data from '%s'\n", name_and_value);
		return -1;
	}

	// Add or set global variable.
	if (chat_box->add_or_set_global_var(curr_var) == -1) {
		fprintf(stderr, "Error adding or setting global variable '%s'\n", curr_var.name);
		return -1;
	}

	return 0;
}

int show_globals_command(COM_CB_ARGS) {
	std::vector<VariableData> global_vars;
	global_vars = chat_box->get_global_vars();
	PRINT_TYPE print_type;

	std::string output_message;

	size_t name_and_value_size = NAME_MAX;
	char name_and_value[name_and_value_size];

	// Get print type.
	if (arg_count <= 1) {
		print_type = PRINT_CHATBOX;
		goto skip_getting_print_type;
	}

	print_type = get_print_type_from_str(args[1]);

	if (print_type == PRINT_UNKNOWN) {
		fputs("Error getting print type\n", stderr);
		return -1;
	}

skip_getting_print_type:

	if (global_vars.empty()) {
		output_message = "None";
		goto print_message_branch;
	}

	for (auto v : global_vars) {
		memset(name_and_value, 0, name_and_value_size);
		snprintf(name_and_value, name_and_value_size, "Name: %s, Value: %s\n", v.name, v.value);
		output_message += name_and_value;
	}

print_message_branch:

	// Print 'output_message'.
	if (print_to(print_type, output_message.c_str(), chat_box) == -1) {
		fputs("Error printing data\n", stderr);
		return -1;
	}

	return 0;
}

int if_command(COM_CB_ARGS) {
	int i;
	COMPARE_TYPE com_type;
	char command_str[NAME_MAX];

	bool case_true = false;

	if (arg_count < 5) {
		fputs("More arguments required\n", stderr);
		return -1;
	}

	// Get compare type.
	if (strncmp(args[2], EQUAL_ARG, NAME_MAX) == 0)
		com_type = COMPARE_EQUAL;
	else if (strncmp(args[2], NOT_EQUAL_ARG, NAME_MAX) == 0)
		com_type = COMPARE_NOT_EQUAL;
	else {
		fprintf(stderr, "Unknown compare type '%s'\n", args[2]);
		return -1;
	}

	// Get command.
	// Skip other arguments.
	if (gameTools::str_array_to_str(args, NAME_MAX, arg_count, " ", command_str, NAME_MAX, 4) == -1) {
		fputs("Error getting command\n", stderr);
		return -1;
	}

	// Compare values.
	switch (com_type) {
		case COMPARE_EQUAL:
			if (strncmp(args[1], args[3], NAME_MAX) == 0)
				case_true = true;

			break;
		case COMPARE_NOT_EQUAL:
			if (strncmp(args[1], args[3], NAME_MAX) != 0)
				case_true = true;

			break;
		default:
			fprintf(stderr, "Unknown compare type %d\n", com_type);
			return -1;
	}

	// Run command if case is true.
	if (case_true)
		return chat_box->run_command(command_str);

	return 0;
}

int list_commands_command(COM_CB_ARGS) {
	PRINT_TYPE print_type;
	std::vector<ChatCommand> command_list;
	std::string output_message;
	char buf[NAME_MAX];

	command_list = chat_box->commands();

	// Get print type.
	if (arg_count <= 1) {
		print_type = PRINT_CHATBOX;
		goto skip_getting_print_type;
	}

	print_type = get_print_type_from_str(args[1]);

	if (print_type == PRINT_UNKNOWN) {
		fputs("Error getting print type\n", stderr);
		return -1;
	}

skip_getting_print_type:

	// Add commands name and info.
	for (auto c : command_list) {
		memset(buf, 0, NAME_MAX);
		snprintf(buf, NAME_MAX, "Name: %s, silent %s\n", c.name, c.show_command ? "no" : "yes");
		output_message += buf;
	}

	// Print 'output_message'.
	if (print_to(print_type, output_message.c_str(), chat_box) == -1) {
		fputs("Error printing data\n", stderr);
		return -1;
	}

	return 0;
}

int show_script_locat_command(COM_CB_ARGS) {
	PRINT_TYPE print_type;
	VariableData script_location;
	char output_buf[NAME_MAX];

	// Clear script location global.
	chat_box->add_or_set_global_var({SCRIPT_LOCATION_GLOBAL, "\0"});

	// Get print type.
	if (arg_count <= 1) {
		print_type = PRINT_CHATBOX;
		goto skip_getting_print_type;
	}

	print_type = get_print_type_from_str(args[1]);

	if (print_type == PRINT_UNKNOWN) {
		fputs("Error getting print type\n", stderr);
		return -1;
	}

skip_getting_print_type:

	// Set the output.
	memset(output_buf, 0, NAME_MAX);

	if (chat_box->get_script_location()[0] == '\0')
		strncat(output_buf, "No script location set", NAME_MAX - 1);
	else
		snprintf(output_buf, NAME_MAX, "Script location: %s", chat_box->get_script_location());

	// Print 'output_message'.
	if (print_to(print_type, output_buf, chat_box) == -1) {
		fputs("Error printing data\n", stderr);
		return -1;
	}

	// Set global variable.
	memset(script_location.name, 0, NAME_MAX);
	memset(script_location.value, 0, NAME_MAX);

	strncat(script_location.name, SCRIPT_LOCATION_GLOBAL, NAME_MAX - 1);
	strncat(script_location.value, chat_box->get_script_location(), NAME_MAX - 1);

	chat_box->add_or_set_global_var(script_location);

	return 0;
}

int get_script_locat_command(COM_CB_ARGS) {
	VariableData script_location;

	memset(script_location.name, 0, NAME_MAX);
	memset(script_location.value, 0, NAME_MAX);

	strncat(script_location.name, SCRIPT_LOCATION_GLOBAL, NAME_MAX - 1);
	strncat(script_location.value, chat_box->get_script_location(), NAME_MAX - 1);

	chat_box->add_or_set_global_var(script_location);
	return 0;
}

int change_heath_by_command(COM_CB_ARGS) {
	int heath_gain;
	Player * player = (Player*)mdata->player;

	if (arg_count != 2) {
		fputs("One argument required\n", stderr);
		return -1;
	}

	if (player == NULL) {
		fputs("Player is null\n", stderr);
		return -1;
	}

	// Get heath gain.
	if (gameTools::str_to_int(args[1], NAME_MAX, &heath_gain) == -1) {
		fprintf(stderr, "Error getting int value from %s\n", args[1]);
		return -1;
	}

	player->add_heath(heath_gain);
	return 0;
}

int set_heath_command(COM_CB_ARGS) {
	int heath_value;
	Player * player = (Player*)mdata->player;

	if (arg_count != 2) {
		fputs("One argument required\n", stderr);
		return -1;
	}

	if (player == NULL) {
		fputs("Player is null\n", stderr);
		return -1;
	}

	// Get heath arg.
	if (gameTools::str_to_int(args[1], NAME_MAX, &heath_value) == -1) {
		fprintf(stderr, "Error getting int value from %s\n", args[1]);
		return -1;
	}

	player->set_heath(heath_value);
	return 0;
}

int set_coin_count_command(COM_CB_ARGS) {
	int coin_count;
	Player * player = (Player*)mdata->player;

	if (arg_count != 2) {
		fputs("One argument required\n", stderr);
		return -1;
	}

	if (player == NULL) {
		fputs("Player is null\n", stderr);
		return -1;
	}

	// Get coin arg.
	if (gameTools::str_to_int(args[1], NAME_MAX, &coin_count) == -1) {
		fprintf(stderr, "Error getting int value from %s\n", args[1]);
		return -1;
	}

	player->set_coins(coin_count);
	return 0;
}

int change_coin_count_by_command(COM_CB_ARGS) {
	int coin_gain;
	Player * player = (Player*)mdata->player;

	if (arg_count != 2) {
		fputs("One argument required\n", stderr);
		return -1;
	}

	if (player == NULL) {
		fputs("Player is null\n", stderr);
		return -1;
	}

	// Get coin gain.
	if (gameTools::str_to_int(args[1], NAME_MAX, &coin_gain) == -1) {
		fprintf(stderr, "Error getting int value from %s\n", args[1]);
		return -1;
	}

	player->add_coins(coin_gain);
	return 0;
}

void add_main_commands(MainData * mdata, ChatBox * chat_box) {
	ChatCommand command_list[] = {
		{"say", say_command, NULL, false},
		{"exit", exit_command, NULL},
		{"load", load_command, NULL},
		{"silent_load", load_command, NULL, false},
		{"do_nothing", do_nothing_command, NULL, false},
		{"popup", popup_command, NULL, false},
		{"print", print_command, NULL, false},
		{"run_sequence", run_sequence_command, NULL, false},
		{"set", set_command, NULL},
		{"silent_set", set_command, NULL, false},
		{"show_globals", show_globals_command, NULL},
		{"if", if_command, NULL, false},
		{"list_commands", list_commands_command, NULL},
		{"show_script_locat", show_script_locat_command, NULL},
		{"get_script_locat", get_script_locat_command, NULL, false}
	};

	chat_box->add_command_list(command_list, sizeof(command_list) / sizeof(ChatCommand));
}

void add_game_only_commands(MainData * mdata, ChatBox * chat_box)  {
	ChatCommand command_list[] = {
		{"change_heath_by", change_heath_by_command, NULL},
		{"silent_change_heath_by", change_heath_by_command, NULL, false},

		{"set_heath", set_heath_command, NULL},
		{"silent_set_heath", set_heath_command, NULL, false},

		{"change_coin_count_by", change_coin_count_by_command, NULL},
		{"silent_change_coin_count_by", change_coin_count_by_command, NULL, false},

		{"set_coin_count", set_coin_count_command, NULL},
		{"silent_set_coint_count", set_coin_count_command, NULL, false}
	};

	chat_box->add_command_list(command_list, sizeof(command_list) / sizeof(ChatCommand));
}
