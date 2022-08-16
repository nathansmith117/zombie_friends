#pragma once

#include "program_data.h"
#include "tab_menu.h"
#include "debug_window.h"
#include "chat_box.h"
#include "game_commands.h"
#include "player_info_display.h"

class GameTabMenu : public TabMenu {
	public:
		GameTabMenu(MainData * md, MENU_SIDE menu_location) : TabMenu(md, menu_location) {
			main_init(md, menu_location);
		}

		void main_init(MainData * md, MENU_SIDE menu_location);

		// Reset items position and size if location changes.
		void reset_items_size();
	protected:
		void add_items();

		// Tabs.
		Fl_Group * debug_win_tab = NULL;
		Fl_Group * chat_box_tab = NULL;
		Fl_Group * player_info_tab = NULL;
};
