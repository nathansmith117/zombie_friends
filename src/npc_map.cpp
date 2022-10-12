#include "npc_map.h"
#include "evil_puppy.h"

NpcData get_clear_npc_data() {
	int i;
	NpcData npc_data;

	for (i = 0; i < NPC_DATA_TOOLS_SIZE; i++) {
		npc_data.tools[i] = -1;
		npc_data.fuel[i] = 0;
	}

	return npc_data;
}

NpcMap::NpcMap(MainData * md) {
	mdata = md;
	
	map = NULL;

	width = 0;
	height = 0;
}

void NpcMap::map_data(Npc *** map) {
	if (this->map != NULL)
		delete_map();

	this->map = map;
}

int NpcMap::create_map(int width, int height) {
	int x, y;

	// Check map size.
	if (width <= 0 || height <= 0) {
		fputs("Size can't be less than 1\n", stderr);
		return -1;
	}

	if (map != NULL)
		delete_map();

	this->width = width;
	this->height = height;

	// Create map.
	map = gameTools::create_2d_array<Npc*>(width, height);

	if (map == NULL) {
		fputs("Error creating npc map\n", stderr);
		return -1;
	}

	// Fill with null.
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			map[y][x] = NULL;

	// Debug.

	/*
	NpcData npc_data[] = {
		{NPC_TYPE_EVIL_PUPPY, 5, 5},
		{NPC_TYPE_EVIL_PUPPY, 2, 2}
	};

	add_npcs_from_data(npc_data, sizeof(npc_data) / sizeof(NpcData));

	for (auto n : npc_data)
		mdata->map->get_npc_data()->push_back(n);
	*/

	return 0;
}

void NpcMap::delete_map() {
	int x, y;

	if (map == NULL)
		return;

	// Delete npcs.
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			if (map[y][x] != NULL)
				delete map[y][x];

	if (gameTools::delete_2d_array<Npc*>(map, width, height) == -1) {
		fputs("Error deleting npc map\n", stderr);
		return;
	}

	// Delete npcs in use.
	for (auto n : npcs_in_use)
		if (n != NULL)
			delete n;

	npcs_in_use.clear();

	map = NULL;
	width = 0;
	height = 0;
}

void NpcMap::add_npcs_from_data(NpcData * npc_data, size_t n) {
	int i;
	Npc * new_npc = NULL;

	if (npc_data == NULL || n <= 0)
		return;

	for (i = 0; i < n; i++)
		npc(npc_data[i]);
}

