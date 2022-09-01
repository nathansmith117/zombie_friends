#include "evil_puppy.h"
#include "npc_map.h"
#include "player.h"

EvilPuppy::~EvilPuppy() {
	delete path_finder;
}

void EvilPuppy::main_init(MainData * md) {
	mdata = md;
	
	refresh_images();

	old_world_x = 0.0;
	old_world_y = 0.0;

	// Config.
	always_updated = true;

	path_finder = new Astar::PathFinder(mdata);
	path_finder->set_character(this);
	path_finder->set_target(mdata->player);

	Astar::PathFinderSettings pathfinder_settings;
	pathfinder_settings.dis_intel_update = 1.0;
	pathfinder_settings.safe_zone_width = 2;
	pathfinder_settings.safe_zone_height = 1;
	pathfinder_settings.character_is_npc = true;
	pathfinder_settings.thread_speed = 10;
	// pathfinder_settings.try_after_path_failed = true;

	path_finder->set_settings(pathfinder_settings);
	path_finder->start_thread();
}

void EvilPuppy::update() {

	if (mdata->player == NULL)
		return;

	handle_hit_data();

	last_call_count++;

	//path_finder->update();
	path_finder->set_target(mdata->player);

	float a_speed = (float)mdata->settings.player_speed / 2 * mdata->settings.scale;

	if (dir.right)
		world_x += a_speed;
	if (dir.left)
		world_x -= a_speed;
	if (dir.up)
		world_y -= a_speed;
	if (dir.down)
		world_y += a_speed;

	keep_position();

	// Slow stuff down.
	if (last_call_count < (int)roundf(mdata->settings.update_fps 
				* mdata->settings.player_update_speed))
		return;
	else
		last_call_count = 0;

	// Update frames.
	if (is_moving()) {
		if (dir.right || (!dir.left && facing_right())) {

			if (frame == EVIL_PUPPY_RIGHT + 1)
				frame = EVIL_PUPPY_RIGHT + 2;
			else
				frame = EVIL_PUPPY_RIGHT + 1;

		} else if (dir.left || (!dir.right && facing_left())) {

			if (frame == EVIL_PUPPY_LEFT + 1)
				frame = EVIL_PUPPY_LEFT + 2;
			else
				frame = EVIL_PUPPY_LEFT + 1;
		}
	} else {
		if (facing_right())
			frame = EVIL_PUPPY_RIGHT;
		else
			frame = EVIL_PUPPY_LEFT;
	}
}

void EvilPuppy::handle_hit_data() {
	int i;
	std::vector<CharacterHitData> hit_data;

	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	hit_data = get_hit_data();

	if (hit_data.empty())
		return;

	for (i = 0; i < hit_data.size(); i++) {
		curr_tile = mdata->map->tile(hit_data[i].coord.x, hit_data[i].coord.y);
		curr_item = mdata->map->item(hit_data[i].coord.x, hit_data[i].coord.y);

		// Hit already handled.
		if (hit_data[i].hit_handled)
			continue;

		// Character/npc.
		if ((hit_data[i].type & HIT_CHARACTER) == HIT_CHARACTER) {
			hit_data[i].hit_handled = true;
			
			//if (dir != old_direction) {
				wx(old_world_x);
				wy(old_world_y);
			//}
		}
	}

	old_world_x = world_x;
	old_world_y = world_y;
	old_direction = dir;
}

void EvilPuppy::go_right() {
	CommonTool * tl = get_current_tool();

	if (frame == EVIL_PUPPY_LEFT)
		frame = EVIL_PUPPY_RIGHT;

	dir.right = true;
	dir.left = false;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

void EvilPuppy::go_left() {
	CommonTool * tl = get_current_tool();

	if (frame == EVIL_PUPPY_RIGHT)
		frame = EVIL_PUPPY_LEFT;

	dir.right = false;
	dir.left = true;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

bool EvilPuppy::facing_right() {
	return frame < EVIL_PUPPY_LEFT;
}

bool EvilPuppy::facing_left() {
	return frame >= EVIL_PUPPY_LEFT;
}

void EvilPuppy::refresh_images() {
	images(gameTools::copy_image_list(mdata->scaled_images.npc.evil_puppy));
	refresh_tool_images();
}
