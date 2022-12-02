#include "launcher.h"

Launcher::Launcher() {
	memset(dll_path, 0, NAME_MAX);
}

Launcher::~Launcher() {
	close();
}

int Launcher::load(const char * file_path) {
	int i;

	close();

	if (file_path == NULL) {
		fputs("File path is null\n", stderr);
		return -1;
	}

	// Get handle.
	game_handle = DllLoader::load(file_path);

	if (game_handle == NULL) {
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	}

	// Get run function.
	run_game_cb = (RUN_GAME_CB)DllLoader::get_address(game_handle, "run_game");

	if (run_game_cb == NULL) {
		fprintf(stderr, "Error loading 'run_game' from %s\n", file_path);
		DllLoader::close(game_handle);
		game_handle = NULL;
		return -1;
	}

	memset(dll_path, 0, NAME_MAX);
	strncat(dll_path, file_path, NAME_MAX - 1);

	printf("Finished loading %s\n", file_path);
	return 0;
}

void Launcher::close() {

	// Not loaded.
	if (game_handle == NULL)
		return;

	// Stop thread.
	if (run_thread != NULL)
		stop_thread();

	DllLoader::close(game_handle);
	game_handle = NULL;
	run_game_cb = NULL;

	fprintf(stderr, "Finished closing %s\n", dll_path);
	memset(dll_path, 0, NAME_MAX);
}

int Launcher::run(GameArgs args, bool threaded_mode) {
	if (!is_loaded()) {
		fputs("No game is loaded\n", stderr);
		return -1;
	}

	if (threaded_mode) {
		run_thread = new std::thread(run_cb, args, this);
		return 0;
	}

	return run_game_cb(args);
}

void Launcher::stop_thread() {
	if (run_thread == NULL || !is_loaded())
		return;

	// Get close callback.
	CLOSE_GAME_CB close_game_cb = (CLOSE_GAME_CB)DllLoader::get_address(game_handle, "close_game");

	if (close_game_cb == NULL) {
		fprintf(stderr, "Error loading 'close_game' from %s\n", dll_path);
		return;
	}

	close_game_cb();

	run_thread->join();
	delete run_thread;
	run_thread = NULL;
}

int Launcher::wait_for_game_to_close() {
	WAIT_FOR_GAME_TO_CLOSE_CB wait_for_game_to_close_cb;

	if (!is_loaded())
		return -1;

	// Load callback.
	wait_for_game_to_close_cb = (WAIT_FOR_GAME_TO_CLOSE_CB)DllLoader::get_address(game_handle, "wait_for_game_to_close");

	if (wait_for_game_to_close_cb == NULL) {
		fprintf(stderr, "Error loading 'wait_for_game_to_close' from %s\n", dll_path);
		return -1;
	}

	wait_for_game_to_close_cb();
	return 0;
}

void Launcher::run_cb(GameArgs args, Launcher * launcher) {
	if (launcher == NULL)
		return;

	launcher->run(args, false);
}
