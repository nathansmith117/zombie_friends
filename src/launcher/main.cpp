#include "program_data.h"
#include "dll_format.h"
#include "launcher_window.h"
#include "launcher_top_menu.h"
#include "launcher.h"
#include "settings_editor.h"

int main(int argc, char ** argv) {
	fl_register_images();
	MainData mdata;

	mdata.argc = argc;
	mdata.argv = argv;

	// First init.
	mdata.scale_tile_size = 0;

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
	//freopen(mdata.log_file_path, "w+", stdout);
	//freopen(mdata.error_log_file_path, "w+", stderr);

	// Version and other data.
	puts("Starting Launcher");

	printf("Version: %s\n", GAME_VERSION);
	printf("release date: %s\n", GAME_RELEASE_DATE);
	printf("build date: %s\n", __DATE__);
	printf("FLTK abi version: %d\n", Fl::abi_version());
	printf("User: %s\n", mdata.user_name);
	printf("Program Directory: %s\n", mdata.MAIN_DIR);
	printf("Log file path: %s\n", mdata.log_file_path);

	mdata.launcher = new Launcher();

	//mdata.launcher->load("/home/nathan/Desktop/zombie_friends/launcher_dlls/zombie_friends.so");
	//mdata.launcher->run({argc, argv}, false);
	//mdata.launcher->close();

	//mdata.launcher->load("/home/nathan/Desktop/zombie_friends/launcher_dlls/zombie_friends.so");
	//mdata.launcher->run({argc, argv}, false);
	//mdata.launcher->close();

	//mdata.launcher->load("/home/nathan/Desktop/zombie_friends/launcher_dlls/zombie_friends.so");
	//mdata.launcher->run({argc, argv}, false);
	//mdata.launcher->close();

	//delete launcher;

	// Settings editor.
	mdata.settings_editor = new SettingsEditor(&mdata);

	// Launcher window.
	mdata.win = new LauncherWindow(&mdata, Fl::w() / 2, Fl::h() / 2, "Zombie Friends Launcher");

	// Top menu.
	mdata.top_menu = new Fl_Menu_Bar(
		0,
		0,
		mdata.win->w(),
		mdata.settings.menu.height
	);

	// Add menu items after creating everything just in case something is null.
	add_launcher_menu_items(mdata.top_menu, &mdata);

	mdata.win->end();
	mdata.win->resizable(mdata.win);
	mdata.win->show(argc, argv);

	return Fl::run();
}
