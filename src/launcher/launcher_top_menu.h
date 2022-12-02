#pragma once

#include "program_data.h"
#include "top_menu.h"

// Callbacks.
void launcher_open_cb(Fl_Widget * w, void * d);
void launcher_close_game_cb(Fl_Widget * w, void * d);

void add_launcher_menu_items(Fl_Menu_Bar * menu, MainData * mdata);
