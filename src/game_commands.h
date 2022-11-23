#pragma once

#include "program_data.h"
#include "chat_box.h"
#include "game_tools.h"
#include "map.h"
#include "view_window.h"

#define PRINT_STDOUT_ARG "stdout"
#define PRINT_STDERR_ARG "stderr"
#define PRINT_CHATBOX_ARG "chatbox"

// Print type stuff.
enum PRINT_TYPES {
	PRINT_UNKNOWN,
	PRINT_STDOUT,
	PRINT_STDERR,
	PRINT_CHATBOX
};

typedef uint8_t PRINT_TYPE;

PRINT_TYPE get_print_type_from_str(const char * str);
int print_to(PRINT_TYPE type, const char * message, ChatBox * chat_box=NULL);

// /say what ever you want
int say_command(COM_CB_ARGS);

// No arguments.
int exit_command(COM_CB_ARGS);

#define LOAD_MAIN_DIR_ARG "md"
#define LOAD_SCRIPT_LOCATION_ARG "sl"

enum LOAD_LOCATIONS {
	LOAD_MAIN_DIR,
	LOAD_SCRIPT_LOCATION
};

// /load [file_name] [location=md]
// md for main directory. Image folder location only.
// sl for script location. Image folder and file path.  Full path doesn't work in this mode.
int load_command(COM_CB_ARGS);

// Space filler command.
int do_nothing_command(COM_CB_ARGS);

// Show pop message.
int popup_command(COM_CB_ARGS);

// Print to stdout or stderr.
// print [stream] message...
int print_command(COM_CB_ARGS);

// Runs commands in sequence
// run_sequence command1 arg1 arg2 arg3 | command2 | command3 arg1 arg2
int run_sequence_command(COM_CB_ARGS);

// Adding or setting global vars.
// set [variable_name] [value of variable]
int set_command(COM_CB_ARGS);

// Print out all of the global variables.
// show_globals [output]
// default output is 'chatbox'.
int show_globals_command(COM_CB_ARGS);

#define EQUAL_ARG "=="
#define NOT_EQUAL_ARG "!="

enum COMPARE_TYPES {
	COMPARE_EQUAL,
	COMPARE_NOT_EQUAL
};

typedef uint8_t COMPARE_TYPE;

// if value1 operator value2 command arg...
int if_command(COM_CB_ARGS);

// List all the commands and print into about each one.
// list_commands [output]
// default output is 'chatbox'.
int list_commands_command(COM_CB_ARGS);


#define SCRIPT_LOCATION_GLOBAL "SCRIPT_LOCATION"

// Show the script location.
// show_script_locat [output]
// default output is chat box.
int show_script_locat_command(COM_CB_ARGS);

// Update script location global.
int get_script_locat_command(COM_CB_ARGS);

// This code looks like something your crazy english teacher would make.
enum IS_THINGS {
	IS_EMPTY,
	IS_NOT_EMPTY
};

typedef uint8_t IS_THING_TYPE;

#define IS_EMPTY_ARG "empty"
#define IS_NOT_EMPTY_ARG "not_empty"

// Run command if value is thing.
// value thing command arg...
int if_is_command(COM_CB_ARGS);

// Game only commands.
int change_heath_by_command(COM_CB_ARGS);
int set_heath_command(COM_CB_ARGS);

int change_coin_count_by_command(COM_CB_ARGS);
int set_coin_count_command(COM_CB_ARGS);

// Adding commands.
void add_main_commands(MainData * mdata, ChatBox * chat_box);
void add_game_only_commands(MainData * mdata, ChatBox * chat_box);
