#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "launcher.h"
#include "dll_format.h"
#include "program_data.h"
#include "game_tools.h"

int old_stdout;
int old_stderr;
bool streams_stored = false;

GameArgs * game_args = NULL;
Launcher * launcher = NULL;

size_t args_width;
size_t args_height;
char ** args = NULL;

void clean_memory() {
	if (launcher != NULL)
		delete launcher;
	if (game_args != NULL)
		delete game_args;
	if (args != NULL)
		gameTools::delete_2d_array<char>(args, args_width, args_height);

	launcher = NULL;
	game_args = NULL;
	args = NULL;
}

void store_streams() {
	old_stdout = dup(STDOUT_FILENO);
	old_stderr = dup(STDERR_FILENO);
	streams_stored = true;
}

void restore_streams() {
	if (!streams_stored)
		return;

	// Clear stdout and stderr.
	fseek(stdout, 0, SEEK_SET);
	fseek(stderr, 0, SEEK_SET);
	fputc(0, stdout);
	fputc(0, stderr);

	// Restore stdout and stderr.
	dup2(old_stdout, STDOUT_FILENO);
	dup2(old_stderr, STDERR_FILENO);

	streams_stored = false;
}

void sig_handler(int sig) {
	restore_streams();
	clean_memory();
	exit(0);
}

int main(int argc, char ** argv) {
	int i;
	int res = 0;

	size_t argv_item_size;

	// Set signal handler.
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGKILL, sig_handler);

	if (argc <= 1) {
		fputs("No arguments given\n", stderr);
		return -1;
	}

	store_streams();

	// Launch.
	launcher = new Launcher();

	if (launcher->load(argv[1]) == -1) {
		res = -1;
		goto end;
	}

	// Get string size in argv.
	args_width = 0;
	args_height = argc - 1;

	for (i = 1; i < argc; i++) { // Skip first item.
		argv_item_size = strlen(argv[i]);

		if (argv_item_size > args_width)
			args_width = argv_item_size;
	}

	args_width += 2; // More space for null byte.

	// Create arguments.
	game_args = new GameArgs;
	args = gameTools::create_2d_array<char>(args_width, args_height);

	if (args == NULL) {
		fprintf(stderr, "Error getting memory for arguments while loading %s\n", launcher->get_path());
		res = -1;
		goto end;
	}

	// Add arguments.
	for (i = 0; i < args_height; i++) {
		memset(args[i], 0, args_width);
		strncat(args[i], argv[i + 1], args_width - 1);
	}

	game_args->argc = args_height;
	game_args->argv = args;

	// Run.
	launcher->run(*game_args, false);

end:
	restore_streams();
	clean_memory();
	return res;
}
