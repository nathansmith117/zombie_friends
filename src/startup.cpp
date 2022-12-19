#include "startup.h"
#include "tile.h"

const ArgumentData ARGUMENTS[ARGUMENTS_SIZE] = {
	{"md", set_main_directory_cb}
};

ArgumentData get_data_from_argument(const char * name) {
	int i, start_at;
	ArgumentData arg_data = {"\0", NULL};
	char formated_name[NAME_MAX];

	if (name == NULL)
		return arg_data;

	// Get formated name.
	memset(formated_name, 0, NAME_MAX);

	// Skip '-'.
	start_at = (name[0] == '-') ? 1 : 0;

	for (i = start_at; i < NAME_MAX; i++) {
		formated_name[i - start_at] = name[i];

		if (name[i] == '\0')
			break;
	}

	// Find in list.
	for (auto a : ARGUMENTS)
		if (strncmp(formated_name, a.name, NAME_MAX) == 0)
			return a;

	return arg_data;
}

int set_main_directory_cb(ARGUMENT_CB_ARGS) {
	size_t s;

	memset(mdata->MAIN_DIR, 0, MAIN_DIR_SIZE);
	strncat(mdata->MAIN_DIR, value, MAIN_DIR_SIZE - 1);

	s = strnlen(mdata->MAIN_DIR, MAIN_DIR_SIZE);

	// Add slash at end if not there.
	if (mdata->MAIN_DIR[s] != PSLASH[0])
		strncat(mdata->MAIN_DIR, PSLASH, sizeof(PSLASH));

	return 0;
}

int startup(MainData * mdata, bool change_output_streams) {
	struct stat st;

	memset(mdata->MAIN_DIR, 0, MAIN_DIR_SIZE);
	memset(mdata->log_file_path, 0, LOG_PATH_MAX);

	handle_arguments(mdata);

	// First init.
	mdata->scale_tile_size = TILE_SIZE * mdata->settings.scale;

	// Get username and main dir.
#ifdef _WIN32
	GetUserName(mdata->user_name, &mdata->user_name_len);
#else
	getlogin_r(mdata->user_name, mdata->user_name_len);
#endif

	//snprintf(mdata->MAIN_DIR, MAIN_DIR_SIZE, MAIN_DIR_FORMAT, mdata->user_name);
	
	// Set to default main directory if not already set.
	if (mdata->MAIN_DIR[0] == '\0')
		strncat(mdata->MAIN_DIR, DEFAULT_MAIN_DIR, MAIN_DIR_SIZE - 1);

	// Time.
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

	// Log file path.
	if (!change_output_streams)
		goto skip_log_files;

	//snprintf(mdata->log_file_path, LOG_PATH_MAX, "%slogs%s%s", mdata->MAIN_DIR, PSLASH, log_file_name);
	snprintf(mdata->log_file_path, LOG_PATH_MAX, "%slogs", mdata->MAIN_DIR);

	// Check if it exists.
	if (stat(mdata->log_file_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
		fprintf(stderr, "%s does not exists\n", mdata->log_file_path);
		return -1;
	}

	// Finished formatting.
	memset(mdata->log_file_path, 0, LOG_PATH_MAX);
	snprintf(mdata->log_file_path, LOG_PATH_MAX, "%slogs%s%s", mdata->MAIN_DIR, PSLASH, log_file_name);

	snprintf(mdata->error_log_file_path, LOG_PATH_MAX, "%slogs%serror-%s", mdata->MAIN_DIR, PSLASH, log_file_name);

	// Open stdout and stderr.
	freopen(mdata->log_file_path, "w+", stdout);
	freopen(mdata->error_log_file_path, "w+", stderr);

skip_log_files:

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

void handle_arguments(MainData * mdata) {
	int i;
	ArgumentData curr_arg;
	bool running_callback = false;

	if (mdata->argc <= 1 || mdata->argv == NULL)
		return;

	// Get each argument and run its callback.
	for (i = 1; i < mdata->argc; i++) {
		if (running_callback) {
			if (curr_arg.callback != NULL)
				curr_arg.callback(mdata, mdata->argv[i]);

			running_callback = false;
			continue;
		}

		curr_arg = get_data_from_argument(mdata->argv[i]);
		running_callback = true;

		// Invaild argument.
		if (curr_arg.name[0] == '\0')
			fprintf(stderr, "%s is a invaild argument\n", mdata->argv[i]);
	}
}
