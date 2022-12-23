#include "launcher_option_editor.h"
#include "game_tools.h"

void LauncherOptionEditor::main_init(MainData * md, int X, int Y, int W, int H) {
	mdata = md;

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

	// Launch button.
	launch_button = new Fl_Button(0, 0, 0, 0);
	launch_button->label("@>");
	launch_button->callback(launch_cb, (void*)this);


	// Debug stuff.
	dump_options_button = new Fl_Button(0, 0, 0, 0);
	dump_options_button->label("@filesave");
	dump_options_button->callback(dump_options_cb, (void*)this);

	load_options_button = new Fl_Button(0, 0, 0, 0);
	load_options_button->label("@fileopen");
	load_options_button->callback(load_options_cb, (void*)this);

	reset_size();
	end();

	// Set option inputs.
	option_inputs.push_back(launcher_dll_input);
	option_inputs.push_back(main_dir_input);
	option_inputs.push_back(launcher_location_input);
	option_inputs.push_back(startup_script_input);
}

void LauncherOptionEditor::reset_size() {
	int ww = w() / 8;
	int wh = mdata->settings.input_height;
	int wx = x() + ww;
	int wy = y();

	int button_w = mdata->settings.input_height * 2;
	int button_h = mdata->settings.input_height;

	// Launcher dll input.
	launcher_dll_input->resize(
		wx,
		wy,
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

	// Launch button.
	launch_button->resize(
		wx,
		startup_script_input->y() + startup_script_input->h(),
		button_w,
		button_h
	);

	// Debug stuff.
	dump_options_button->resize(
		wx,
		launch_button->y() + launch_button->h(),
		button_w,
		button_h
	);

	load_options_button->resize(
		dump_options_button->x() + dump_options_button->w() + 1,
		dump_options_button->y(),
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
	
	const char * options[] = {
		"-md",
		l->main_dir_input->value(),
		"-ss",
		l->startup_script_input->value()
	};
	
	Launcher::exec_dll(
		l->launcher_location_input->value(),
		l->launcher_dll_input->value(),
		options,
		sizeof(options) / sizeof(char*)
	);
}

int LauncherOptionEditor::dump_options(const char * file_path) {
	int i;
	int res = 0;
	FILE * fp = NULL;

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
		if (v == NULL) {
			res = -1;
			goto clean_mem;
		}

		fprintf(fp, "%s\n", v->value());
	}

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
	i = 0;

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

		// To many options.
		if (i >= option_inputs.size())
			break;

		if (option_inputs[i] != NULL)
			option_inputs[i]->value(line_buf);

		i++;
	}

	printf("Finished load options from %s\n", file_path);

clean_mem:
	if (fp != NULL)
		fclose(fp);
	if (file_buf != NULL)
		delete [] file_buf;
	
	return res;
}

void LauncherOptionEditor::dump_options_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;

	if (l->dump_options("/home/nathan/Desktop/zombie_friends/options.txt") == -1)
		fl_alert("Error dumping\n");
}

void LauncherOptionEditor::load_options_cb(Fl_Widget * w, void * d) {
	LauncherOptionEditor * l = (LauncherOptionEditor*)d;

	if (l->load_options("/home/nathan/Desktop/zombie_friends/options.txt") == -1)
		fl_alert("Error loading\n");
}
