#include "edit_tab_menu.h"
#include "view_window.h"

void EditTabMenu::main_init(MainData * md, MENU_SIDE menu_location) {
	mdata = md;

	menu_side(menu_location, false);
	add_items();

	menu_side(menu_location);
}

void EditTabMenu::add_items() {
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
	add_edit_commands(mdata, mdata->chat_box);

	chat_box_tab->end();

	// Scrollbar tab.
	scrollbar_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h, 
		"Scroll bars"
	);

	/* 
	Vertical and horizontal does not refer to the scrollbar type but what it
	does.
	*/

	// Vertical scrollbar.
	mdata->map_vertical_scrollbar = new Fl_Scrollbar(
		x(), 
		y() + (h() / 2) - mdata->settings.editor.scrollbar_thickness, 
		w(), 
		mdata->settings.editor.scrollbar_thickness,
		"Vertical"
	);

	// Horizontal scrollbar.
	mdata->map_horizontal_scrollbar = new Fl_Scrollbar(
		x(), 
		y() + (h() / 2) + (mdata->settings.editor.scrollbar_thickness * 2), 
		w(), 
		mdata->settings.editor.scrollbar_thickness,
		"Horizontal"
	);

	// Vertical scrollbar display.
	mdata->map_vertical_scrollbar_output = new Fl_Output(
		x() + mdata->settings.editor.scrollbar_output_width,
		y() + mdata->settings.tab_menu_tab_size,
		mdata->settings.editor.scrollbar_output_width,
		mdata->settings.editor.scrollbar_output_height,
		"Vertical"
	);

	// Horizontal scrollbar display.
	mdata->map_horizontal_scrollbar_output = new Fl_Output(
		x() + mdata->settings.editor.scrollbar_output_width,
		y() + mdata->settings.tab_menu_tab_size + mdata->settings.editor.scrollbar_output_height,
		mdata->settings.editor.scrollbar_output_width,
		mdata->settings.editor.scrollbar_output_height,
		"Horizontal"
	);

	// Config scrollbars.
	mdata->map_vertical_scrollbar->type(FL_HORIZONTAL);
	mdata->map_horizontal_scrollbar->type(FL_HORIZONTAL);

	// Callbacks.
	mdata->map_vertical_scrollbar->callback(
		scrollbar_cb, 
		(void*)mdata->map_vertical_scrollbar_output
	);

	mdata->map_horizontal_scrollbar->callback(
		scrollbar_cb, 
		(void*)mdata->map_horizontal_scrollbar_output
	);

	mdata->map_vertical_scrollbar->when(FL_WHEN_CHANGED);
	mdata->map_horizontal_scrollbar->when(FL_WHEN_CHANGED);

	// Set values to 0.
	mdata->map_vertical_scrollbar_output->value("0");
	mdata->map_horizontal_scrollbar_output->value("0");

	scrollbar_tab->end();

	// Tile creater tab.
	tile_creater_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h, 
		TILE_CREATER_TAB_LABEL
	);

	tile_creater = new EditTileCreater(
		mdata, 
		mdata->map,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	tile_creater_tab->end();

	// Item creater tab.
	item_creater_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h, 
		ITEM_CREATER_TAB_LABEL
	);

	item_creater = new EditItemCreater(
		mdata,
		mdata->map,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	item_creater_tab->end();

	// Item command list tab.
	item_command_list_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h,
		"Item command list"
	);

	item_command_list = new EditItemCommandList(
		mdata,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	item_command_list_tab->end();

	// Npc Creater tab.
	npc_creater_tab = new Fl_Group(
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h,
		NPC_CREATER_TAB_LABEL
	);

	npc_creater = new EditNpcCreater(
		mdata,
		tab_x, 
		tab_y, 
		tab_w, 
		tab_h
	);

	npc_creater_tab->end();

	// If Hidden.
	if (mdata->settings.tab_menu_default_hidden)
		menu_side(MENU_HIDDEN);
	else
		menu_side(menu_locat);

	end();
}

void EditTabMenu::reset_items_size() {
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

	// Scrollbar tab.
	if (scrollbar_tab != NULL)
		scrollbar_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	if (mdata->map_vertical_scrollbar != NULL)
		mdata->map_vertical_scrollbar->resize(
			x(), 
			y() + (h() / 2) - mdata->settings.editor.scrollbar_thickness, 
			w(), 
			mdata->settings.editor.scrollbar_thickness
		);

	if (mdata->map_horizontal_scrollbar != NULL)
		mdata->map_horizontal_scrollbar->resize(
			x(), 
			y() + (h() / 2) + (mdata->settings.editor.scrollbar_thickness * 2), 
			w(), 
			mdata->settings.editor.scrollbar_thickness
		);

	if (mdata->map_vertical_scrollbar_output != NULL)
		mdata->map_vertical_scrollbar_output->resize(
			x() + mdata->settings.editor.scrollbar_output_width,
			y() + mdata->settings.tab_menu_tab_size,
			mdata->settings.editor.scrollbar_output_width,
			mdata->settings.editor.scrollbar_output_height
		);

	if (mdata->map_horizontal_scrollbar_output != NULL)
		mdata->map_horizontal_scrollbar_output->resize(
			x() + mdata->settings.editor.scrollbar_output_width,
			y() + mdata->settings.tab_menu_tab_size + mdata->settings.editor.scrollbar_output_height,
			mdata->settings.editor.scrollbar_output_width,
			mdata->settings.editor.scrollbar_output_height
		);

	// Tile creater tab.
	if (tile_creater_tab != NULL)
		tile_creater_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	if (tile_creater != NULL) {
		tile_creater->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		// tile preview.
		tile_creater->tile_preview->size(
			tile_creater->tile_preview->h(),
			tile_creater->tile_preview->h()
		);
	}

	// Chat box tab.
	if (chat_box_tab != NULL) {
		chat_box_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		((ChatBox*)mdata->chat_box)->reset_size();
	}

	// Chat box.
	if (mdata->chat_box != NULL)
		mdata->chat_box->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Item creater tab.
	if (item_creater_tab != NULL)
		item_creater_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Item creater.
	if (item_creater != NULL) {
		item_creater->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		item_creater->item_preview->size(
			item_creater->item_preview->h(),
			item_creater->item_preview->h()
		);
	}

	// Item command list tab.
	if (item_command_list_tab != NULL)
		item_command_list_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Item command list.
	if (item_command_list != NULL) {
		item_command_list->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		item_command_list->reset_size();
	}

	// Npc creater tab.
	if (npc_creater_tab != NULL)
		npc_creater_tab->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

	// Npc creater.
	if (npc_creater != NULL) {
		npc_creater->resize(
			tab_x, 
			tab_y, 
			tab_w, 
			tab_h
		);

		npc_creater->reset_size();
	}
}

void EditTabMenu::scrollbar_cb(Fl_Widget * w, void * d) {
	Fl_Scrollbar * scrollbar = (Fl_Scrollbar*)w;
	Fl_Output * output = (Fl_Output*)d;
	char output_buf[NAME_MAX];

	// Set scrollbar output.
	snprintf(output_buf, NAME_MAX, "%d", scrollbar->value());
	output->value(output_buf);
}
