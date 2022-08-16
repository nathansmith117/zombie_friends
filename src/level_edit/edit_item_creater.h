#pragma once

#include "program_data.h"
#include "common_item.h"
#include "game_tools.h"
#include "map.h"

// Constants for item type in string.
#define ITEM_TYPE_STR_MAX NAME_MAX

#define ITEM_LOAD_TYPE_BASIC_STR "basic"
#define ITEM_LOAD_TYPE_LOADED_STR "loaded"
#define ITEM_LOAD_TYPE_ADDON_STR "addon"

class EditItemCreater : public Fl_Group {
	public:
		EditItemCreater(MainData * md, Map * map, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, map, X, Y, W, H);
		}

		int handle(int event);

		Map * get_map() { return map; }
		void set_map(Map * map) { this->map = map; }

		CommonItem::ItemData item() { return current_item; }
		void item(CommonItem::ItemData current_item);

		void reset_command_choice();

		CommonItem::ItemPreview * item_preview = NULL;
	private:
		MainData * mdata;
		Map * map = NULL;

		CommonItem::ItemData current_item;

		std::vector<std::string> old_item_commands;

		Fl_Input * id_choice = NULL;
		Fl_Input * type_choice = NULL;
		Fl_Choice * load_type_choice = NULL;
		Fl_Input * heath_gain_choice = NULL;
		Fl_Input * coin_gain_choice = NULL;
		Fl_Input * tool_gain_choice = NULL;
		Fl_Input * fuel_gain_choice = NULL;
		Fl_Choice * command_choice = NULL;
		Fl_Button * enter_button = NULL;

		void main_init(MainData * md, Map * map, int X, int Y, int W, int H);

		// Callbacks.
		static void id_choice_cb(Fl_Widget * w, void * d);
		static void type_choice_cb(Fl_Widget * w, void * d);
		static void load_type_choice_cb(Fl_Widget * w, void * d);
		static void heath_gain_choice_cb(Fl_Widget * w, void * d);
		static void coin_gain_choice_cb(Fl_Widget * w, void * d);
		static void tool_gain_choice_cb(Fl_Widget * w, void * d);
		static void fuel_gain_choice_cb(Fl_Widget * w, void * d);
		static void command_choice_cb(Fl_Widget * w, void * d);
		static void enter_button_cb(Fl_Widget * w, void * d);
};
