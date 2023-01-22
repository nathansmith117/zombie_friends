#include "launcher_option_editor.h"
#include "game_tools.h"

void LauncherOptionEditor::main_init(MainData * md, int X, int Y, int W, int H) {
	mdata = md;

	char instance_chooser_dir[NAME_MAX];
	memset(instance_chooser_dir, 0, NAME_MAX);

	// Default launcher.
	memset(default_launcher, 0, NAME_MAX);
	snprintf(default_launcher, NAME_MAX, "%scmd_launcher", mdata->MAIN_DIR);

	// Instance chooser.
	instance_chooser = new Fl_File_Browser(0, 0, 0, 0, "Instances");
	instance_chooser->type(FL_HOLD_BROWSER);
	instance_chooser->callback(instance_chooser_cb, (void*)this);

	snprintf(instance_chooser_dir, NAME_MAX, "%sinstances%s", mdata->MAIN_DIR, PSLASH);
	instance_chooser->load(instance_chooser_dir);

	if (instance_chooser->text(1))
		if (!strncmp(instance_chooser->text(1), "../", sizeof("../")))
			instance_chooser->remove(1);

	// Instance chooser directory input.
	instance_chooser_dir_input = new Fl_Input(0, 0, 0, 0, "Instance folder");
	instance_chooser_dir_input->value(instance_chooser_dir);

	// Instance chooser directory browser.
	instance_chooser_dir_browser = new Fl_Button(0, 0, 0, 0);
	instance_chooser_dir_browser->label("@fileopen");
	instance_chooser_dir_browser->callback(directory_browser_cb, (void*)instance_chooser_dir_input);

	// Refresh option chooser.
	refresh_instance_chooser = new Fl_Button(0, 0, 0, 0);
	refresh_instance_chooser->label("@refresh");
	refresh_instance_chooser->callback(refresh_instance_chooser_cb, (void*)this);

	// Launcher dll input.
	launcher_dll_input = new Fl_Input(0, 0, 0, 0, "Launcher dll");
	launcher_dll_input->value(mdata->MAIN_DIR);

	// Launcher dll button.
	launcher_dll_browser = new Fl_Button(0, 0, 0, 0);
	launcher_dll_browser->label("@fileopen");
	launcher_dll_browser->callback(file_browser_cb, (void*)launcher_dll_input);

	// Main directory input.
	main_dir_input = new Fl_Input(0, 0, 0, 0, "Main directory");
	main_dir_input->value(mdata->MAIN_DIR);

	// Main directory button.
	main_dir_browser = new Fl_Button(0, 0, 0, 0);
	main_dir_browser->label("@fileopen");
	main_dir_browser->callback(directory_browser_cb, (void*)main_dir_input);

	// Launcher location input.
	launcher_location_input = new Fl_Input(0, 0, 0, 0, "Launcher");
	launcher_location_input->value(mdata->MAIN_DIR);

	// Launcher location button.
	launcher_location_browser = new Fl_Button(0, 0, 0, 0);
	launcher_location_browser->label("@fileopen");
	launcher_location_browser->callback(file_browser_cb, (void*)launcher_location_input);

	// Startup script input.
	startup_script_input = new Fl_Input(0, 0, 0, 0, "Startup script");
	startup_script_input->value(mdata->MAIN_DIR);

	// Startup script button.
	startup_script_browser = new Fl_Button(0, 0, 0, 0);
	startup_script_browser->label("@fileopen");
	startup_script_browser->callback(file_browser_cb, (void*)startup_script_input);

	// Dump button.
	dump_button = new Fl_Button(0, 0, 0, 0);
	dump_button->label("@filesave");
	dump_button->callback(dump_options_cb, (void*)this);

	// Launch button.
	launch_button = new Fl_Button(0, 0, 0, 0);
	launch_button->label("@>");
	launch_button->callback(launch_cb, (void*)this);

	// New options.
	new_options = new Fl_Input(0, 0, 0, 0, "New options");

	// Create new options button.
	create_new_options = new Fl_Button(0, 0, 0, 0);
	create_new_options->label("@filenew");
	create_new_options->callback(new_options_cb, (void*)this);

	reset_size();
	end();

	// Set option inputs.
	LauncherOptionData launcher_option_data[] = {
		LauncherOptionData("dll", {launcher_dll_input}, LAUNCHER_OPTION_INPUT),
		LauncherOptionData("md", {main_dir_input}, LAUNCHER_OPTION_INPUT),
		LauncherOptionData("ll", {launcher_location_input}, LAUNCHER_OPTION_INPUT),
		LauncherOptionData("ss", {startup_script_input}, LAUNCHER_OPTION_INPUT)
	};

	for (auto l : launcher_option_data)
		option_inputs.push_back(l);

	// Set tags.
	FilePathTag tags[] = {
		FilePathTag("$ofd", option_file_dir),
		FilePathTag("$dl", default_launcher)
	};

	for (auto t : tags)
		file_path_tags.push_back(t);
}

