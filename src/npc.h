#pragma once

#include "program_data.h"
#include "game_tools.h"
#include "character.h"
#include "weapon.h"
#include "gun.h"

#include "map.h"
#include "common_tool.h"
#include "common_item.h"
#include "tile.h"
#include "astar_pathfinder.h"


#define FOLLOW_DATA_FILE_HEADER "NPC_FOLLOWING_DATA"

enum NPC_TYPES {
	NPC_TYPE_NONE = -1,
	NPC_TYPE_EVIL_PUPPY = 0
};

typedef int8_t NPC_TYPE;

enum POINT_FOLLOWING_MODES {
	POINT_FOLLOWING_REPEAT,
	POINT_FOLLOWING_BACK_AND_FORTH,
	POINT_FOLLOWING_STOP_AT_END
};

enum FOLLOWING_DIRECTIONS {
	FOLLOW_FORWARDS = 1,
	FOLLOW_BACKWARDS = -1
};

typedef int8_t FOLLOWING_TYPE;

int switch_following_direction(int dir);

class Npc : public Character {
	public:
		Npc(MainData * md, class NpcMap * npc_map) : Character(md) {
			this->npc_map = npc_map;

			type = NPC_TYPE_NONE;

			always_updated = false;
			being_updated = false;

			current_point = 0;
			following_type = POINT_FOLLOWING_STOP_AT_END;
			following_direction = FOLLOW_FORWARDS;
		}

		bool get_always_updated() { return always_updated; }
		void set_always_updated(bool always_updated) { this->always_updated = always_updated; }

		virtual bool get_being_updated() { return being_updated; }
		virtual void set_being_updated(bool being_updated) { this->being_updated = being_updated; }

		void keep_position();

		void set_leader(Character * leader) { this->leader = leader; }
		Character * get_leader() { return leader; }

		virtual bool is_on_view_window();

		NPC_TYPE get_type() { return type; }
		void set_type(NPC_TYPE type) { this->type = type; }

		std::vector<Astar::Point> get_points() { return points; }
		void set_points(std::vector<Astar::Point> points) { this->points = points; }

		Astar::Point get_current_point();

		int get_current_point_locat() { return current_point; }
		void set_current_point_locat(int current_point) { this->current_point = current_point; }

		bool at_end_point();

		bool follow_points(); // Returns is at end.

		int load_follow_data_from_file(const char * file_path);
	protected:
		class NpcMap * npc_map;

		// Npc path.
		std::vector<Astar::Point> points;
		int current_point;
		FOLLOWING_TYPE following_type;
		int following_direction;

		NPC_TYPE type;

		Character * leader = NULL;

		bool being_updated;

		// Update npc even when not shown in view window,
		bool always_updated;
};
