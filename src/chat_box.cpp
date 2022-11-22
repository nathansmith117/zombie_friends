#include "chat_box.h"
#include "game_commands.h"

void ChatBox::main_init(MainData * md, int X, int Y, int W, int H) {
	mdata = md;

	memset(script_location, 0, NAME_MAX);

	// Init widgets.
	
	// Text display and buffer.
	text_display = new Fl_Text_Display(0, 0, 0, 0);

	text_buffer = new Fl_Text_Buffer();
	text_display->buffer(text_buffer);

	// Input on button.
	int iob_width = mdata->settings.chat_box_input_height * 2;

	input_on_button = new Fl_Check_Button(0, 0, 0, 0);

	input_on_button->label("On");
	input_on_button->value(0);

	// Input line.
	input_line = new Fl_Input(0, 0, 0, 0);

	input_line->textsize(mdata->settings.chat_box_input_height - 8);
	input_line->callback(input_line_cb, (void*)this);
	input_line->when(FL_WHEN_ENTER_KEY);
	input_line->deactivate();

	// Set callback when input line is not null.
	input_on_button->callback(input_on_button_cb, (void*)input_line);

	reset_size();

	// Add main directory global.
	VariableData main_dir_global;

	memset(main_dir_global.name, 0, NAME_MAX);
	memset(main_dir_global.value, 0, NAME_MAX);

	strncat(main_dir_global.name, "MAIN_DIR", NAME_MAX - 1);
	strncat(main_dir_global.value, mdata->MAIN_DIR, NAME_MAX - 1);

	add_or_set_global_var(main_dir_global);

	// Add script location.
	add_or_set_global_var({SCRIPT_LOCATION_GLOBAL, "\0"});

	end();
}

void ChatBox::reset_size() {
	// Input on button.
	int iob_width = mdata->settings.chat_box_input_height * 2;
	int input_y = h() - mdata->settings.chat_box_input_height;

	// If at top.
	if (mdata->settings.tab_menu_locat == MENU_TOP)
		input_y -= mdata->top_menu->h();

	// Text display.
	text_display->resize(
		x(),
		y(),
		w(),
		input_y
	);

	input_y = y() + text_display->h();

	// Input one button.
	input_on_button->resize(
		x() + (w() - iob_width),
		input_y,
		iob_width,
		mdata->settings.chat_box_input_height
	);

	// Input line.
	input_line->resize(
		x(),
		input_y,
		w() - iob_width,
		mdata->settings.chat_box_input_height
	);
}

int ChatBox::handle(int event) {
	Fl_Group::handle(event);
	return 0;
}

void ChatBox::printline(const char * msg) {
	text_buffer->append(msg);
	text_buffer->append("\n");
	scroll_to_bottom();
}

void ChatBox::post(const char * name, const char * msg, size_t n) {
	size_t buf_size = n + 5; // Add more bytes for formatting.
	char buf[buf_size];

	memset(buf, 0, buf_size);
	snprintf(buf, buf_size, "%s: %s\n", name, msg);

	text_buffer->append(buf);
	scroll_to_bottom();
}

bool ChatBox::is_command(const char * msg) {
	return msg[0] == '/';
}

void ChatBox::add_command_list(std::vector<ChatCommand> new_command_list) {
	for (auto c : new_command_list)
		command_list.push_back(c);
}

int ChatBox::add_command_list(const ChatCommand * new_command_list, size_t n) {
	long int i;

	if (new_command_list == NULL || n <= 0)
		return -1;
	
	for (i = 0; i < n; i++)
		command_list.push_back(new_command_list[i]);
	
	return 0;
}

