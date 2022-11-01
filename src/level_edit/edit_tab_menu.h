#pragma once

#include "program_data.h"
#include "tab_menu.h"
#include "debug_window.h"
#include "edit_tile_creater.h"
#include "chat_box.h"
#include "game_commands.h"
#include "edit_commands.h"
#include "edit_item_creater.h"
#include "edit_item_command_list.h"
#include "edit_npc_creater.h"

#define TILE_CREATER_TAB_LABEL "Tile creater"
#define ITEM_CREATER_TAB_LABEL "Item creater"
#define NPC_CREATER_TAB_LABEL "Npc creater"

class EditTabMenu : public TabMenu {
	public:
		EditTabMenu(MainData * md, MENU_SIDE menu_location) : TabMenu(md, menu_location) {
			main_init(md, menu_location);
		}

		void main_init(MainData * md, MENU_SIDE menu_location);

		// Reset items position and size if location changes.
		void reset_items_size();

		// Widgets.
		EditTileCreater * tile_creater = NULL;
		EditItemCreater * item_creater = NULL;
		EditItemCommandList * item_command_list = NULL;
		EditNpcCreater * npc_creater = NULL;
	protected:
		MainData * mdata;
		MENU_SIDE menu_locat;

		void add_items();

		static void scrollbar_cb(Fl_Widget * w, void * d);

		// Tabs.
		Fl_Group * debug_win_tab = NULL;
		Fl_Group * chat_box_tab = NULL;
		Fl_Group * scrollbar_tab = NULL;
		Fl_Group * tile_creater_tab = NULL;
		Fl_Group * item_creater_tab = NULL;
		Fl_Group * item_command_list_tab = NULL;
		Fl_Group * npc_creater_tab = NULL;
};
