#include "program_data.h"
#include "game_tools.h"
#include "editWindow.h"
#include "edit_menu.h"
#include "debug_window.h"
#include "view_window.h"
#include "map.h"
#include "player.h"
#include "edit_tab_menu.h"

void load_first_images(MainData * mdata) {
	gameTools::load_images_from_loader_file(mdata, "../../image_list.il");
}

int main(int argc, char ** argv) {
	MainData mdata;

	// First init.
	mdata.scale_tile_size = TILE_SIZE * mdata.settings.scale;

	// Get username and main dir.
#ifdef _WIN32
	GetUserName(mdata.user_name, &mdata.user_name_len);
#else
	getlogin_r(mdata.user_name, mdata.user_name_len);
#endif

	snprintf(mdata.MAIN_DIR, MAIN_DIR_SIZE, MAIN_DIR_FORMAT, mdata.user_name);

	// Log file path.
	time_t raw_time;
	char log_file_name[NAME_MAX];

	time(&raw_time);
	mdata.timeinfo = localtime(&raw_time);

	snprintf(log_file_name, NAME_MAX, "%d-%d-%d-%d.txt", 
		mdata.timeinfo->tm_year + 1900,
		mdata.timeinfo->tm_mon, 
		mdata.timeinfo->tm_mday, 
		time(NULL)
	);

	snprintf(mdata.log_file_path, LOG_PATH_MAX, "%slogs%s%s", mdata.MAIN_DIR, PSLASH, log_file_name);
	snprintf(mdata.error_log_file_path, LOG_PATH_MAX, "%slogs%serror-%s", mdata.MAIN_DIR, PSLASH, log_file_name);

	// Open stdout and stderr.
	freopen(mdata.log_file_path, "w+", stdout);
	freopen(mdata.error_log_file_path, "w+", stderr);

	// Version and other data.
	puts("Starting editor");
	printf("Version: %s\n", EDITOR_VERSION);
	printf("release date: %s\n", EDITOR_RELEASE_DATE);
	printf("build date: %s\n", __DATE__);
	printf("FLTK abi version: %d\n", Fl::abi_version());

	printf("User: %s\n", mdata.user_name);
	printf("Program Directory: %s\n", mdata.MAIN_DIR);

	load_first_images(&mdata);

	mdata.map = new Map(&mdata);

	// Start size.
	if (mdata.settings.editor.start_with_created_map)
		mdata.map->create_map(
			mdata.settings.editor.start_width, 
			mdata.settings.editor.start_height
		);

	// Window.
	mdata.win = new EditWindow(&mdata, Fl::w() / 2, Fl::h() / 2, "Level editor");

	// Top Menu.
	mdata.top_menu = new Fl_Menu_Bar(mdata.settings.menu.x, mdata.settings.menu.y, 
			(mdata.settings.menu.w == -1) ? mdata.win->w() : mdata.settings.menu.w,
			mdata.settings.menu.h);

	// View window.
	mdata.view_win = new ViewWindow(
		&mdata, 
		mdata.settings.editor.side_menu_width, 
		mdata.settings.menu.h, 
		mdata.win->w() - mdata.settings.editor.side_menu_width, 
		mdata.win->h() - mdata.settings.menu.h, 
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