int ChatBox::run_command(const char * comm, std::vector<VariableData> * var_list) {
	int i;
	int res_value = 0; // Return value;
	
	VariableData curr_variable;
	char var_value[NAME_MAX];

	bool variable_found;

	size_t comm_buf_size = NAME_MAX * 2;
	char comm_buf[comm_buf_size];
	memset(comm_buf, 0, comm_buf_size);

	int arg_count = 1; // The command name is a argument so there is also at
					   // least one argument.
	char ** args = NULL;

	int curr_arg = 0;
	bool still_looking = true;

	if (comm == NULL) {
		fputs("Command is null\n", stderr);
		return -1;
	}

	// Remove slash if there.
	if (comm[0] == '/')
		for (i = 0; i < NAME_MAX - 1; i++)
			comm_buf[i] = comm[i + 1];
	else
		strncat(comm_buf, comm, comm_buf_size);

	// Get arg count.
	arg_count = get_arg_count(comm_buf, comm_buf_size);

	if (arg_count == -1)
		return -1;
	
	// Get memory for args.
	args = gameTools::create_2d_array<char>(NAME_MAX, arg_count);

	if (args == NULL) {
		fputs("Args are null\n", stderr);
		return -1;
	}

	// Get arguments.
	i = 0;

	for (auto c : comm_buf) {

		if (i >= NAME_MAX || curr_arg >= arg_count) { // Check sizes.
			fprintf(stderr, "'i' or 'arg_count' in %s:%d got to big\n", __FILE__, __LINE__);
			res_value = -1;
			goto clear_mem;
		} if (c == '\0' || c == '#') { // End of buffer or comment.
			args[curr_arg][i] = '\0';
			break;
		} if (c == ' ') { // Next argument.
			args[curr_arg][i] = '\0';
			curr_arg++;
			i = 0;
			continue;
		}

		args[curr_arg][i] = c;
		i++;
	}

	// Replace variables with there value.
	for (i = 0; i < arg_count; i++) {
		if (args[i][0] != '$')
			continue;

		variable_found = false;

		// Get variable from list.
		if (var_list != NULL)
			if (get_value_from_list(args[i], *var_list, var_value))
				variable_found = true;

		// Search globals if not found.
		if (!variable_found)
			if (get_value_from_list(args[i], global_vars, var_value))
					variable_found = true;

		// Variable not found.
		if (!variable_found) {
			fprintf(stderr, "Variable '%s' not found\n", args[i]);
			res_value = -1;
			goto clear_mem;
		}

		// Replace.
		memset(args[i], 0, NAME_MAX);
		strncat(args[i], var_value, NAME_MAX);
	}

	// Find command in list then run.
	for (auto c : command_list)
		if (strncmp(args[0], c.name, NAME_MAX) == 0) {

			// Output command name and args.
			if (c.show_command)
				printline(comm);

			// Run command and check for errors.
			if (c.callback(arg_count, (const char**)args, mdata, this, var_list, c.data) == -1) {
				res_value = -1;
				goto clear_mem;
			}

			still_looking = false;
			break;
		}

	// Command not found.
	if (still_looking) {
		fputs("Command not found\n", stderr);
		res_value = -1;
		goto clear_mem;
	}

	// Clear input line.
	input_line->value("");

	// Clear memory.
clear_mem:
	int res = gameTools::delete_2d_array<char>(args, NAME_MAX, arg_count);

	if (res == -1) {
		fputs("Error deleting args\n", stderr);
		res_value = -1;
	}

	return res_value;
}

int ChatBox::get_arg_count(const char * comm_buf, size_t n) {
	int i;
	int arg_count = 1;// The command name is a argument so there is also at
					  // least one argument.
	
	// Check first character.
	if (comm_buf[0] == ' ' || comm_buf[0] == '\t') {
		fputs("No white space at start of command\n", stderr);
		return -1;
	}

	for (i = 0; i < n - 1; i++) {
		if (comm_buf[i] == '\0') // End of buffer.
			break;
		if (comm_buf[i] == ' ' && comm_buf[i + 1] == ' ') { // 2 spaces.
			fputs("You cant have two or more spcaes in a row\n", stderr);
			return -1;
		} if (comm_buf[i] == '\t') { // tabs.
			fputs("No tabs\n", stderr);
			return -1;
		}

		// Add to arg_count.
		if (comm_buf[i] != ' ')
			continue;

		// If at end of command.
		if (comm_buf[i + 1] == '\0') {
			fputs("No white space at end of command\n", stderr);
			return -1;
		}
			
		arg_count++;
	}

	return arg_count;
}

