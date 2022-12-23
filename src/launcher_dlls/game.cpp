#include "game.h"

#include "dll_format.h"
#include "common_tool.h"
#include "appWindow.h"
#include "image_loader.h"
#include "player.h"
#include "map.h"
#include "game_tools.h"
#include "debug_window.h"
#include "program_data.h"
#include "view_window.h"
#include "tile.h"
#include "top_menu.h"
#include "character.h"
#include "tab_menu.h"
#include "game_tab_menu.h"
#include "common_item.h"
#include "progress_bar.h"
#include "settings_editor.h"
#include "startup.h"

MainData mdata;

SHARED_LIB void close_game() {
	mdata.should_close = true;
}

SHARED_LIB void wait_for_game_to_close(bool * give_up) {
	while (mdata.state != GAME_STOPPED) {
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		if (give_up != NULL)
			if (*give_up)
				break;
	}
}

SHARED_LIB void wait_for_game_to_start(bool * give_up) {
	while (mdata.state == GAME_NOT_STARTED) {
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		if (give_up != NULL)
			if (*give_up)
				break;
	}
}

SHARED_LIB const char * get_game_version() {
	return GAME_VERSION;
}

SHARED_LIB const char * get_release_date() {
	return GAME_RELEASE_DATE;
}

void init_images(MainData * mdata) {
	// Icon.
	char icon_path[NAME_MAX];
	memset(icon_path, 0, NAME_MAX);
	snprintf(icon_path, NAME_MAX, "%simages%sicon.png", mdata->MAIN_DIR, PSLASH);

	mdata->images.icon = new Fl_PNG_Image(icon_path);

	// Format image path.
	char img_data_path[NAME_MAX];
	memset(img_data_path, 0, NAME_MAX);
	snprintf(img_data_path, NAME_MAX, "%simage_list.il", mdata->MAIN_DIR);

	gameTools::load_file(mdata, img_data_path);
}

SHARED_LIB int run_game(GameArgs args) {
	fl_register_images();

	mdata.argc = args.argc;
	mdata.argv = args.argv;

	// First init.
	if (startup(&mdata, true) == -1)
		return -1;

	//init_images(&mdata);

	// Create map.
	const char map_file_name[] = "map_data.mp";
	mdata.map = new Map(&mdata);
	//mdata.map->load(map_file_name, NAME_MAX);

	// Settings editor.
	mdata.settings_editor = new SettingsEditor(&mdata);

	// Open window.
	mdata.win = new AppWindow(&mdata, Fl::w() / 2, Fl::h() / 2, "Zombie Friends");

	// Top menu.
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
		"Game"
	);

	// Player.
	mdata.player = new Player(&mdata);

	mdata.view_win->end();
	mdata.view_win->resizable(mdata.view_win);

	// Tab menu.
	mdata.tab_menu = new GameTabMenu(&mdata, mdata.settings.tab_menu_locat);

	// Add menu items after creating everything just in case something is null.
	add_menu_items(mdata.top_menu, &mdata);

	mdata.win->end();
	mdata.win->resizable(mdata.win);

	// Icon.
#ifdef _WIN32
	// Stuff.
#else
	/*
	if (!mdata.images.icon->fail()) {
		mdata.win->icon(mdata.images.icon);
		mdata.view_win->icon(mdata.images.icon);
	}
	*/
#endif

	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.win->show();

	mdata.win->begin();
	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.view_win->show();
	mdata.win->end();

	// Load startup script.
	if (mdata.startup_script[0] != '\0')
		gameTools::load_file(&mdata, mdata.startup_script);

	return Fl::run();
}

SHARED_LIB void * ADDRESS_LIST[] = {
	(void*)close_game,
	(void*)wait_for_game_to_close,
	(void*)wait_for_game_to_start,
	(void*)get_game_version,
	(void*)get_release_date,
	(void*)run_game
};

SHARED_LIB ADDR_LIST_SIZE ADDRESS_LIST_SIZE = sizeof(ADDRESS_LIST) / sizeof(void*);
