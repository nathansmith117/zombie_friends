#include "character.h"
#include "npc_map.h"

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

int Character::get_width() {
	Fl_PNG_Image * curr_image = get_current_image();

	if (curr_image == NULL)
		return 0;

	return curr_image->w();
}

int Character::get_height() {
	Fl_PNG_Image * curr_image = get_current_image();

	if (curr_image == NULL)
		return 0;

	return curr_image->h();
}

bool Character::is_moving() {
	return dir.right || dir.left || dir.up || dir.down;
}

bool Character::hit_tile(Tile::TileObject tile, int x, int y) {
	int tile_x, tile_y;
	int char_x, char_y;

	Fl_PNG_Image * tile_image = Tile::get_image(tile, mdata);

	if (tile_image == NULL || mdata->map == NULL)
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
		get_width(),
		get_height(),
		tile_x,
		tile_y,
		tile_image->w(),
		tile_image->h()
	);
}

bool Character::hit_item(CommonItem::ItemData item, int x, int y) {
	int item_x, item_y;
	int char_x, char_y;

	Fl_PNG_Image * item_image = CommonItem::get_image(item, mdata);

	if (item_image == NULL || mdata->map == NULL)
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
		get_width(),
		get_height(),
		item_x,
		item_y,
		item_image->w(),
		item_image->h()
	);
}

bool Character::hit_character(Character * character) {
	if (character == NULL)
		return false;

	return gameTools::did_collide(
		x(),
		y(),
		get_width(),
		get_height(),
		character->x(),
		character->y(),
		character->get_width(),
		character->get_height()
	);
}

std::vector<CharacterHitData> Character::get_hit_data() {
	std::vector<CharacterHitData> hits_data;
	int x, y;
	
	int start_x, start_y, end_x, end_y;
	int char_tile_w, char_tile_h;
	int map_w, map_h;

	int char_width, char_height;

	char_width = get_width();
	char_height = get_height();

	CharacterHitData curr_hit_data;
	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	if (mdata->map == NULL)
		return hits_data;

	// Get item map width and height.
	map_w = mdata->map->get_width();
	map_h = mdata->map->get_height();

	// Get character tile width and height.
	char_tile_w = (int)roundf((float)char_width / mdata->scale_tile_size);
	char_tile_h = (int)roundf((float)char_height / mdata->scale_tile_size);

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
	CommonTool * tool_gained = NULL;
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

	// Gain tool.
	tool_gained = get_tool_from_type(item.gain_tool, mdata, this);

	if (tool_gained == NULL)
		return;

	tool_gained->add_fuel(item.gain_fuel);

	// Set direction.
	if (facing_right())
		tool_gained->set_right();
	else
		tool_gained->set_left();

	tool_gained->move_to_location();
	tools.push_back(tool_gained);
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

void Character::handle_collision() {
	wx(old_world_x);
	wy(old_world_y);
}

void Character::handle_collision(Character * character) {
	gameTools::Direction * char_dir = NULL;

	if (character == NULL)
		return;

	char_dir = character->direction();

	if (!is_moving())
		return;

	if (!character->is_moving())
		dir = NO_MOVEMENT;

	if ((char_dir->right && dir.right)
		|| (char_dir->left && dir.left)
		|| (char_dir->up && dir.up)
		|| (char_dir->down && dir.down)) {
		dir = NO_MOVEMENT;
		return;
	}

	wx(old_world_x);
	wy(old_world_y);
}

void Character::update_world_position(float speed) {
	update_old_values();

	if (dir.right)
		world_x += speed;
	if (dir.left)
		world_x -= speed;
	if (dir.up)
		world_y -= speed;
	if (dir.down)
		world_y += speed;
}

void Character::update_old_values() {
	old_world_x = world_x;
	old_world_y = world_y;
	old_direction = dir;
}
