#include "top_menu.h"
#include "debug_window.h"
#include "view_window.h"
#include "tab_menu.h"
#include "game_tools.h"
#include "settings_editor.h"

void exit_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;
	mdata->win->hide();
	exit(0);
}

void settings_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;
	SettingsEditor * settings_editor = (SettingsEditor*)mdata->settings_editor;

	if (settings_editor == NULL) {
		fputs("Settings editor is null\n", stderr);
		return;
	}

	settings_editor->show_and_position();
}

void open_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	size_t file_name_size;
	char * file_name = NULL;

	size_t img_folder_size;
	char * img_folder = NULL;

	char file_locat[NAME_MAX];
	char file_ext[NAME_MAX];
	gameTools::GAME_DATA_TYPE data_type;

	
	memset(file_locat, 0, NAME_MAX);
	memset(file_ext, 0, NAME_MAX);

	// File browser stuff, take a look here https://www.fltk.org/doc-1.3/classFl__Native__File__Chooser.html
	Fl_Native_File_Chooser * file_browser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);
	file_browser->title("Zombie Friends file chooser");
	file_browser->directory(mdata->MAIN_DIR);

	// Get file.
	switch (file_browser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
			delete file_browser;
			return;
		case 1: // cancel
			delete file_browser;
			return;
		default:
			file_name_size = strnlen(file_browser->filename(), NAME_MAX);
			file_name = new char[file_name_size];

			if (file_name == NULL) {
				fputs("Error getting memory for 'file_name'\n", stderr);
				return;
			}

			memset(file_name, 0, file_name_size);
			strncat(file_name, file_browser->filename(), file_name_size);
			break;
	}

	// Get file ext.
	if (gameTools::get_file_ext(file_name, file_ext, NAME_MAX) == -1) {
		fputs("Error getting file type\n", stderr);
		goto clear_mem;
	}

	data_type = gameTools::file_ext_to_datatype(file_ext);

	if (data_type == gameTools::GAME_NONE) {
		fputs("File type not supported\n", stderr);
		goto clear_mem;
	// Get load director for images.
	} else if (data_type == gameTools::GAME_IMAGE_LOADER) {
		file_browser->title("Zombie Friends image folder");
		file_browser->directory(mdata->MAIN_DIR);
		file_browser->type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

		// Get file.
		switch (file_browser->show()) {
			case -1: // Error
				fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_browser->errmsg());
				goto clear_mem;
			case 1: // cancel
				break;
			default:
				// Make room for the slash at the end.
				img_folder_size = strnlen(file_browser->filename(), NAME_MAX) + sizeof(PSLASH);
				img_folder = new char[img_folder_size];

				if (img_folder == NULL) {
					fputs("Error getting memory for 'img_folder'\n", stderr);
					goto clear_mem;
				}

				memset(img_folder, 0, img_folder_size);
				snprintf(img_folder, img_folder_size, "%s%s", file_browser->filename(), PSLASH);
				break;
		}
	}

	// Load map.
	if (gameTools::load_file(mdata, file_name, img_folder) == -1)
		fprintf(stderr, "Error loading %s\n", file_name);

clear_mem:
	delete [] file_name;
	delete file_browser;

	if (img_folder != NULL)
		delete [] img_folder;
}

void fullscreen_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	// Get button.
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;
	const Fl_Menu_Item * button = menu_bar->mvalue();

	if (button->value())
		mdata->win->fullscreen();
	else
		mdata->win->fullscreen_off();
}

