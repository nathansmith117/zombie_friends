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

class Npc : public Character {
	public:
		Npc(MainData * md) : Character(md) {}

		bool get_always_updated() { return always_updated; }
		void set_always_updated(bool always_updated) { this->always_updated = always_updated; }
	protected:

		// Update npc even when not shown in view window,
		bool always_updated = false;
};