void LauncherOptionEditor::reset_size() {
	int ww = w() / 8;
	int wh = mdata->settings.input_height;
	int wx = x() + ww;
	int wy = y();

	int button_w = mdata->settings.input_height * 2;
	int button_h = mdata->settings.input_height;

	// Instance chooser.
	instance_chooser->resize(
		x(),
		wy,
		w(),
		h() / 3
	);

	// Instance chooser directory input.
	instance_chooser_dir_input->resize(
		wx,
		instance_chooser->y() + instance_chooser->h() + wh,
		ww,
		wh
	);

	// Instance chooser directory browser.
	instance_chooser_dir_browser->resize(
		instance_chooser_dir_input->x() + instance_chooser_dir_input->w() + 1,
		instance_chooser_dir_input->y(),
		button_w,
		button_h
	);

	// Refresh option chooser.
	refresh_instance_chooser->resize(
		wx,
		instance_chooser_dir_browser->y() + instance_chooser_dir_browser->h(),
		button_w,
		button_h
	);

	// Launcher dll input.
	launcher_dll_input->resize(
		wx,
		refresh_instance_chooser->y() + refresh_instance_chooser->h() + wh,
		ww,
		wh
	);

	// Launcher dll button.
	launcher_dll_browser->resize(
		launcher_dll_input->x() + launcher_dll_input->w() + 1,
		launcher_dll_input->y(),
		button_w,
		button_h
	);

	// Main directory input.
	main_dir_input->resize(
		wx,
		launcher_dll_input->y() + launcher_dll_input->h(),
		ww,
		wh
	);

	// Main directory button.
	main_dir_browser->resize(
		main_dir_input->x() + main_dir_input->w() + 1,
		main_dir_input->y(),
		button_w,
		button_h
	);

	// Launcher location input.
	launcher_location_input->resize(
		wx,
		main_dir_input->y() + main_dir_input->h(),
		ww,
		wh
	);

	// Launcher location button.
	launcher_location_browser->resize(
		launcher_location_input->x() + launcher_location_input->w() + 1,
		launcher_location_input->y(),
		button_w,
		button_h
	);

	// Startup script input.
	startup_script_input->resize(
		wx,
		launcher_location_input->y() + launcher_location_input->h(),
		ww,
		wh
	);

	// Startup script button.
	startup_script_browser->resize(
		startup_script_input->x() + startup_script_input->w() + 1,
		startup_script_input->y(),
		button_w,
		button_h
	);

	// Dump button.
	dump_button->resize(
		wx,
		startup_script_input->y() + startup_script_input->h(),
		button_w,
		button_h
	);

	// Launch button.
	launch_button->resize(
		wx,
		dump_button->y() + dump_button->h(),
		button_w,
		button_h
	);

	// New options.
	new_options->resize(
		wx,
		launch_button->y() + launch_button->h() + wh,
		ww,
		wh
	);

	// Create new options button.
	create_new_options->resize(
		new_options->x() + new_options->w() + 1,
		new_options->y(),
		button_w,
		button_h
	);
}

