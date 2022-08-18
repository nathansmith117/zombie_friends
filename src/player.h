#pragma once

#include "program_data.h"
#include "weapon.h"
#include "map.h"
#include "character.h"
#include "common_tool.h"

enum PLAYER_FRAME_DIRECTIONS {
	PLAYER_RIGHT = 0,
	PLAYER_LEFT = 3
};

class Player : public Character {
	public:
		Player(MainData * md) : Character(md) {
			main_init(md);
		}

		void update();
		void handle_items();

		void center();

		void go_right();
		void go_left();

		bool facing_right();
		bool facing_left();

		void refresh_images();

		int wx() { return world_x; }
		void wx(int world_x);

		int wy() { return world_y; }
		void wy(int world_y);

		void update_player_info();

		// Redraw screen to update update_player_info.
		void next_tool();
		void pre_tool();
	private:
		void main_init(MainData * md);

		int old_map_x_offset;
		int old_map_y_offset;

		int last_call_count;

		// Old values.
		int old_coin_count;
		int old_heath_count;
		int old_fuel_count;
		int old_tool_local;
		int old_tool_count;
};
