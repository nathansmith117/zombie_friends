#pragma once

#include "program_data.h"
#include "tile.h"
#include "game_tools.h"
#include "map.h"

// Constants for tile type in string.
#define TILE_TYPE_STR_MAX NAME_MAX

#define TILE_LOAD_TYPE_BASIC_STR "basic"
#define TILE_LOAD_TYPE_LOADED_STR "loaded"
#define TILE_LOAD_TYPE_ADDON_STR "addon"

class EditTileCreater : public Fl_Group {
	public:
		EditTileCreater(MainData * md, Map * map, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, map, X, Y, W, H);
		}

		int handle(int event);

		Map * get_map() { return map; }
		void set_map(Map * map) { this->map = map; }

		Tile::TileObject tile() { return current_tile; }
		void tile(Tile::TileObject tile);

		// Preview.
		Tile::TilePreview * tile_preview = NULL;
	private:
		MainData * mdata;
		Map * map = NULL;

		Tile::TileObject current_tile;

		Fl_Input * id_choice = NULL;
		Fl_Input * type_choice = NULL;
		Fl_Choice * load_type_choice = NULL;
		Fl_Button * enter_button = NULL;

		void main_init(MainData * md, Map * map, int X, int Y, int W, int H);

		// Callbacks.
		static void id_choice_cb(Fl_Widget * w, void * d);
		static void type_choice_cb(Fl_Widget * w, void * d);
		static void load_type_choice_cb(Fl_Widget * w, void * d);
		static void enter_button_cb(Fl_Widget * w, void * d);
};
