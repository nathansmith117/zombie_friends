#include "npc.h"
#include "character.h"
#include "tile.h"
#include "npc_map.h"

void Npc::keep_position() {

	// Location on map pixel not tile).
	int map_pixel_x;
	int map_pixel_y;

	// Map offset.
	int x_offset;
	int y_offset;

	if (mdata->map == NULL)
		return;

	x_offset = mdata->map->offset_x();
	y_offset = mdata->map->offset_y();
}

void Npc::follow_other_char(Character * other_char, float distance) {
	float rise, run;
	float dis;
	float angle;

	if (other_char == NULL)
		return;

	rise = other_char->wy() - world_y;
	run = other_char->wx() - world_x;
	dis = hypotf(run, rise);

	angle = gameTools::rad_to_deg(atan2f(rise, run));
	angle = gameTools::make_non_neg_angle(angle + 90.0);

	//printf("Dis: %f, angle: %f, rise: %f, run: %f\n", dis, angle, rise, run);

	dir = NO_MOVEMENT;

	/*
	if (gameTools::is_in_range<float>(dis, 0.0, distance)) {
		dir = other_char->get_direction();
		return;
	}
	*/

	std::vector<Astar::Point> points;

	points = Astar::find_path(
		mdata->map, 
		{wx_rounded(), wy_rounded()},
		{mdata->player->wx_rounded(), mdata->player->wy_rounded()}
	);

	for (auto p : points)
		printf("%d %d | ", p.x, p.y);

	putchar('\n');
	return;

	// Already there.
	if (dis <= distance)
		return;

	if (gameTools::is_in_range<float>(angle, 0.0, 13.0)) {
		dir.up = true;
	} else if (gameTools::is_in_range<float>(angle, 14.0, 55.0)) {
		dir.right = true;
		dir.up = true;
	} else if (gameTools::is_in_range<float>(angle, 56, 100)) {
		dir.right = true;
	} else if (gameTools::is_in_range<float>(angle, 101, 170)) {
		dir.right = true;
		dir.down = true;
	} else if (gameTools::is_in_range<float>(angle, 171, 190)) {
		dir.down = true;
	} else if (gameTools::is_in_range<float>(angle, 191, 260)) {
		dir.left = true;
		dir.down = true;
	} else if (gameTools::is_in_range<float>(angle, 260, 280)) {
		dir.left = true;
	} else if (gameTools::is_in_range<float>(angle, 281, 350)) {
		dir.left = true;
		dir.up = true;
	} else if (gameTools::is_in_range<float>(angle, 351, 360)) {
		dir.up = true;
	}

	/*
	// Right and left.
	if (run > 0) {
		dir.right = true;
		dir.left = false;
	} else if (run < 0) {
		dir.right = false;
		dir.left = true;
	} else {
		dir.right = false;
		dir.left = false;
	}

	// Up and down.
	if (rise < 0) {
		dir.up = true;
		dir.down = false;
	} else if (rise > 0) {
		dir.up = false;
		dir.down = true;
	} else {
		dir.up = false;
		dir.down = false;
	}
	*/
}