void view_window_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	// Get button.
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;
	const Fl_Menu_Item * button = menu_bar->mvalue();

	Fl_Menu_Item * tab_menu_button = (Fl_Menu_Item*)menu_bar->find_item("&View/Tab menu");
	TabMenu * tab_menu = (TabMenu*)mdata->tab_menu;

	if (tab_menu == NULL)
		return;

	if (button->value()) {
		mdata->win->remove(mdata->view_win);
		mdata->view_win->hide();
		mdata->view_win->show();
		((ViewWindow*)mdata->view_win)->is_inlayed(false);

		// Set view window to same size as main window.
		mdata->view_win->size(mdata->win->w(), mdata->win->h());

		// Resize tab menu.
		tab_menu->menu_side(MENU_FILL);

		// Tab menu check box.
		if (tab_menu_button != NULL)
			tab_menu_button->deactivate();
	} else {
		mdata->win->add(mdata->view_win);
		mdata->view_win->hide();
		mdata->view_win->show();
		((ViewWindow*)mdata->view_win)->is_inlayed(true);

		// Set tab menu back to size in settings.
		if (mdata->settings.tab_menu_default_hidden)
			tab_menu->menu_side(MENU_HIDDEN);
		else
			tab_menu->menu_side(mdata->settings.tab_menu_locat);

		// Tab menu check box.
		if (tab_menu_button != NULL)
			tab_menu_button->activate();
	}

	// Check tab menu button.
	if (tab_menu->menu_side() == MENU_HIDDEN)
		tab_menu_button->clear();
	else
		tab_menu_button->check();

	mdata->win->redraw();
}

void tab_menu_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	// Get button.
	Fl_Menu_Bar * menu_bar = (Fl_Menu_Bar*)w;
	const Fl_Menu_Item * button = menu_bar->mvalue();

	TabMenu * tab_menu = (TabMenu*)mdata->tab_menu;

	// Set menu side.
	tab_menu->menu_side(button->value() ? mdata->settings.tab_menu_locat : MENU_HIDDEN);
}

void zoom_in_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	if (mdata->settings.scale >= MAX_SCALE)
		return;

	mdata->settings.scale++;
	gameTools::scale_all(mdata);
}

void zoom_out_cb(Fl_Widget * w, void * d) {
	MainData * mdata = (MainData*)d;

	if (mdata->settings.scale <= 1)
		return;

	mdata->settings.scale--;
	gameTools::scale_all(mdata);
}

void add_menu_items(Fl_Menu_Bar * menu, MainData * mdata) {
	int menu_flag;

	// File.
	menu->add("&File/Open", FL_CTRL + 'o', (Fl_Callback*)open_cb, (void*)mdata);
	menu->add("&File/Save", FL_CTRL + 's', 0, 0);
	menu->add("&File/Save as", FL_CTRL + FL_SHIFT + 's', 0, 0, FL_MENU_DIVIDER);
	menu->add("&File/Settings", 0, (Fl_Callback*)settings_cb, (void*)mdata);
	menu->add("&File/Exit", FL_ALT + FL_F + 4, (Fl_Callback*)exit_cb, (void*)mdata);

	// View.
	menu->add("&View/Fullscreen", FL_F + 11, (Fl_Callback*)fullscreen_cb, (void*)mdata, FL_MENU_TOGGLE | FL_MENU_DIVIDER);
	menu->add("&View/Zoom in", FL_CTRL + '=', (Fl_Callback*)zoom_in_cb, (void*)mdata);
	menu->add("&View/Zoom out", FL_CTRL + '-', (Fl_Callback*)zoom_out_cb, (void*)mdata, FL_MENU_DIVIDER);

	// Make null objects inactive.
	menu->add("&View/View window", 0, (Fl_Callback*)view_window_cb, (void*)mdata, 
			(mdata->view_win == NULL) ? FL_MENU_TOGGLE | FL_MENU_INACTIVE : FL_MENU_TOGGLE);

	// Set flags for tab menu item.
	menu_flag = FL_MENU_TOGGLE;

	if (mdata->tab_menu == NULL)
		menu_flag |= FL_MENU_INACTIVE;
	if (!mdata->settings.tab_menu_default_hidden || !((ViewWindow*)mdata->view_win)->is_inlayed())
		menu_flag |= FL_MENU_VALUE;

	// Tab menu.
	menu->add("&View/Tab menu", FL_CTRL + '`', (Fl_Callback*)tab_menu_cb, (void*)mdata, menu_flag);
}
