#include "launcher_top_menu.h"
#include "launcher.h"

void launcher_open_cb(Fl_Widget * w, void * d) {
	int i;
	MainData * mdata = (MainData*)d;
	char command_path[NAME_MAX];

	// File browser stuff, take a look here https://www.fltk.org/doc-1.3/classFl__Native__File__Chooser.html
	Fl_Native_File_Chooser * file_browser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);
	file_browser->title("Zombie Friends file chooser");
	file_browser->directory(mdata->MAIN_DIR);

	switch (file_browser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
			delete file_browser;
			return;
		case 1: // cancel
			delete file_browser;
			return;
		default:
			break;
	}

	// Create command.
	memset(command_path, 0, NAME_MAX);
	snprintf(command_path, NAME_MAX, "%scmd_launcher", mdata->MAIN_DIR);

	const char * argv[] = {
		command_path,
		file_browser->filename(),
		NULL
	};

	size_t argv_size = sizeof(argv) / sizeof(char*);

	// Print command with c array style format.
	printf("Running command: {");

	for (i = 0; i < argv_size; i++) {
		if (argv[i] == NULL)
			continue;
		else if (i == argv_size - 2) // At last item minus the null one.
			printf("\"%s\"", argv[i]);
		else
			printf("\"%s\", ", argv[i]);
	}

	puts("}");

	// Run command.
	execv(command_path, (char*const*)argv);

	// Error.
	/*
	char error_buf[NAME_MAX];
	memset(error_buf, 0, NAME_MAX);
	strerror_r(errno, error_buf, NAME_MAX);

	fputs(error_buf, stderr);
	*/
}

void launcher_close_game_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;
}

void add_launcher_menu_items(Fl_Menu_Bar * menu, MainData * mdata) {

	// File.
	menu->add("&File/Open", FL_CTRL + 'o', (Fl_Callback*)launcher_open_cb, (void*)mdata);
	menu->add("&File/Save", FL_CTRL + 's', 0, 0);
	menu->add("&File/Save as", FL_CTRL + FL_SHIFT + 's', 0, 0, FL_MENU_DIVIDER);
	menu->add("&File/Close game", 0, (Fl_Callback*)launcher_close_game_cb, (void*)mdata);
	menu->add("&File/Settings", 0, (Fl_Callback*)settings_cb, (void*)mdata);
	menu->add("&File/Exit", FL_ALT + FL_F + 4, (Fl_Callback*)exit_cb, (void*)mdata);

	// View.
	menu->add("&View/Fullscreen", FL_F + 11, (Fl_Callback*)fullscreen_cb, (void*)mdata, FL_MENU_TOGGLE | FL_MENU_DIVIDER);
}
