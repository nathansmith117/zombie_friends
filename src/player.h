#pragma once

#include "program_data.h"
#include "weapon.h"
#include "map.h"
#include "character.h"
#include "common_tool.h"

enum {
	PLAYER_RIGHT = 0,
	PLAYER_LEFT = 3
};

class Player : public Character {
	public:
		Player(MainData * md) : Character(md) {
			main_init(md);
		}

		void main_init(MainData * md);
		void update();
		void handle_items();

		void center();

		void go_right();
		void go_left();

		bool facing_right();
		bool facing_left();

		int wx() { return world_x; }
		void wx(int world_x);

		int wy() { return world_y; }
		void wy(int world_y);

		void update_player_info();

		// Redraw screen to update update_player_info.
		void next_tool();
		void pre_tool();
	protected:
		int last_call_count;

		// Old values.
		int old_coin_count;
		int old_heath_count;
		int old_fuel_count;
		int old_tool_local;
		int old_tool_count;
};
