#pragma once

#include "program_data.h"

extern "C" {

void close_game();
typedef void (*CLOSE_GAME_CB)();

void wait_for_game_to_close(bool * give_up);
typedef void (*WAIT_FOR_GAME_TO_CLOSE_CB)(bool * give_up);

void wait_for_game_to_start(bool * give_up);
typedef void (*WAIT_FOR_GAME_TO_START_CB)(bool * give_up);

const char * get_game_version();
typedef const char * (*GET_GAME_VERSION_CB)();

const char * get_release_date();
typedef const char * (*GET_RELEASE_DATE_CB)();

// Data for launching the game.
struct GameArgs {
	int argc;
	char ** argv;
};

// This is where everything starts.
int run_game(GameArgs args);
typedef int (*RUN_GAME_CB)(GameArgs args);

// Address list stores all of the function pointers.
enum ADDRESS_LIST_ITEMS {
	CLOSE_GAME,
	WAIT_FOR_GAME_TO_CLOSE,
	WAIT_FOR_GAME_TO_START,
	GET_GAME_VERSION,
	GET_RELEASE_DATE,
	RUN_GAME
};

// Make sure 'const void * ADDRESS_LIST[]' is in the right order and only add new items to the bottom.
// Use null if item not defined.

typedef void ** ADDR_LIST;
typedef int32_t ADDR_LIST_SIZE;

}