int ChatBox::run_script(const char * file_path) {
	long int i;
	int res_value = 0; // Return value.

	FILE * fp = NULL;
	size_t file_size;
	char * file_buf = NULL;
	long int new_line_count;

	char ** comm_list = NULL;
	long int comm_list_x = 0;
	long int comm_list_y = 0;

	VariableData curr_variable;
	std::vector<VariableData> script_variables;

	if (file_path == NULL) {
		fputs("File path is null\n", stderr);
		return -1;
	}

	// Open file.
	fp = fopen(file_path, "r");

	if (fp == NULL) {
		fprintf(stderr, "%s not found\n", file_path);
		return -1;
	}

	// Get file size.
	file_size = gameTools::get_file_size(fp);

	// Get script location.
	memset(script_location, 0, NAME_MAX);

	if (gameTools::get_file_location(file_path, script_location) == -1) {
		fclose(fp);
		fprintf(stderr, "Error getting the location of %s\n", file_path);
		return -1;
	}

	if (file_size <= 0) {
		fputs("File is empty\n", stderr);
		fclose(fp);
		return -1;
	}

	// Get memory for buffer.
	file_buf = new char[file_size];

	if (file_buf == NULL) {
		fputs("Error getting memory for buffer\n", stderr);
		fclose(fp);
		return -1;
	}

	// Read file.
	if (fread(file_buf, file_size, 1, fp) == -1) {
		fprintf(stderr, "Error reading %s\n", file_path);
		res_value = -1;
		goto clear_mem;
	}

	// Get new line count.
	new_line_count = gameTools::get_char_count(file_buf, '\n', file_size);

	// Get memory for command list.
	comm_list = gameTools::create_2d_array<char>(NAME_MAX, new_line_count);

	if (comm_list == NULL) {
		fputs("Error getting memory for comm_list\n", stderr);
		res_value = -1;
		goto clear_mem;
	}

	// Put commands in comm_list.
	for (i = 0; i < file_size; i++) {
		if (file_buf[i] == '\0') // Check for zero byte.
			break;
		// Check size.
		if (comm_list_x >= NAME_MAX || comm_list_y >= new_line_count) {
			fprintf(stderr, "x or y value in %s:%d got to big\n", __FILE__, __LINE__);
			res_value = -1;
			goto clear_mem;
		}

		if (file_buf[i] == '\n') { // At new line.

			// Add zero byte and/or new line.
			if (comm_list_x == 0) {
				comm_list[comm_list_y][comm_list_x] = '\n';
				comm_list[comm_list_y][comm_list_x + 1] = '\0';
			} else
				comm_list[comm_list_y][comm_list_x] = '\0';

			comm_list_x = 0;
			comm_list_y++;
			continue;
		}

		// Add characters to comm_list.
		comm_list[comm_list_y][comm_list_x] = file_buf[i];
		comm_list_x++;

	}

	// Run commands in list.
	for (i = 0 /*zero like your iq*/; i < new_line_count; i++) { // A sus loop.
		if (comm_list[i][0] == '\n' || /* || means 'or' you supid python programmer */ comm_list[i][0] == '#') // Check for
																 // new lines
																 // and
																 // commets.
			continue;
		if (comm_list[i][0] == '$') { // Variables.
			if (variable_str_to_data(comm_list[i], NAME_MAX, &curr_variable) == -1) {
				fprintf(stderr, "Error Getting variable data '%s'\n", comm_list[i]);
				res_value = -1;
				goto clear_mem;
			}

			// Variable is in globals.
			// Warn the user but dont stop running the script.
			if (is_in_globals(curr_variable)) {
				fprintf(stderr, "'%s' is in globals\n", curr_variable.name);
				continue;
			}

			// Add or set variable.
			if (add_or_set_variable(curr_variable, &script_variables) == -1) {
				fprintf(stderr, "Error adding '%s' to variable list\n", curr_variable.name);
				res_value = -1;
				goto clear_mem;
			}

			continue;
		}

		if (run_command(comm_list[i], &script_variables) == -1) { // Run command and check for errors.
			res_value = -1; // Set it to -1 because I didnt make a variable for
							// it )----:
			goto clear_mem; // Allways use goto statements in c++ because they
							// are good.
		}
	} // The madness is over for now hahaha?!

clear_mem:
	fclose(fp);
	delete [] file_buf;

	// Clear comm_list and check for errors.
	if (comm_list != NULL)
		if (gameTools::delete_2d_array<char>(comm_list, NAME_MAX, new_line_count) == -1) // Set return value if error.
			res_value = -1;

	return res_value;
}

