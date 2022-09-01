#include "character.h"
#include "npc_map.h"

// Tools/Weapons.
#include "common_tool.h"
#include "gun.h"
#include "sword.h"
#include "submachine_gun.h"

void Character::draw() {
	Fl_PNG_Image * current_image;
	current_image = get_current_image();
	CommonTool * tl = get_current_tool();

	if (current_image == NULL)
		return;

	current_image->draw(x(), y());

	// Draw weapon.
	if (tl == NULL)
		return;

	tl->draw();
}

void Character::images(std::vector<Fl_PNG_Image*> character_images) {
	gameTools::delete_image_list(&this->character_images);

	this->character_images = character_images;
	size(character_images[0]->w(), character_images[0]->h());
}

Fl_PNG_Image * Character::get_current_image() {
	if (frame >= character_images.size() || frame < 0)
		return NULL;

	return character_images[frame];
}

void Character::refresh_tool_images() {
	for (auto tl : tools)
		if (tl != NULL)
			tl->refresh_images();
}

bool Character::is_moving() {
	return dir.right || dir.left || dir.up || dir.down;
}

bool Character::hit_tile(Tile::TileObject tile, int x, int y) {
	int tile_x, tile_y;
	int char_x, char_y;

	Fl_PNG_Image * char_image = get_current_image();
	Fl_PNG_Image * tile_image = Tile::get_image(tile, mdata);

	if (char_image == NULL || tile_image == NULL || mdata->map == NULL)
		return false;

	// Get item x and y.
	tile_x = x * mdata->scale_tile_size;
	tile_y = y * mdata->scale_tile_size;

	// Character x and y.
	char_x = world_x * mdata->scale_tile_size;
	char_y = world_y * mdata->scale_tile_size;

	return gameTools::did_collide(
		char_x,
		char_y,
		char_image->w(),
		char_image->h(),
		tile_x,
		tile_y,
		tile_image->w(),
		tile_image->h()
	);
}

bool Character::hit_item(CommonItem::ItemData item, int x, int y) {
	int item_x, item_y;
	int char_x, char_y;

	Fl_PNG_Image * char_image = get_current_image();
	Fl_PNG_Image * item_image = CommonItem::get_image(item, mdata);

	// Check if images are NULL.
	if (char_image == NULL || item_image == NULL || mdata->map == NULL)
		return false;

	// Get item x and y.
	item_x = x * mdata->scale_tile_size;
	item_y = y * mdata->scale_tile_size;

	// Character x and y.
	char_x = world_x * mdata->scale_tile_size;
	char_y = world_y * mdata->scale_tile_size;

	return gameTools::did_collide(
		char_x,
		char_y,
		char_image->w(),
		char_image->h(),
		item_x,
		item_y,
		item_image->w(),
		item_image->h()
	);
}

bool Character::hit_character(Character * character) {
	if (character == NULL)
		return false;

	Fl_PNG_Image * curr_image = get_current_image();
	Fl_PNG_Image * char_image = character->get_current_image();

	// Check if images are NULL.
	if (curr_image == NULL || char_image == NULL)
		return false;

	return gameTools::did_collide(
		x(),
		y(),
		curr_image->w(),
		curr_image->h(),
		character->x(),
		character->y(),
		char_image->w(),
		char_image->h()
	);
}

