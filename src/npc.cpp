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

bool Npc::is_on_view_window() {
	if (mdata->view_win == NULL)
		return false;

	keep_position();

	return gameTools::did_collide(
		x(),
		y(),
		get_width(),
		get_height(),
		0,
		0,
		mdata->view_win->w(),
		mdata->view_win->h()
	);
}
