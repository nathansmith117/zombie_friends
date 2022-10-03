#include "edit_menu.h"
#include "map.h"
#include "game_tools.h"
#include "view_window.h"
#include "editWindow.h"

// Top menu for game.
#include "top_menu.h"

void delete_editor_diologs(EditMenuDiologs * menu_diologs) {
	delete menu_diologs->new_diolog;
	delete menu_diologs;
}

void editor_new_cb(Fl_Widget * w, void * d) {
	NewDiolog * new_diolog = (NewDiolog*)d;
	MainData * mdata = new_diolog->mdata;
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;

	new_diolog->diolog_win->position(Fl::event_x_root(), Fl::event_y_root());
	new_diolog->diolog_win->show();
}

void editor_create_cb(Fl_Widget * w, void * d) {
	NewDiolog * new_diolog = (NewDiolog*)d;
	MainData * mdata = new_diolog->mdata;

	int width, height;

	// Check if stuff is null.
	if (new_diolog == NULL) {
		fputs("Something is null\n", stderr);
		return;
	} if (mdata == NULL 
		|| new_diolog->width_input == NULL 
		|| new_diolog->height_input == NULL
		|| new_diolog->diolog_win == NULL) {
		fputs("Something is null\n", stderr);
		return;
	}

	// Get width and height.
	width = gameTools::valuestr_to_int(new_diolog->width_input);
	height = gameTools::valuestr_to_int(new_diolog->height_input);

	if (width <= 0 || height <= 0) {
		fputs("Size can't be less than 1\n", stderr);
		return;
	}

	mdata->map->create_map(width, height);
	new_diolog->diolog_win->hide();
}

void editor_save_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;

	int res;
	char file_path[NAME_MAX];

	if (mdata->map == NULL) {
		fputs("Can't write to map because it is null\n", stderr);
		return;
	}

	// Get file path.
	res = mdata->map->get_file_path(file_path, NAME_MAX);

	if (res == -1) {
		fputs("Error getting file path\n", stderr);
		return;
	}

	// Check if map file path is set.
	if (file_path[0] == '\0') {
		editor_save_as(menu_bar, mdata);
		return;
	}

	mdata->map->dump(file_path, NAME_MAX);
}

void editor_save_as_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;

	editor_save_as(menu_bar, mdata);
}

int editor_save_as(Fl_Menu_Bar * menu_bar, MainData * mdata) {
	int res = 0;

	if (mdata->map == NULL) {
		fputs("You have to create or open a map first\n", stderr);
		return -1;
	}

	const char * file_name = NULL;

	Fl_Native_File_Chooser * file_browser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	file_browser->title("Zombie Friends Save As");
	file_browser->directory(mdata->MAIN_DIR);

	// Get file.
	switch (file_browser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
			res = -1;
			goto clear_mem;
		case 1: // cancel
			goto clear_mem;
		default:
			file_name = file_browser->filename();
			break;
	}

	if (file_name == NULL) {
		res = -1;
		goto clear_mem;
	}

	mdata->map->dump(file_name, NAME_MAX);

clear_mem:
	delete file_browser;

	return res;
}

void create_diologs(MainData * mdata) {
	if (mdata == NULL)
		return;

	mdata->edit_menu_diologs = new EditMenuDiologs;
	mdata->edit_menu_diologs->new_diolog = new NewDiolog;
	EditMenuDiologs * menu_diologs = (EditMenuDiologs*)mdata->edit_menu_diologs;

	// New diolog.
	Fl_Menu_Window * new_diolog_win = new Fl_Menu_Window(
		mdata->settings.diolog_width, 
		mdata->settings.diolog_height
	);

	// Width.
	Fl_Input * width_input = new Fl_Input(
		new_diolog_win->w() / 4, 
		mdata->settings.input_height / 2, 
		new_diolog_win->w() / 2,
		mdata->settings.input_height,
		"Width"
	);

	width_input->type(FL_INT_INPUT);

	// Height.
	Fl_Input * height_input = new Fl_Input(
		new_diolog_win->w() / 4, 
		width_input->y() + width_input->h(), 
		new_diolog_win->w() / 2,
		mdata->settings.input_height,
		"Height"
	);

	height_input->type(FL_INT_INPUT);

	// Enter button.
	Fl_Button * enter_button = new Fl_Button(
		new_diolog_win->w() / 4,
		(int)height_input->y() + (height_input->h() * 1.5),
		new_diolog_win->w() / 4,
		mdata->settings.input_height * 2,
		"Create"
	);

	// Add args.
	menu_diologs->new_diolog->mdata = mdata;
	menu_diologs->new_diolog->width_input = width_input;
	menu_diologs->new_diolog->height_input = height_input;
	menu_diologs->new_diolog->diolog_win = new_diolog_win;

	enter_button->callback(editor_create_cb, (void*)menu_diologs->new_diolog);

	new_diolog_win->end();
}

void add_editor_menu_items(MainData * mdata, Fl_Menu_Bar * menu) {
	int menu_flag;

	if (mdata == NULL)
		return;

	create_diologs(mdata);

	// File.
	menu->add("&File/Open", FL_CTRL + 'o', (Fl_Callback*)open_cb, (void*)mdata);
	menu->add("&File/New", FL_CTRL + 'n', (Fl_Callback*)editor_new_cb, (void*)mdata->edit_menu_diologs->new_diolog);
	menu->add("&File/Save", FL_CTRL + 's', (Fl_Callback*)editor_save_cb, (void*)mdata);
	menu->add("&File/Save As", FL_CTRL + FL_SHIFT + 's', (Fl_Callback*)editor_save_as_cb, (void*)mdata, FL_MENU_DIVIDER);
	menu->add("&File/Settings", 0, (Fl_Callback*)settings_cb, (void*)mdata);
	menu->add("&File/Exit", FL_ALT + FL_F + 4, (Fl_Callback*)exit_cb, (void*)mdata);

	// View.
	menu->add("&View/Fullscreen", FL_F + 11, (Fl_Callback*)fullscreen_cb, (void*)mdata, FL_MENU_DIVIDER | FL_MENU_TOGGLE);
	menu->add("&View/Zoom in", FL_CTRL + '+', 0, (void*)mdata);
	menu->add("&View/Zoom out", FL_CTRL + '-', 0, (void*)mdata, FL_MENU_DIVIDER);

	// Set flags for tab menu item.
	menu_flag = FL_MENU_TOGGLE;

	if (mdata->tab_menu == NULL)
		menu_flag |= FL_MENU_INACTIVE;
	if (!mdata->settings.tab_menu_default_hidden || !((ViewWindow*)mdata->view_win)->is_inlayed())
		menu_flag |= FL_MENU_VALUE;

	// Tab menu.
	menu->add("&View/Tab menu", FL_CTRL + '`', (Fl_Callback*)tab_menu_cb, (void*)mdata, menu_flag);
}