std::vector<CharacterHitData> Character::get_hit_data() {
	Fl_PNG_Image * current_image;
	std::vector<CharacterHitData> hits_data;
	int x, y;
	
	int start_x, start_y, end_x, end_y;
	int char_tile_w, char_tile_h;
	int map_w, map_h;

	CharacterHitData curr_hit_data;
	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	current_image = get_current_image();

	if (mdata->map == NULL || current_image == NULL)
		return hits_data;

	// Get item map width and height.
	map_w = mdata->map->get_width();
	map_h = mdata->map->get_height();

	// Get character tile width and height.
	char_tile_w = (int)roundf((float)current_image->w() / mdata->scale_tile_size);
	char_tile_h = (int)roundf((float)current_image->h() / mdata->scale_tile_size);

	// Get start and end locations.
	start_x = wx_rounded() - mdata->settings.map_search_overscan;
	start_y = wy_rounded() - mdata->settings.map_search_overscan;
	end_x = wx_rounded() + char_tile_w + (mdata->settings.map_search_overscan + char_tile_w);
	end_y = wy_rounded() + char_tile_h + (mdata->settings.map_search_overscan + char_tile_h);

	// Check values.
	start_x = (start_x < 0) ? 0 : start_x;
	start_y = (start_y < 0) ? 0 : start_y;
	end_x = (end_x > map_w) ? map_w : end_x;
	end_y = (end_y > map_h) ? map_h : end_y;

	//printf("%d %d | %d %d\n", start_x, end_x, start_y, end_y);

	// Find tiles/items hitting character.
	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++) {
			curr_tile = mdata->map->tile(x, y);
			curr_item = mdata->map->item(x, y);

			// Set type and coord.
			curr_hit_data.type = HIT_NONE;
			curr_hit_data.coord = {(int)x, (int)y};

			// Tile.
			if (hit_tile(curr_tile, x, y) && curr_tile.id != Tile::NONE) {
				curr_hit_data.things_hit.tile = curr_tile;
				curr_hit_data.type |= HIT_TILE;
			}

			// Item.
			if (hit_item(curr_item, x, y) && curr_item.id != CommonItem::NONE) {
				curr_hit_data.things_hit.item = curr_item;
				curr_hit_data.type |= HIT_ITEM;
			}

			// If something hits.
			if (curr_hit_data.type != HIT_NONE)
				hits_data.push_back(curr_hit_data);
		}

	// Npcs.
	for (auto n : mdata->map->get_npc_map()->get_npcs_in_use()) {
		if (n == NULL)
			continue;
		if (n == this)
			continue;

		if (hit_character(n)) {
			curr_hit_data.coord = {n->wx_rounded(), n->wy_rounded()};
			curr_hit_data.things_hit.character = n;
			curr_hit_data.type = HIT_CHARACTER;
			hits_data.push_back(curr_hit_data);
			break;
		}
	}

	// Hits player.
	if (mdata->player == NULL || mdata->player == this)
		return hits_data;

	if (hit_character(mdata->player)) {
		curr_hit_data.coord = {mdata->player->wx_rounded(), mdata->player->wy_rounded()};
		curr_hit_data.things_hit.character = mdata->player;
		curr_hit_data.type = HIT_CHARACTER;
		hits_data.push_back(curr_hit_data);
	}

	return hits_data;
}

void Character::add_item(CommonItem::ItemData item) {
	bool tool_added = false;
	int tool_id;

	if (item.id == CommonItem::NONE)
		return;

	add_heath(item.heath_gain);
	add_coins(item.coin_gain);

	// Run item command.
	if (item.command_location != CommonItem::NONE)
		mdata->map->run_item_command(item);

	// Gain Tool.
	tool_id = tool_owned(item.gain_tool);

	// Tool already owned.
	if (tool_id != -1) {
		tools[tool_id]->add_fuel(item.gain_fuel);
		return;
	}

	switch (item.gain_tool) {
		case TOOL_NONE:
			return;
		case TOOL_SWORD:
			tools.push_back(new Sword(mdata, this));
			tool_added = true;
			break;
		case TOOL_SUBMACHINE_GUN:
			tools.push_back(new SubmachineGun(mdata, this));
			tool_added = true;
			break;
		default:
			break;
	}

	if (!tool_added)
		return;

	tools.back()->add_fuel(item.gain_fuel);

	// Set direction.
	if (facing_right())
		tools.back()->set_right();
	else
		tools.back()->set_left();

	tools.back()->move_to_location();
}

int Character::insert_tool(CommonTool * tl, int pos) {
	if (pos >= tools.size() || pos < 0)
		return -1;

	tools[pos] = tl;
	return 0;
}

void Character::set_current_tool_local(int current_tool) {
	this->current_tool = current_tool;

	// Direction.
	if (facing_right())
		tools[current_tool]->set_right();
	else
		tools[current_tool]->set_left();

	tools[current_tool]->move_to_location();
}

CommonTool * Character::get_tool_at(int pos) {
	if (pos >= tools.size() || pos < 0)
		return NULL;

	return tools[pos];
}

CommonTool * Character::get_current_tool() {
	if (current_tool >= tools.size() || current_tool < 0)
		return NULL;

	return tools[current_tool];
}

void Character::next_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool < tools.size() - 1)
		current_tool++;
	else
		current_tool = 0;

	set_current_tool_local(current_tool);
}

void Character::pre_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool > 0)
		current_tool--;
	else
		current_tool = tools.size() - 1;

	set_current_tool_local(current_tool);
}

void Character::use_tool() {
	CommonTool * tl = get_current_tool();

	if (tl == NULL)
		return;

	tl->use();
}

void Character::stop_using_tool() {
	CommonTool * tl = get_current_tool();

	if (tl == NULL)
		return;

	tl->stop_using();
}

int Character::tool_owned(int tool_type) {
	int i;

	for (i = 0; i < tools.size(); i++)
		if (tools[i]->get_type() == tool_type)
			return i;

	return -1;
}
