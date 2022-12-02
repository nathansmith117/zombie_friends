#include "startup.h"
#include "tile.h"

int startup(MainData * mdata, bool change_output_streams) {

	// First init.
	mdata->scale_tile_size = TILE_SIZE * mdata->settings.scale;

	// Get username and main dir.
#ifdef _WIN32
	GetUserName(mdata->user_name, &mdata->user_name_len);
#else
	getlogin_r(mdata->user_name, mdata->user_name_len);
#endif

	snprintf(mdata->MAIN_DIR, MAIN_DIR_SIZE, MAIN_DIR_FORMAT, mdata->user_name);

	// Log file path.
	time_t raw_time;
	char log_file_name[NAME_MAX];

	time(&raw_time);
	mdata->timeinfo = localtime(&raw_time);

	snprintf(log_file_name, NAME_MAX, "%d-%d-%d-%ld.txt", 
		mdata->timeinfo->tm_year + 1900,
		mdata->timeinfo->tm_mon, 
		mdata->timeinfo->tm_mday, 
		time(NULL)
	);

	snprintf(mdata->log_file_path, LOG_PATH_MAX, "%slogs%s%s", mdata->MAIN_DIR, PSLASH, log_file_name);
	snprintf(mdata->error_log_file_path, LOG_PATH_MAX, "%slogs%serror-%s", mdata->MAIN_DIR, PSLASH, log_file_name);

	// Open stdout and stderr.
	if (change_output_streams) {
		freopen(mdata->log_file_path, "w+", stdout);
		freopen(mdata->error_log_file_path, "w+", stderr);
	}

	// Version and other data.
	puts("Starting game");

	printf("Version: %s\n", GAME_VERSION);
	printf("release date: %s\n", GAME_RELEASE_DATE);
	printf("build date: %s\n", __DATE__);
	printf("FLTK abi version: %d\n", Fl::abi_version());
	printf("User: %s\n", mdata->user_name);
	printf("Program Directory: %s\n", mdata->MAIN_DIR);
	printf("Log file path: %s\n", mdata->log_file_path);

	return 0;
}
