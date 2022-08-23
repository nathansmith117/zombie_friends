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

		float wx() { return world_x; }
		void wx(float world_x);

		float wy() { return world_y; }
		void wy(float world_y);

		void update_player_info();
		void update_map_offset();

		// Redraw screen to update update_player_info.
		void next_tool();
		void pre_tool();
	private:
		void main_init(MainData * md);

		float old_world_x;
		float old_world_y;

		int last_call_count;

		// Old values.
		int old_coin_count;
		int old_heath_count;
		int old_fuel_count;
		int old_tool_local;
		int old_tool_count;
};
