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
	// Get image data directory.
	char img_data_name[] = "image_list.il";
	char img_data_path[NAME_MAX + sizeof(img_data_name)];
	snprintf(img_data_path, sizeof(img_data_path), "%s%s", mdata->MAIN_DIR, img_data_name);

	// Icon.
	std::string icon_path = std::string(mdata->MAIN_DIR) 
		+ std::string("images") + std::string(PSLASH) + std::string("icon.png");

	mdata->images.icon = new Fl_PNG_Image(icon_path.c_str());

	// Check length of path.
	if (strnlen(img_data_path, sizeof(img_data_path)) > NAME_MAX)
		fprintf(stderr, "%s is longer than NAME_MAX", img_data_path);

	// Format image path.
	char img_folder[] = "images";
	char image_dir[NAME_MAX + sizeof(img_folder) + 1];
	snprintf(image_dir, sizeof(image_dir), "%s%s%s", mdata->MAIN_DIR, img_folder, PSLASH);

	if (strnlen(image_dir, sizeof(image_dir)) > NAME_MAX)
		fprintf(stderr, "%s is longer than NAME_MAX", image_dir);

	//gameTools::load_images_from_loader_file(mdata, img_data_path);
	gameTools::load_file(mdata, img_data_path);
}

int main(int argc, char ** argv) {
	fl_register_images();
	MainData mdata;

	mdata.argc = argc;
	mdata.argv = argv;

	// First init.
	if (startup(&mdata, true) == -1)
		return -1;

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
	mdata.win->show();

	// Inlay view window in window.
	mdata.win->begin();
	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.view_win->show();
	mdata.win->end();

	return Fl::run();
}
