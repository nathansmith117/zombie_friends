#pragma once

#include "program_data.h"

#define ARGUMENT_CB_ARGS MainData * mdata, const char * value
typedef int (*ARGUMENT_CB)(ARGUMENT_CB_ARGS); // Returns -1 on error.

struct ArgumentData {
	char name[NAME_MAX];
	ARGUMENT_CB callback;
};

#define ARGUMENTS_SIZE 2
extern const ArgumentData ARGUMENTS[ARGUMENTS_SIZE];

// It will work reguardless of a '-' at the start.
ArgumentData get_data_from_argument(const char * name);

// Argument callbacks.
int set_main_directory_cb(ARGUMENT_CB_ARGS);
int get_startup_script_cb(ARGUMENT_CB_ARGS);

// This is for starting the game, level editor, or launcher.
// Set 'change_output_streams' to true if using debug window.
int startup(MainData * mdata, bool change_output_streams);
void handle_arguments(MainData * mdata);
