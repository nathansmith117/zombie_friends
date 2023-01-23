#pragma once

#include "program_data.h"
#include "common_tool.h"
#include "npc_map.h"

// Tools and npcs are stored as ids in .mp file
// but the engine needs the object/class

enum NPC_TYPES {
	NPC_TYPE_NONE = -1,
	NPC_TYPE_EVIL_PUPPY = 0
};

enum TOOL_TYPES {
	TOOL_NONE = -1,
	TOOL_WEAPON,
	TOOL_SWORD,
	TOOL_GUN,
	TOOL_SUBMACHINE_GUN
};

Npc * get_npc_from_type(MainData * mdata, NpcMap * npc_map, NPC_TYPE type);
CommonTool * get_tool_from_type(TOOL_TYPE type, MainData * mdata, Fl_Widget * item_holder=NULL);
