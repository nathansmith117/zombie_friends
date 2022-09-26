#include "npc.h"
#include "character.h"
#include "tile.h"
#include "npc_map.h"

int switch_following_direction(int dir) {
	return dir * -1;
}

void Npc::keep_position() {
	CommonTool * curr_tool = NULL;

	if (mdata->map == NULL)
		return;

	x((world_x * mdata->scale_tile_size) + mdata->map->offset_x());
	y((world_y * mdata->scale_tile_size) + mdata->map->offset_y());

	curr_tool = get_current_tool();

	// Set tool position.
	if (curr_tool != NULL) {
		if (facing_right())
			curr_tool->set_right();
		else
			curr_tool->set_left();

		curr_tool->move_to_location();
	}
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

Astar::Point Npc::get_current_point() {
	if (current_point >= points.size() || current_point < 0)
		return {0, 0};

	return points[current_point];
}

bool Npc::at_end_point() {
	switch (following_direction) {
		case FOLLOW_FORWARDS:
			return current_point >= points.size();
		case FOLLOW_BACKWARDS:
			return current_point < 0;
		default:
			return true;
	}
}

bool Npc::follow_points() {
	int rx, ry;
	Astar::Point curr_point;

	rx = wx_rounded();
	ry = wy_rounded();

	dir = NO_MOVEMENT;

	// No points.
	if (points.empty())
		return false;

	// At end.
	if (at_end_point()) {

		// Handling following type.
		switch (following_type) {
			case POINT_FOLLOWING_REPEAT:
				current_point = 0;
				following_direction = FOLLOW_FORWARDS;
				break;
			case POINT_FOLLOWING_BACK_AND_FORTH:
				following_direction = switch_following_direction(following_direction);

				// Wrap 'current_point'.
				current_point = (current_point < 0) ? 0 : current_point;
				current_point = (current_point >= points.size()) ? points.size() - 1 : current_point;
				break;
			case POINT_FOLLOWING_STOP_AT_END:
				break;
		}

		return true;
	}

	curr_point = get_current_point();

	// Next point.
	if (rx == curr_point.x && ry == curr_point.y) {
		current_point += following_direction;
		return false;
	}

	// Right and left.
	if (rx < curr_point.x)
		dir.right = true;
	else if (rx > curr_point.x)
		dir.left = true;

	// Up and down.
	if (ry > curr_point.y)
		dir.up = true;
	else if (ry < curr_point.y)
		dir.down = true;

	return false;
}
