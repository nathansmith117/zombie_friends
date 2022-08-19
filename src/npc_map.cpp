#include "npc_map.h"
#include "evil_puppy.h"

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

	map = NULL;
	width = 0;
	height = 0;
}

int NpcMap::npc(Npc * new_npc, int x, int y) {
	if (map == NULL || x >= width || y >= height)
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
	return 0;
}

int NpcMap::npc(Npc * new_npc) {
	if (new_npc == NULL)
		return -1;

	return npc(new_npc, new_npc->wx(), new_npc->wy());
}

Npc * NpcMap::npc(int x, int y) {
	if (map == NULL || x >= width || y >= height)
		return NULL;

	return map[y][x];
}

int NpcMap::remove(int x, int y) {
	return npc(NULL, x, y);
}

void NpcMap::draw() {
	Fl_PNG_Image * curr_image = NULL;
	bool collided;

	if (npcs_in_use.empty())
		return;

	// Draw npcs.
	for (auto n : npcs_in_use) {
		if (n == NULL)
			continue;

		// Get image. For check if on view window not for drawing.
		curr_image = n->get_current_image();

		if (curr_image == NULL)
			continue;

		// Check if on view window.
		collided = gameTools::did_collide(
			n->x(),
			n->y(),
			curr_image->w(),
			curr_image->h(),
			0,
			0,
			mdata->view_win->w(),
			mdata->view_win->h()
		);

		if (!collided)
			continue;

		// Draw npc.
		n->draw();
	}
}

void NpcMap::update() {
	update_npcs_in_use();
}

void NpcMap::update_npcs_in_use() {
	if (npcs_in_use.empty())
		return;

	// Update npcs.
	for (auto n : npcs_in_use)
		if (n != NULL)
			n->update();

	// Remove last npc if null.
	if (npcs_in_use.back() == NULL)
		npcs_in_use.pop_back();
}

void NpcMap::put_npc_to_use(int x, int y) {
	Npc * curr_npc = npc(x, y);

	if (curr_npc == NULL)
		return;

	// Add to 'npcs_in_use'.
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
}
