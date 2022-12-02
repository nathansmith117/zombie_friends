#include "program_data.h"
#include "game_tools.h"
#include "editWindow.h"
#include "edit_menu.h"
#include "debug_window.h"
#include "view_window.h"
#include "map.h"
#include "player.h"
#include "edit_tab_menu.h"
#include "loader_file_handling.h"
#include "settings_editor.h"
#include "startup.h"

void load_first_images(MainData * mdata) {
	load_images_from_loader_file(mdata, "../../image_list.il");
}

int main(int argc, char ** argv) {
	fl_register_images();
	MainData mdata;

	mdata.argc = argc;
	mdata.argv = argv;

	// First init.
	startup(&mdata, true);

	load_first_images(&mdata);

	mdata.map = new Map(&mdata);

	// Settings editor.
	mdata.settings_editor = new SettingsEditor(&mdata);

	// Start size.
	if (mdata.settings.editor.start_with_created_map)
		mdata.map->create_map(
			mdata.settings.editor.start_width, 
			mdata.settings.editor.start_height
		);

	// Window.
	mdata.win = new EditWindow(&mdata, Fl::w() / 2, Fl::h() / 2, "Level editor");

	// Top Menu.
	mdata.top_menu = new Fl_Menu_Bar(
		0,
		0,
		mdata.win->w(),
		mdata.settings.menu.height
	);

	// View window.
	mdata.view_win = new ViewWindow(
		&mdata, 
		0, 
		mdata.settings.menu.height, 
		mdata.win->w(), 
		mdata.win->h() - mdata.settings.menu.height, 
		"Map viewer"
	);

	mdata.view_win->end();
	mdata.view_win->resizable(mdata.view_win);

	// Tab menu.
	mdata.tab_menu = new EditTabMenu(&mdata, mdata.settings.tab_menu_locat);

	// After everything else so it knows what is null.
	add_editor_menu_items(&mdata, mdata.top_menu);

	// End.
	mdata.win->end();
	mdata.win->resizable(mdata.win);

	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.win->show(argc, argv);

	// Inlay view window in window.
	mdata.win->begin();
	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.view_win->show();
	mdata.win->end();

	return Fl::run();
}