int NpcMap::npc(Npc * new_npc, int x, int y) {
	if (map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return -1;

	// Delete npc in map.
	if (map[y][x] != NULL)
		delete map[y][x];

	// Set npc position.
	if (new_npc != NULL) {
		new_npc->wx(x);
		new_npc->wy(y);
	}

	map[y][x] = new_npc;

	// Put npc to use if always updated.
	if (new_npc != NULL)
		if (new_npc->get_always_updated())
			put_npc_to_use(x, y);

	return 0;
}

int NpcMap::npc(Npc * new_npc) {
	if (new_npc == NULL)
		return -1;

	return npc(new_npc, new_npc->wx_rounded(), new_npc->wy_rounded());
}

int NpcMap::npc(NpcData npc_data) {
	int i;

	Npc * new_npc = get_npc_from_type(mdata, this, npc_data.type);
	CommonTool * new_tool = NULL;

	if (new_npc == NULL)
		return -1;

	// Coins and health.
	new_npc->set_coins(npc_data.coins);
	new_npc->set_heath(npc_data.health);

	// Add tools.
	for (i = 0; i < NPC_DATA_TOOLS_SIZE; i++) {
		new_tool = get_tool_from_type(npc_data.tools[i], mdata, new_npc);

		if (new_tool == NULL)
			continue;

		new_tool->set_fuel(npc_data.fuel[i]);
		new_npc->add_tool(new_tool);
	}

	return npc(new_npc, npc_data.x, npc_data.y);
}

Npc * NpcMap::npc(int x, int y) {
	if (map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return NULL;

	return map[y][x];
}

int NpcMap::remove(int x, int y) {
	return npc(NULL, x, y);
}

void NpcMap::draw() {

	if (npcs_in_use.empty())
		return;

	// Draw npcs.
	for (auto n : npcs_in_use) {
		if (n == NULL)
			continue;

		// Check if on view window.
		if (!n->is_on_view_window())
			continue;

		// Draw npc.
		n->draw();
	}
}

void NpcMap::update() {
	int x, y;
	int view_w, view_h;
	int start_x, start_y, end_x, end_y;
	int x_offset, y_offset;

	Npc * curr_npc = NULL;

	update_npcs_in_use();

	// Add more npcs to map.
	if (map == NULL || mdata->map == NULL)
		return;

	view_w = mdata->view_win->w();
	view_h = mdata->view_win->h();

	x_offset = mdata->map->offset_x();
	y_offset = mdata->map->offset_y();

	// Set start and end values.
	start_x = -x_offset / mdata->scale_tile_size - mdata->settings.npc_map_overscan;
	start_y = -y_offset / mdata->scale_tile_size - mdata->settings.npc_map_overscan;
	start_x = (start_x < 0) ? 0 : start_x;
	start_y = (start_y < 0) ? 0 : start_y;

	end_x = (view_w / mdata->scale_tile_size) 
		- ((x_offset / mdata->scale_tile_size) - start_x) + mdata->settings.view_overscan;
	end_y = (view_h / mdata->scale_tile_size) 
		- ((y_offset / mdata->scale_tile_size) - start_y) + mdata->settings.view_overscan;

	end_x = (end_x > width) ? width : end_x;
	end_y = (end_y > height) ? height : end_y;

	// printf("%d %d | %d %d\n", start_x, end_x, start_y, end_y);

	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++) {
			curr_npc = map[y][x];

			if (curr_npc == NULL)
				continue;

			// Out of view.
			if (!curr_npc->is_on_view_window())
				continue;

			put_npc_to_use(x, y);
		}
}

void NpcMap::update_npcs_in_use() {
	int i;

	Npc * curr_npc = NULL;

	if (mdata->view_win == NULL)
		return;

	if (npcs_in_use.empty())
		return;

	// Update npcs.
	for (i = 0; i < npcs_in_use.size(); i++) {
		curr_npc = npcs_in_use[i];

		if (curr_npc == NULL)
			continue;

		// Update.
		curr_npc->update();

		if (curr_npc->get_always_updated())
			continue;

		// Is on view window.
		if (curr_npc->is_on_view_window())
			continue;

		remove_npc_from_use(i);
	}

	// Remove last npc if null.
	if (npcs_in_use.back() == NULL)
		npcs_in_use.pop_back();
}

void NpcMap::put_npc_to_use(int x, int y) {
	Npc * curr_npc = npc(x, y);

	if (curr_npc == NULL)
		return;

	// Refresh images and stuff.
	curr_npc->refresh_images();
	curr_npc->keep_position();
	curr_npc->move_tool_to_location();

	// Add to 'npcs_in_use'.
	curr_npc->set_being_updated(true);
	npcs_in_use.push_back(curr_npc);

	// Remove from map (without deleting).
	map[y][x] = NULL;
}

void NpcMap::remove_npc_from_use(int location) {
	Npc * curr_npc = NULL;

	if (location >= npcs_in_use.size())
		return;

	curr_npc = npcs_in_use[location];

	if (curr_npc == NULL)
		return;

	curr_npc->set_being_updated(false);
	npc(curr_npc);
	npcs_in_use[location] = NULL;
}

void NpcMap::remove_npc_from_use(Npc * unused_npc) {
	int i;

	if (unused_npc == NULL)
		return;
	if (npcs_in_use.empty())
		return;

	for (i = 0; i < npcs_in_use.size(); i++)
		if (npcs_in_use[i] == unused_npc) {
			remove_npc_from_use(i);
			break;
		}
}

void NpcMap::delete_npc_in_use(int location) {
	if (location >= npcs_in_use.size() || location < 0)
		return;
	if (npcs_in_use[location] == NULL)
		return;

	delete npcs_in_use[location];
	npcs_in_use[location] = NULL;
}

void NpcMap::delete_npc_in_use(Npc * unused_npc) {
	int i;
	Npc * curr_npc;

	if (unused_npc == NULL)
		return;
	if (npcs_in_use.empty())
		return;

	for (i = 0; i < npcs_in_use.size(); i++) {
		curr_npc = npcs_in_use[i];

		if (curr_npc == NULL)
			continue;

		if (curr_npc == unused_npc) {
			delete_npc_in_use(i);
			break;
		}
	}
}

bool NpcMap::is_in_use(Npc * the_npc) {
	if (the_npc == NULL)
		return false;
	if (npcs_in_use.empty())
		return false;

	for (auto n : npcs_in_use)
		if (n == the_npc)
			return true;

	return false;
}

void NpcMap::scale_npc_images() {
	int x, y;
	int scale = mdata->settings.scale;
	Npc * curr_npc = NULL;

	mdata->scaled_images.npc.evil_puppy = gameTools::scale_images(mdata->images.npc.evil_puppy, scale);

	// Refresh images.
	if (map == NULL)
		return;

	// Map.
	/*
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++) {
			curr_npc = npc(x, y);

			if (curr_npc == NULL)
				continue;

			curr_npc->refresh_images();
			curr_npc->keep_position();
			curr_npc->move_tool_to_location();
		}
	*/

	// Npcs in use.
	for (auto n : npcs_in_use)
		if (n != NULL) {
			n->refresh_images();
			n->keep_position();
			n->move_tool_to_location();
		}
}

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
