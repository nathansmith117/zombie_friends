#include "common_tool.h"
#include "program_data.h"
#include "appWindow.h"
#include "image_loader.h"
#include "player.h"
#include "map.h"
#include "game_tools.h"
#include "debug_window.h"
#include "view_window.h"
#include "tile.h"
#include "top_menu.h"
#include "character.h"
#include "tab_menu.h"
#include "game_tab_menu.h"
#include "common_item.h"
#include "progress_bar.h"
#include "settings_editor.h"

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

	snprintf(log_file_name, NAME_MAX, "%d-%d-%d-%ld.txt", 
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
	puts("Starting game");

	printf("Version: %s\n", GAME_VERSION);
	printf("release date: %s\n", GAME_RELEASE_DATE);
	printf("build date: %s\n", __DATE__);
	printf("FLTK abi version: %d\n", Fl::abi_version());
	printf("User: %s\n", mdata.user_name);
	printf("Program Directory: %s\n", mdata.MAIN_DIR);
	printf("Log file path: %s\n", mdata.log_file_path);

	load_first_images(&mdata);

	// Create map.
	const char map_file_name[] = "map_data.mp";
	mdata.map = new Map(&mdata);
	mdata.map->load(map_file_name, NAME_MAX);

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
	if (!mdata.images.icon->fail()) {
		mdata.win->icon(mdata.images.icon);
		mdata.view_win->icon(mdata.images.icon);
	}
#endif

	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.win->show(argc, argv);

	mdata.win->begin();
	Fl::visual(FL_DOUBLE | FL_INDEX);
	mdata.view_win->show();
	mdata.win->end();

	return Fl::run();
}