// Callbacks.
void LauncherOptionEditor::file_browser_cb(Fl_Widget * w, void * d) {
	Fl_Input * input = (Fl_Input*)d;
	Fl_Native_File_Chooser * file_browser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);

	char dir_buf[NAME_MAX];

	if (input == NULL)
		goto clear_mem;

	// Set directory.
	if (input->value()[0] != '\0')
		file_browser->directory(input->value());

	switch (file_browser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
			goto clear_mem;
		case 1: // cancel
			goto clear_mem;
		default:
			break;
	}

	input->value(file_browser->filename());

clear_mem:
	delete file_browser;
}

void LauncherOptionEditor::directory_browser_cb(Fl_Widget * w, void * d) {
	Fl_Input * input = (Fl_Input*)d;
	Fl_Native_File_Chooser * file_browser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

	size_t dir_buf_size;
	char dir_buf[NAME_MAX];

	if (input == NULL)
		goto clear_mem;

	// Set directory.
	if (input->value()[0] != '\0')
		file_browser->directory(input->value());

	switch (file_browser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
			goto clear_mem;
		case 1: // cancel
			goto clear_mem;
		default:
			break;
	}

	// Save directory path.
	memset(dir_buf, 0, NAME_MAX);
	strncat(dir_buf, file_browser->filename(), NAME_MAX - 1);
	dir_buf_size = strnlen(dir_buf, NAME_MAX);

	// Add slash to end if not there.
	if (dir_buf[dir_buf_size - 1] != PSLASH[0])
		strncat(dir_buf, PSLASH, sizeof(PSLASH));

	// Set input value.
	input->value(dir_buf);

clear_mem:
	delete file_browser;
}

void LauncherOptionEditor::launch_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d; // I didn't have a short name for this )--:
	LaunchOptions launch_options;

	l->get_launch_options(&launch_options);
	
	const char * options[] = {
		"-md",
		launch_options.main_dir,
		"-ss",
		launch_options.startup_script
	};
	
	Launcher::exec_dll(
		launch_options.launcher,
		launch_options.launcher_dll,
		options,
		sizeof(options) / sizeof(char*)
	);
}

int LauncherOptionEditor::dump_options(const char * file_path) {
	int i;
	int res = 0;
	FILE * fp = NULL;
	char line_buf[NAME_MAX];

	if (file_path == NULL) {
		res = -1;
		goto clean_mem;
	}

	// Open file.
	fp = fopen(file_path, "w");

	if (fp == NULL) {
		res = -1;
		goto clean_mem;
	}

	// Write items to file.
	for (auto v : option_inputs) {
		format_option_data(v, line_buf);
		fprintf(fp, "%s\n", line_buf);
	}

	gameTools::get_file_location(file_path, option_file_dir);
	printf("Options written to %s\n", file_path);

clean_mem:
	if (fp != NULL)
		fclose(fp);

	return res;
}

int LauncherOptionEditor::load_options(const char * file_path) {
	int i;
	int res = 0;
	FILE * fp = NULL;

	size_t file_buf_size;
	char * file_buf = NULL;

	int start_at = 0;
	char line_buf[NAME_MAX];

	if (file_path == NULL) {
		res = -1;
		goto clean_mem;
	}

	clear_option_inputs();

	// Open file.
	fp = fopen(file_path, "r");

	if (fp == NULL) {
		res = -1;
		goto clean_mem;
	}

	// Creat buffer.
	file_buf_size = gameTools::get_file_size(fp);
	file_buf = new char[file_buf_size];

	if (file_buf == NULL) {
		res = -1;
		goto clean_mem;
	}

	memset(file_buf, 0, file_buf_size);

	// Read from file.
	if (fread(file_buf, sizeof(char), file_buf_size, fp) == -1) {
		res = -1;
		goto clean_mem;
	}

	// Read each option and write it.
	while (start_at < file_buf_size) {
		start_at = gameTools::read_line_from_str(
			file_buf,
			file_buf_size,
			line_buf,
			NAME_MAX,
			start_at
		);

		// At end.
		if (start_at == -1)
			break;

		// Blank line.
		if (line_buf[0] == '\0')
			continue;

		// Read line.
		if (read_option_line(line_buf) == -1)
			fprintf(stderr, "Error reading '%s' from %s\n", line_buf, file_path);
	}

	gameTools::get_file_location(file_path, option_file_dir);
	printf("Finished load options from %s\n", file_path);

clean_mem:
	if (fp != NULL)
		fclose(fp);
	if (file_buf != NULL)
		delete [] file_buf;
	
	return res;
}

