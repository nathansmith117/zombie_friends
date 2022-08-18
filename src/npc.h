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
};