void ChatBox::set_script_location(const char * script_location) {
	memset(this->script_location, 0, NAME_MAX);

	if (script_location == NULL)
		return;

	strncat(this->script_location, script_location, NAME_MAX - 1);
}

void ChatBox::scroll_to_bottom() {
	text_display->scroll(text_buffer->count_lines(0, text_buffer->length()), 0);
}

void ChatBox::input_on_button_cb(Fl_Widget * w, void * d) {
	const Fl_Check_Button * button = (const Fl_Check_Button*)w;
	Fl_Input * input_line = (Fl_Input*)d;

	int button_value = button->value();

	if (button_value)
		input_line->activate();
	else
		input_line->deactivate();
}

void ChatBox::input_line_cb(Fl_Widget * w, void * d) {
	ChatBox * chat_box = (ChatBox*)d;

	const char name[] = "You";

	const char * input_value = chat_box->input_line->value();
	size_t input_size = chat_box->input_line->size();

	// Check for errors.
	if (input_size <= 0)
		return;
	if (input_size >= NAME_MAX) {
		fprintf(stderr, "The input size is %d, the max is %d\n", input_size, NAME_MAX);
		return;
	}

	// Run command if command.
	if (chat_box->is_command(input_value)) {
		chat_box->run_command(input_value);
		return;
	}

	// Print input value then clear value.
	chat_box->post(name, input_value, sizeof(name) + input_size);
	chat_box->input_line->value("");
}

int ChatBox::variable_str_to_data(const char * variable_str, size_t n, VariableData * variable_data) {
	int i, j;
	int start_at;

	bool got_name = false;
	bool got_value = false;

	if (variable_str == NULL || variable_data == NULL)
		return -1;

	// Clear 'variable_data'
	memset(variable_data->name, 0, NAME_MAX);
	memset(variable_data->value, 0, NAME_MAX);

	// Skip '$' at start.
	start_at = (variable_str[0] == '$') ? 1 : 0;

	j = 0;

	// Get name and value.
	for (i = start_at; i < n; i++) {
		if (variable_str[i] == '\0' || variable_str[i] == '\n') { // End.
			if (got_name && j > 0) // Did get value.
				got_value = true;

			break;
		}

		// j to big.
		if (j >= NAME_MAX)
			return -1;

		// Get name.
		if (!got_name) {
			if (variable_str[i] == ' ') { // Gottent name.
				got_name = true;
				j = 0;
				continue;
			}

			variable_data->name[j] = variable_str[i];
			j++;
		} else if (!got_value) { // Get name.
			variable_data->value[j] = variable_str[i];
			j++;
		}
	}

	if (!got_name || !got_value)
		return -1;

	return 0;
}

int ChatBox::add_or_set_variable(VariableData variable_data, std::vector<VariableData> * var_list) {
	int i;

	if (var_list == NULL)
		return -1;

	// Already in list.
	for (i = 0; i < var_list->size(); i++)
		if (strncmp(variable_data.name, var_list->at(i).name, NAME_MAX) == 0) {
			var_list->at(i) = variable_data;
			return 0;
		}

	var_list->push_back(variable_data);
	return 0;
}

bool ChatBox::get_value_from_list(const char * variable_name, std::vector<VariableData> var_list, char * variable_value) {
	int i;
	char var_name[NAME_MAX];

	if (variable_name == NULL || variable_value == NULL)
		return false;

	memset(variable_value, 0, NAME_MAX);
	memset(var_name, 0, NAME_MAX);

	// Remove '$' from 'variable_value' if there.
	if (variable_name[0] == '$')
		for (i = 1; i < NAME_MAX; i++)
			var_name[i - 1] = variable_name[i];
	else
		strncat(var_name, variable_name, NAME_MAX - 1);

	// Look for name.
	for (auto v : var_list)
		if (strncmp(var_name, v.name, NAME_MAX) == 0) {
			strncat(variable_value, v.value, NAME_MAX);
			return true;
		}

	return false;
}

bool ChatBox::is_in_globals(VariableData var_data) {
	for (auto v : global_vars)
		if (strncmp(var_data.name, v.name, NAME_MAX) == 0)
			return true;

	return false;
}

int ChatBox::add_or_set_global_var(VariableData var_data) {
	return add_or_set_variable(var_data, &global_vars);
}