void LauncherOptionEditor::clear_option_inputs() {
	for (auto o : option_inputs)
		clear_option_input(o);
}

bool LauncherOptionEditor::get_option_filepath(char * file_path) {
	bool is_dir;
	size_t filepath_size;

	if (file_path == NULL)
		return false;

	memset(file_path, 0, NAME_MAX);

	// No option file.
	if (!instance_chooser->value())
		return false;

	snprintf(file_path, NAME_MAX, "%s%s", 
		instance_chooser_dir_input->value(),
		instance_chooser->text(instance_chooser->value())
	);

	filepath_size = strnlen(file_path, NAME_MAX);

	// Check if file is a directory.
	is_dir = file_path[filepath_size - 1] == PSLASH[0];

	if (is_dir)
		strncat(file_path, "options.txt", sizeof("options.txt"));

	return true;
}

int LauncherOptionEditor::get_launch_options(LaunchOptions * launch_options) {
	if (launch_options == NULL)
		return -1;

	launch_options->clear();

	// Copy values.
	strncat(launch_options->launcher_dll, launcher_dll_input->value(), NAME_MAX - 1);
	strncat(launch_options->main_dir, main_dir_input->value(), NAME_MAX - 1);
	strncat(launch_options->launcher, launcher_location_input->value(), NAME_MAX - 1);
	strncat(launch_options->startup_script, startup_script_input->value(), NAME_MAX - 1);

	// Tags.
	handle_tag_in_filepath(launch_options->launcher_dll);
	handle_tag_in_filepath(launch_options->main_dir);
	handle_tag_in_filepath(launch_options->launcher);
	handle_tag_in_filepath(launch_options->startup_script);

	return 0;
}

int LauncherOptionEditor::handle_tag_in_filepath(char * file_path) {
	FilePathTag file_path_tag;
	bool no_tag_found = true;

	if (file_path == NULL)
		return -1;

	// Get tag.
	for (auto t : file_path_tags)
		if (gameTools::str_starts_with(file_path, t.tag, NAME_MAX)) {
			file_path_tag = t;
			no_tag_found = false;
			break;
		}

	// No tag.
	if (no_tag_found)
		return 0;

	return replace_tag_with_value(file_path_tag, file_path);
}

int LauncherOptionEditor::replace_tag_with_value(FilePathTag tag, char * file_path) {
	int i;
	char file_path_cp[NAME_MAX];
	size_t tag_size;

	memset(file_path_cp, 0, NAME_MAX);

	if (file_path == NULL)
		return -1;

	// Doesn't start with the tag.
	if (!gameTools::str_starts_with(file_path, tag.tag, NAME_MAX))
		return -1;

	tag_size = strnlen(tag.tag, NAME_MAX);

	// file_path_cp will have no tag.
	for (i = tag_size; i < NAME_MAX; i++)
		file_path_cp[i - tag_size] = file_path[i];

	// Write to file path.
	memset(file_path, 0, NAME_MAX);
	snprintf(file_path, NAME_MAX, "%s%s", tag.value, file_path_cp);
	return 0;
}

void LauncherOptionEditor::clear_option_input(LauncherOptionData data) {
	switch (data.input_type) {
		case LAUNCHER_OPTION_INPUT:
			data.input.input->value("");
			break;
		case LAUNCHER_OPTION_CHOICE:
			data.input.choice->value(0);
			break;
		case LAUNCHER_OPTION_CHECK:
			data.input.check->value(0);
			break;
		default:
			break;
	}
}

