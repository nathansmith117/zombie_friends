#pragma once

#include "program_data.h"
#include "chat_box.h"
#include "game_tools.h"
#include "map.h"
#include "view_window.h"
#include "edit_tile_creater.h"
#include "edit_tab_menu.h"

// Commands.
int edit_create_command(COM_CB_ARGS);
int edit_fill_command(COM_CB_ARGS);

void add_edit_commands(MainData * mdata, ChatBox * chat_box);
