#include "launcher.h"
#include "game_tools.h"

Launcher::Launcher() {
	memset(dll_path, 0, NAME_MAX);
}

Launcher::~Launcher() {
	close();
}

int Launcher::load(const char * file_path) {
	int i;
	DllLoader::ADDRESS temp = NULL;

	close();

	if (file_path == NULL) {
		fputs("File path is null\n", stderr);
		return -1;
	}

	// Get handle.
	game_handle = DllLoader::load(file_path);

	if (game_handle == NULL) {
		fprintf(stderr, "Error loading %s\n", file_path);
		return -1;
	}

	// Get address list size.
	temp = DllLoader::get_address(game_handle, "ADDRESS_LIST_SIZE");

	if (temp == NULL) {
		fprintf(stderr, "Error loading 'ADDRESS_LIST_SIZE' from %s\n", file_path);
		DllLoader::close(game_handle);
		game_handle = NULL;
		return -1;
	}

	address_list_size = *((ADDR_LIST_SIZE*)temp);

	if (address_list_size <= 0) {
		fputs("Address list size needs to be larger then 0\n", stderr);
		DllLoader::close(game_handle);
		game_handle = NULL;
		return -1;
	}

	// Get address list.
	temp = DllLoader::get_address(game_handle, "ADDRESS_LIST");

	if (temp == NULL) {
		fprintf(stderr, "Error loading 'ADDRESS_LIST' from %s\n", file_path);
		DllLoader::close(game_handle);
		game_handle = NULL;
		return -1;
	}

	address_list = (ADDR_LIST)temp;

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
	address_list = NULL;
	run_failed = false;

	fprintf(stderr, "Finished closing %s\n", dll_path);
	memset(dll_path, 0, NAME_MAX);
}

int Launcher::run(GameArgs args, bool threaded_mode) {
	int res;
	run_failed = false;

	if (!is_loaded()) {
		fputs("No game is loaded\n", stderr);
		return -1;
	}

	// Threaded mode time!!!!
	if (threaded_mode) {
		run_thread = new std::thread(run_cb, args, this);
		return 0;
	}

	res = ((RUN_GAME_CB)get_item(RUN_GAME, (void*)int_nothing))(args);

	if (res == -1)
		run_failed = true;

	return res;
}

int Launcher::test() {

	printf("%s\n", is_loaded() ? "Loaded" : "Not loaded");

	if (address_list == NULL)
		puts("Address list is null");

	if (DllLoader::get_address(game_handle, "close_game") == NULL) {
		fputs("Test failed\n", stderr);
		return -1;
	}

	puts("Nothing wrong in testing you dummy\n");
	return 0;
}

void Launcher::stop_thread() {
	if (run_thread == NULL || !is_loaded())
		return;

	// Call close.
	((CLOSE_GAME_CB)get_item(CLOSE_GAME, (char*)void_nothing))();

	run_thread->join();
	delete run_thread;
	run_thread = NULL;
}

void Launcher::wait_for_game_to_close() {
	((WAIT_FOR_GAME_TO_CLOSE_CB)get_item(WAIT_FOR_GAME_TO_CLOSE, (void*)void_nothing))(&run_failed);
}

void Launcher::wait_for_game_to_start() {
	if (run_failed)
		return;

	((WAIT_FOR_GAME_TO_START_CB)get_item(WAIT_FOR_GAME_TO_START, (void*)void_nothing))(&run_failed);
}

void Launcher::run_cb(GameArgs args, Launcher * launcher) {
	if (launcher == NULL)
		return;

	launcher->run(args, false);
}

void * Launcher::get_item(int item_id, void * return_on_error, bool print_errors) {
	void * item_address = NULL;

	// Check size and address list.
	if (!is_loaded()) {
		if (print_errors)
			fputs("Handle is not loaded\n", stderr);

		return return_on_error;
	} else if (address_list == NULL) {
		if (print_errors)
			fputs("Address list is null\n", stderr);

		return return_on_error;
	} else if (address_list_size <= 0 || item_id >= address_list_size) {
		if (print_errors)
			fputs("Address list is to small\n", stderr);

		return return_on_error;
	} else if (item_id < 0) {
		if (print_errors)
			fputs("Item id for address list can not be less then zero\n", stderr);

		return return_on_error;
	}

	item_address = address_list[item_id];

	// Item address is null.
	if (item_address == NULL) {
		if (print_errors)
			fprintf(stderr, "Item %d is null\n", item_id);

		return return_on_error;
	}

	return item_address;
}

void Launcher::exec_dll(const char * command_path, const char * file_path, const char ** options, size_t options_size) {
	int i;

	// Create 'argv'.
	size_t argv_size = options_size + 3;
	const char * argv[options_size];

	argv[0] = command_path;
	argv[1] = file_path;
	argv[argv_size - 1] = NULL; // Set last item to null.

	// Copy items from 'options' to 'argv'.
	if (options != NULL)
		for (i = 2; i < argv_size - 1; i++) // Skip 'command_path', 'file_path', and last item.
			argv[i] = options[i - 2];

	// Print command with c array style format.
	printf("Running command: {");

	for (i = 0; i < argv_size; i++) {
		if (argv[i] == NULL)
			continue;
		else if (i == argv_size - 2) // At last item minus the null one.
			printf("\"%s\"", argv[i]);
		else
			printf("\"%s\", ", argv[i]);
	}

	puts("}");

	// Run command.
	execv(command_path, (char*const*)argv);

error_and_stuff:

	// Error.
	fprintf(stderr, "Error running %s\n", command_path);
	fl_alert("Error running %s\n", command_path);
}
