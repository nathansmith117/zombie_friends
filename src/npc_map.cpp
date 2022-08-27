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


	// Debug.
	npc(new EvilPuppy(mdata, this), 5, 5);
	EvilPuppy * evil_puppy = new EvilPuppy(mdata, this);
	evil_puppy->set_always_updated(false);
	npc(evil_puppy, 2, 2);

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
		if (n != NULL) {
			delete n;
			n = NULL;
		}

	npcs_in_use.clear();

	map = NULL;
	width = 0;
	height = 0;
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

	if (new_npc->get_always_updated())
		put_npc_to_use(x, y);

	return 0;
}

int NpcMap::npc(Npc * new_npc) {
	if (new_npc == NULL)
		return -1;

	return npc(new_npc, new_npc->wx_rounded(), new_npc->wy_rounded());
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
	Fl_PNG_Image * curr_image = NULL;

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
	Fl_PNG_Image * npc_image = NULL;

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
	printf("%ld %ld\n", npcs_in_use.size(), clock());

	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++) {
			curr_npc = map[y][x];

			if (curr_npc == NULL)
				continue;

			curr_npc->keep_position(); // Set position for checking if on map.
			npc_image = curr_npc->get_current_image();

			if (npc_image == NULL)
				continue;

			// Out of view.
			if (!curr_npc->is_on_view_window())
				continue;

			put_npc_to_use(x, y);
		}
}

void NpcMap::update_npcs_in_use() {
	int i;
	Fl_PNG_Image * npc_image = NULL;

	Npc * curr_npc = NULL;

	if (mdata->view_win == NULL)
		return;

	if (npcs_in_use.empty())
		return;

	// Update npcs.
	for (i = 0; i < npcs_in_use.size(); i++) {
		curr_npc = npcs_in_use[i];

		// Remove null npcs.
		if (curr_npc == NULL)
			continue;

		// Update.
		curr_npc->update();

		if (curr_npc->get_always_updated())
			continue;

		// Get current npc image for getting width and height.
		npc_image = curr_npc->get_current_image();

		if (npc_image == NULL)
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
