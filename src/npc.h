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

enum NPC_TYPES {
	NPC_TYPE_NONE = -1,
	NPC_TYPE_EVIL_PUPPY = 0
};

typedef int8_t NPC_TYPE;

class Npc : public Character {
	public:
		Npc(MainData * md, class NpcMap * npc_map) : Character(md) {
			this->npc_map = npc_map;

			type = NPC_TYPE_NONE;

			always_updated = false;
			being_updated = false;
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
	protected:
		class NpcMap * npc_map;

		NPC_TYPE type;

		Character * leader = NULL;

		bool being_updated;

		// Update npc even when not shown in view window,
		bool always_updated;
};
