#include "player.h"
#include "character.h"
#include "common_item.h"
#include "game_tools.h"
#include "sword.h"
#include "player_info_display.h"
#include "submachine_gun.h"

void Player::main_init(MainData * md) {
	mdata = md;

	character_images = mdata->scaled_images.player;

	center();
	go_left();

	old_coin_count = 0;
	old_heath_count = 0;

	dir = {false, false, false, false};
	last_call_count = 0;

	world_x = 0;
	world_y = 0;

	wx(mdata->map->get_width() / 2);
	wy(mdata->map->get_height() / 2);
}

bool Player::facing_right() {
	return frame < PLAYER_LEFT;
}

bool Player::facing_left() {
	return frame >= PLAYER_LEFT;
}

void Player::update() {
	CommonTool * tl = get_current_tool();

	update_player_info();

	// This function needs the map.
	if (mdata->map == NULL)
		return;

	last_call_count++;

	handle_items();

	if (tl != NULL)
		tl->update();

	// Move in the world.
	if (dir.right)
		mdata->map->offset_x(mdata->map->offset_x() - (mdata->settings.player_speed * mdata->settings.scale));
	if (dir.left)
		mdata->map->offset_x(mdata->map->offset_x() + (mdata->settings.player_speed * mdata->settings.scale));
	if (dir.up)
		mdata->map->offset_y(mdata->map->offset_y() + (mdata->settings.player_speed * mdata->settings.scale));
	if (dir.down)
		mdata->map->offset_y(mdata->map->offset_y() - (mdata->settings.player_speed * mdata->settings.scale));

	// Update world position.
	world_x = (x() - mdata->map->offset_x()) / mdata->scale_tile_size;
	world_y = (y() - mdata->map->offset_y()) / mdata->scale_tile_size;

	// Slow stuff down.
	if (last_call_count < (int)roundf(mdata->settings.update_fps 
				* mdata->settings.player_update_speed))
		return;
	else
		last_call_count = 0;

	// Update frames.
	if (dir.right || ((dir.up || dir.down) && frame < PLAYER_LEFT)) {

		if (frame == PLAYER_RIGHT + 1)
			frame = PLAYER_RIGHT + 2;
		else
			frame = PLAYER_RIGHT + 1;

	} else if (dir.left || ((dir.up || dir.down) && frame >= PLAYER_LEFT)) {

		if (frame == PLAYER_LEFT + 1)
			frame = PLAYER_LEFT + 2;
		else
			frame = PLAYER_LEFT + 1;
		
	} else {
		
		if (frame >= PLAYER_LEFT)
			frame = PLAYER_LEFT;
		else
			frame = PLAYER_RIGHT;
	}
}

void Player::handle_items() {
	int i;
	std::vector<CharacterHitData> hit_data;

	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	hit_data = map_hit();

	if (hit_data.size() <= 0)
		return;

	for (i = 0; i < hit_data.size(); i++) {
		curr_tile = mdata->map->tile(hit_data[i].coord.x, hit_data[i].coord.y);
		curr_item = mdata->map->item(hit_data[i].coord.x, hit_data[i].coord.y);

		// Hit already handled.
		if (hit_data[i].hit_handled)
			continue;

		// tile.
		if ((hit_data[i].type & HIT_TILE) == HIT_TILE) {
			hit_data[i].hit_handled = true;
		}

		// Item.
		if ((hit_data[i].type & HIT_ITEM) == HIT_ITEM) {
			hit_data[i].hit_handled = true;
			add_item(curr_item);
			mdata->map->remove_item(hit_data[i].coord.x, hit_data[i].coord.y);
		}
	}
}

void Player::center() {
	int win_width, win_height, p_width, p_height;
	Fl_PNG_Image * current_image;

	current_image = get_current_image();

	if (current_image == NULL)
		return;

	win_width = mdata->view_win->w();
	win_height = mdata->view_win->h();

	p_width = current_image->w();
	p_height = current_image->h();

	resize((win_width / 2) - (p_width / 2) , 
			(win_height / 2) - (p_height / 2), 
			p_width, p_height);
}

void Player::go_right() {
	CommonTool * tl = get_current_tool();

	if (frame == PLAYER_LEFT)
		frame = PLAYER_RIGHT;

	dir.right = true;
	dir.left = false;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

void Player::go_left() {
	CommonTool * tl = get_current_tool();

	if (frame == PLAYER_RIGHT)
		frame = PLAYER_LEFT;

	dir.right = false;
	dir.left = true;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

void Player::wx(int world_x) {
	this->world_x = world_x;

	if (mdata->map == NULL)
		return;

	mdata->map->offset_x(-(world_x * mdata->scale_tile_size) + x());
}

void Player::wy(int world_y) {
	this->world_y = world_y;

	if (mdata->map == NULL)
		return;

	mdata->map->offset_y(-(world_y * mdata->scale_tile_size) + y());
}

void Player::update_player_info() {
	int fuel;
	CommonTool * curr_tool;

	curr_tool = get_current_tool();

	if (curr_tool != NULL)
		fuel = curr_tool->get_fuel();
	else
		fuel = 0;

	// Should update.
	if (coins == old_coin_count 
		&& heath == old_heath_count
		&& fuel == old_fuel_count
		&& current_tool == old_tool_local
		&& old_tool_count == tools.size())
		return;

	// Rest old values.
	old_coin_count = coins;
	old_heath_count = heath;
	old_fuel_count = fuel;
	old_tool_local = current_tool;
	old_tool_count = tools.size();

	// Update info display.
	if (mdata->player_info_display != NULL)
		mdata->player_info_display->update();
}

void Player::next_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool < tools.size() - 1)
		current_tool++;
	else
		current_tool = 0;

	set_current_tool_local(current_tool);
	mdata->win->redraw();
}

void Player::pre_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool > 0)
		current_tool--;
	else
		current_tool = tools.size() - 1;

	set_current_tool_local(current_tool);
	mdata->win->redraw();
}
