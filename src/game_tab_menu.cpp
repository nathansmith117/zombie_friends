#include "game_tab_menu.h"

void GameTabMenu::main_init(MainData * md, MENU_SIDE menu_location) {
	mdata = md;

	menu_side(menu_location, false);
	add_items();

	menu_side(menu_location);
}

void GameTabMenu::add_items() {
	int tab_x, tab_y, tab_w, tab_h;
	tab_x = x();
	tab_y = y() + mdata->settings.tab_menu_tab_size;
	tab_w = w();
	tab_h = h() - mdata->settings.tab_menu_tab_size;

	// Debug window tab.
	debug_win_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h, 
		"Debug info"
	);

	mdata->debug_win = new DebugWindow(
		mdata, 
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h - ((mdata->settings.tab_menu_locat == MENU_TOP) ? mdata->top_menu->h() : 0)
	);

	debug_win_tab->end();

	// Chat box tab.
	chat_box_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h,
		"Chat box"
	);

	mdata->chat_box = new ChatBox(
		mdata,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	add_main_commands(mdata, mdata->chat_box);
	add_game_only_commands(mdata, mdata->chat_box);

	chat_box_tab->end();

	// Player info tab.
	player_info_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h,
		"Player info"
	);

	mdata->player_info_display = new PlayerInfoDisplay(
		mdata,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	player_info_tab->end();

	if (mdata->settings.tab_menu_default_hidden)
		menu_side(MENU_HIDDEN);
	else
		menu_side(menu_locat);

	end();
}

void GameTabMenu::reset_items_size() {
	int tab_x, tab_y, tab_w, tab_h;
	tab_x = x();
	tab_y = y() + mdata->settings.tab_menu_tab_size;
	tab_w = w();
	tab_h = h() - mdata->settings.tab_menu_tab_size;

	// Debug window tab.
	if (debug_win_tab != NULL)
		debug_win_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h 
		);

	// Debug window.
	if (mdata->debug_win != NULL)
		mdata->debug_win->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h - ((mdata->settings.tab_menu_locat == MENU_TOP) ? mdata->top_menu->h() : 0)
		);

	// Chat box tab.
	if (chat_box_tab != NULL)
		chat_box_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Chat box.
	if (mdata->chat_box != NULL) {
		mdata->chat_box->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		((ChatBox*)mdata->chat_box)->reset_size();
	}

	// Player info tab.
	if (player_info_tab != NULL)
		player_info_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Player info display.
	if (mdata->player_info_display != NULL) {
		mdata->player_info_display->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		((PlayerInfoDisplay*)mdata->player_info_display)->reset_size();
	}
}