int LauncherOptionEditor::format_option_data(LauncherOptionData option_data, char * output_buf) {
	char value_buf[NAME_MAX];

	if (output_buf == NULL)
		return -1;

	memset(value_buf, 0, NAME_MAX);

	switch (option_data.input_type) {
		case LAUNCHER_OPTION_INPUT:
			snprintf(value_buf, NAME_MAX, "%s", option_data.input.input->value());
			break;
		case LAUNCHER_OPTION_CHOICE:
			snprintf(value_buf, NAME_MAX, "%d", option_data.input.choice->value());
			break;
		case LAUNCHER_OPTION_CHECK:
			snprintf(value_buf, NAME_MAX, "%d", option_data.input.check->value());
			break;
		default:
			break;
	}

	// Write to 'output_buf'.
	memset(output_buf, 0, NAME_MAX);
	snprintf(output_buf, NAME_MAX, "%s %s", option_data.option, value_buf);
	return 0;
}

int LauncherOptionEditor::read_option_line(const char * line) {
	int i, j;

	char option_buf[NAME_MAX];
	char value_buf[NAME_MAX];

	bool getting_option = true;

	if (line == NULL)
		return -1;

	memset(option_buf, 0, NAME_MAX);
	memset(value_buf, 0, NAME_MAX);

	j = 0;

	// Get option and value.
	for (i = 0; i < NAME_MAX; i++) {
		if (line[i] == '\0')
			break;

		// End of option.
		if (getting_option && line[i] == ' ') {
			j = 0;
			getting_option = false;
			continue;
		}

		// Get option.
		if (getting_option) {
			option_buf[j] = line[i];
			j++;
			continue;
		}

		// Get value.
		value_buf[j] = line[i];
		j++;
	}

	// Finsihed to get option.
	if (option_buf[0] == '\0')
		return -1;

	return handle_option_and_value(option_buf, value_buf);
}

int LauncherOptionEditor::handle_option_and_value(const char * option_buf, const char * value_buf) {
	LauncherOptionData option_data;
	int value_num;
	bool is_num;

	if (option_buf == NULL || value_buf == NULL)
		return -1;

	// Search for matching option.
	for (auto o : option_inputs)
		if (strncmp(o.option, option_buf, NAME_MAX) == 0) {
			option_data = o;
			break;
		}

	// Nothing found.
	if (option_data.option[0] == '\0')
		return -1;

	// Get value number.
	is_num = (gameTools::str_to_int(value_buf, NAME_MAX, &value_num) == -1) ? false : true;

	switch (option_data.input_type) {
		case LAUNCHER_OPTION_INPUT:
			option_data.input.input->value(value_buf);
			break;
		case LAUNCHER_OPTION_CHOICE:
			if (is_num)
				option_data.input.choice->value(value_num);

			break;
		case LAUNCHER_OPTION_CHECK:
			if (is_num)
				option_data.input.check->value(value_num);

			break;
		default:
			break;
	}

	return 0;
}

void LauncherOptionEditor::refresh_options() {
	instance_chooser->load(instance_chooser_dir_input->value());

	if (instance_chooser->text(1))
		if (!strncmp(instance_chooser->text(1), "../", sizeof("../")))
			instance_chooser->remove(1);
}

void LauncherOptionEditor::instance_chooser_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;

	char option_file[NAME_MAX];

	if (!l->get_option_filepath(option_file))
		return;

	// Load.
	l->load_options(option_file);
}

void LauncherOptionEditor::dump_options_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;

	char option_file[NAME_MAX];

	if (!l->get_option_filepath(option_file))
		return;

	// Dump.
	l->dump_options(option_file);
}

void LauncherOptionEditor::refresh_instance_chooser_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;
	l->refresh_options();
}

void LauncherOptionEditor::new_options_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;

	// No name.
	if (l->new_options->value()[0] == '\0')
		return;

	// Set option file.
	char option_file[NAME_MAX];
	memset(option_file, 0, NAME_MAX);

	snprintf(option_file, NAME_MAX, "%s%s", 
		l->instance_chooser_dir_input->value(),
		l->new_options->value()
	);

	// Unselect option file.
	l->instance_chooser->value(0);

	// Dump.
	l->dump_options(option_file);

	// Refresh.
	l->refresh_options();
}
