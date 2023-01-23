#include "id_to_object.h"

// Npcs.
#include "evil_puppy.h"

// tools.
#include "sword.h"
#include "submachine_gun.h"

Npc * get_npc_from_type(MainData * mdata, NpcMap * npc_map, NPC_TYPE type) {
	switch (type) {
		case NPC_TYPE_NONE:
			return NULL;
		case NPC_TYPE_EVIL_PUPPY:
			return new EvilPuppy(mdata, npc_map);
		default:
			return NULL;
	}
}

CommonTool * get_tool_from_type(TOOL_TYPE type, MainData * mdata, Fl_Widget * item_holder) {
	switch(type) {
		case TOOL_SWORD:
			return new Sword(mdata, item_holder);
		case TOOL_SUBMACHINE_GUN:
			return new SubmachineGun(mdata, item_holder);
		default:
			return NULL;
	}
}
