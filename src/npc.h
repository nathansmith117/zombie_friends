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

class Npc : public Character {
	public:
		Npc(MainData * md, class NpcMap * npc_map) : Character(md) {
			this->npc_map = npc_map;
			always_updated = false;
			being_updated = false;
		}

		bool get_always_updated() { return always_updated; }
		void set_always_updated(bool always_updated) { this->always_updated = always_updated; }

		virtual bool get_being_updated() { return being_updated; }
		virtual void set_being_updated(bool being_updated) { this->being_updated = being_updated; }

		// Put this in update function.
		virtual void keep_position();

		virtual bool is_on_view_window();
	protected:
		class NpcMap * npc_map;

		bool being_updated;

		// Update npc even when not shown in view window,
		bool always_updated;
};
