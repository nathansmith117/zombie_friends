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
	Fl_PNG_Image * curr_image = NULL;

	if (mdata->view_win == NULL)
		return false;

	keep_position();
	curr_image = get_current_image();

	if (curr_image == NULL)
		return false;

	return gameTools::did_collide(
		x(),
		y(),
		curr_image->w(),
		curr_image->h(),
		0,
		0,
		mdata->view_win->w(),
		mdata->view_win->h()
	);
}
