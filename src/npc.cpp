#include "npc.h"
#include "character.h"
#include "tile.h"
#include "npc_map.h"

void Npc::keep_position() {
	if (mdata->map == NULL)
		return;

	x((world_x * mdata->scale_tile_size) + mdata->map->offset_x());
	y((world_y * mdata->scale_tile_size) + mdata->map->offset_y());
}
