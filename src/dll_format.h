#pragma once

#include "program_data.h"

extern "C" {

void close_game();
typedef void (*CLOSE_GAME_CB)();

void wait_for_game_to_close();
typedef void (*WAIT_FOR_GAME_TO_CLOSE_CB)();

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

}
