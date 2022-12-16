#pragma once

#include "dll_format.h"

extern "C" {

SHARED_LIB void close_game();
SHARED_LIB void wait_for_game_to_close(bool * give_up);
SHARED_LIB void wait_for_game_to_start(bool * give_up);
SHARED_LIB const char * get_game_version();
SHARED_LIB const char * get_release_date();
SHARED_LIB int run_game(GameArgs args);

}
