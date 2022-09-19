#include "map.h"
#include "common_item.h"
#include "game_tools.h"
#include "chat_box.h"
#include "npc_map.h"

Map::Map(MainData * md) {
	mdata = md;

	width = 0;
	height = 0;

	tile_map = NULL;
	item_map = NULL;

	x_offset = 0;
	y_offset = 0;

	memset(file_path, 0, NAME_MAX);

	npc_map = new NpcMap(mdata);

	progress = new ProgressBar(mdata);
}

Map::~Map() {
	delete_map();
	delete npc_map;
}

int Map::create_map(int width, int height) {
	int x, y;

	// Check map size.
	if (width <= 0 || height <= 0) {
		fputs("Size can't be less than 1\n", stderr);
		return -1;
	}

	// Delete if not deleted.
	if (tile_map != NULL || item_map != NULL)
		delete_map();

	this->width = width;
	this->height = height;

	// Create tile map.
	tile_map = gameTools::create_2d_array<Tile::TileObject>(width, height);

	// Create item map.
	item_map = gameTools::create_2d_array<CommonItem::ItemData>(width, height);

	if (tile_map == NULL || item_map == NULL) {
		fputs("Error creating map\n", stderr);
		return -1;
	}

	// Fill item map with none id items.
	CommonItem::ItemData fill_item;
	fill_item.id = CommonItem::NONE;

	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			item(fill_item, x, y);

	// Fill tile map with null tiles.
	Tile::TileObject null_tile;
	null_tile.id = Tile::NULL_TILE;
	null_tile.type = Tile::NONE;
	null_tile.load_type = Tile::BASIC;

	fill_with(null_tile);
	printf("New map created width: %d, height: %d\n", width, height);
	gameTools::set_scrollbar_bounds(mdata, width, height);

	// Create npc map.
	if (npc_map->create_map(width, height) == -1)
		return -1;

	return 0;
}

void Map::delete_map() {
	if (tile_map == NULL)
		return;

	int res = gameTools::delete_2d_array<Tile::TileObject>(tile_map, width, height);

	if (res == -1) {
		fputs("Error deleting tile map\n", stderr);
		return;
	}

	res = gameTools::delete_2d_array<CommonItem::ItemData>(item_map, width, height);

	if (res == -1) {
		fputs("Error deleting item map\n", stderr);
		return;
	}

	tile_map = NULL;
	item_map = NULL;
	width = 0;
	height = 0;

	npc_data.clear();

	// Delete npc map.
	npc_map->delete_map();

	puts("Map deleted");
}

Tile::TileObject Map::tile(int x, int y) {
	if (tile_map == NULL || x >= width || y >= height || x < 0 || y < 0) {
		Tile::TileObject tile_obj;
		tile_obj.id = Tile::NONE;
		return tile_obj;
	}

	return tile_map[y][x];
}

int Map::tile(Tile::TileObject new_tile, int x, int y) {
	if (tile_map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return -1;

	tile_map[y][x] = new_tile;
	return 0;
}

int Map::remove_tile(int x, int y) {
	if (tile_map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return -1;

	tile_map[y][x].id = Tile::NONE;
	return 0;
}

int Map::item(CommonItem::ItemData new_item, int x, int y) {
	if (item_map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return -1;

	item_map[y][x] = new_item;
	return 0;
}

CommonItem::ItemData Map::item(int x, int y) {
	if (item_map == NULL || x >= width || y >= height || x < 0 || y < 0) {
		CommonItem::ItemData item_obj;
		item_obj.id = CommonItem::NONE;
		return item_obj;
	}

	return item_map[y][x];
}

int Map::remove_item(int x, int y) {
	if (item_map == NULL || x >= width || y >= height || x < 0 || y < 0)
		return -1;

	CommonItem::ItemData none_item;
	none_item.id = CommonItem::NONE;

	item_map[y][x] = none_item;
	return 0;
}

void Map::add_item_commands(std::vector<std::string> new_items) {
	for (auto i : new_items)
		item_commands.push_back(i);
}

int Map::fill_with(Tile::TileObject full_tile) {
	int x, y;

	if (tile_map == NULL)
		return -1;

	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			if (tile(full_tile, x, y) == -1)
				return -1;

	return 0;
}

int Map::fill_with(Tile::TileObject full_tile, int start_x, int start_y, int end_x, int end_y) {
	int x, y;
	int rend_x, rend_y;

	if (tile_map == NULL)
		return -1;

	// Check start values.
	if (start_x < 0 || start_x >= width)
		return -1;
	else if (start_y < 0 || start_y >= height)
		return -1;

	// If end value is -1 go to end of map array.
	rend_x = (end_x == -1) ? width : end_x;
	rend_y = (end_y == -1) ? height : end_y;

	// Check end values.
	if (rend_x <= 0 || end_x > width)
		return -1;
	else if (rend_y <= 0 || end_y > height)
		return -1;

	// Check if start values are bigger then end values.
	if (start_x >= rend_x)
		return -1;
	else if (start_y >= rend_x)
		return -1;

	// Set tile.
	for (y = start_y; y < rend_y; y++)
		for (x = start_x; x < rend_x; x++)
			if (tile(full_tile, x, y) == -1)
				return -1;

	return 0;
}

void Map::draw() {
	int x, y, draw_x = 0, draw_y = 0;
	int view_x, view_y, view_w, view_h;
	int start_x, start_y, end_x, end_y;

	if (tile_map == NULL)
		return;

	view_w = mdata->view_win->w();
	view_h = mdata->view_win->h();

	// Set start and end values for drawing.
	start_x = -x_offset / mdata->scale_tile_size - mdata->settings.view_overscan;
	start_y = -y_offset / mdata->scale_tile_size - mdata->settings.view_overscan;
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
			// Draw tile.
			Tile::draw_tile(
				mdata, 
				tile_map[y][x], 
				(x * mdata->scale_tile_size) + x_offset, 
				(y * mdata->scale_tile_size) + y_offset
			);

			// Draw item.
			CommonItem::draw_item(mdata, item_map[y][x], x, y);
		}

	// Draw npcs.
	npc_map->draw();
}

int Map::set_file_path(const char * fpath, size_t n) {
	if (fpath == NULL)
		return -1;

	memset(file_path, 0, NAME_MAX);
	strncat(file_path, fpath, n);
	return 0;
}

int Map::get_file_path(char * fpath, size_t n) {
	if (fpath == NULL)
		return -1;
	if (n < NAME_MAX)
		return -1;

	memset(fpath, 0, n);
	strncat(fpath, file_path, n);
	return 0;
}

int Map::load(const char * file_path, size_t n) {
	int res = 0;
	FILE * fp = NULL;

	uint32_t tile_object_size = (uint32_t)sizeof(Tile::TileObject);
	uint32_t item_object_size = (uint32_t)sizeof(ItemDataWithCoord);
	uint32_t npc_object_size = (uint32_t)sizeof(NpcData);

	MapFileHeader header;

	size_t map_size;
	Tile::TileObject * tile_data = NULL;

	size_t item_data_size;
	ItemDataWithCoord * item_data = NULL;

	ItemDataWithCoord curr_item;

	size_t command_list_size;
	char * command_list = NULL;

	size_t npc_list_size;
	NpcData * npc_list = NULL;

	Npc * curr_npc = NULL;
	NpcData curr_npc_data;

	long int i = 0;
	int x, y;

	clear_item_commands();

	// Open file.
	fp = fopen(file_path, "rb");

	if (fp == NULL) {
		fprintf(stderr, "Error opening %s\n", file_path);
		return -1;
	}

	// Load header.
	if (fread(&header, sizeof(MapFileHeader), 1, fp) == -1) {
		fprintf(stderr, "Error loading header from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Check magic number.
	if (header.magic_number != MAP_MAGIC_NUM) {
		fprintf(stderr, "Incorrect magic number in %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Check tile object size.
	if (header.tile_object_size != tile_object_size) {
		fprintf(stderr, "Tile object size is not correct. current: %u, correct %u\n", 
				header.tile_object_size, tile_object_size
		);
		res = -1;
		goto clean_mem;
	}

	// Check item object size.
	if (header.item_object_size != item_object_size) {
		fprintf(stderr, "Item object size is not correct. current: %u, correct %u\n", 
				header.item_object_size, item_object_size
		);
		res = -1;
		goto clean_mem;
	}

	// Check npc object size.
	if (header.npc_object_size != npc_object_size) {
		fprintf(stderr, "Npc object size is not correct. current: %u, correct %u\n", 
				header.npc_object_size, npc_object_size
		);
		res = -1;
		goto clean_mem;
	}
		
	// Init map.
	if (create_map((int)header.w, (int)header.h) == -1) {
		res = -1;
		goto clean_mem;
	}

	set_file_path(file_path, n);

	// Get command list.
	command_list_size = header.command_list_size;

	if (!command_list_size) // Skip getting command list.
		goto skip_getting_command_list;

	// Get memory.
	command_list = new char[command_list_size];

	if (command_list == NULL) {
		fputs("Error getting memory for 'command_list'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	memset(command_list, 0, command_list_size);

	if (fread(command_list, sizeof(char), command_list_size, fp) == -1) {
		fprintf(stderr, "Error loading command list from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Set 'item_commands'.
	item_commands.clear();

	x = 0;
	item_commands.push_back(std::string());

	for (i = 0; i < command_list_size - 1; i++) {
		if (command_list[i] == '\0') { // Next item.
			x++;
			item_commands.push_back(std::string());
			continue;
		}

		item_commands[x] += command_list[i];
	}

skip_getting_command_list:

	// Create tile data.
	map_size = header.w * header.h;
	tile_data = new Tile::TileObject[map_size];

	if (tile_data == NULL) {
		fputs("Error getting memory for 'tile_data'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	// Create item data.
	item_data_size = header.item_data_size;

	if (item_data_size <= 0)
		goto skip_creating_item_data;

	item_data = new ItemDataWithCoord[item_data_size];

	if (item_data == NULL) {
		fputs("Error getting memory for 'item_data'\n", stderr);
		res = -1;
		goto clean_mem;
	}

skip_creating_item_data:

	npc_list_size = header.npc_list_size;

	if (npc_list_size <= 0)
		goto skip_creating_npc_list;

	npc_list = new NpcData[npc_list_size];

	if (npc_list == NULL) {
		fputs("Error getting memory for 'npc_list'\n", stderr);
		res = -1;
		goto clean_mem;
	}

skip_creating_npc_list:

	// Load tiles.
	if (fread(tile_data, tile_object_size, map_size, fp) == -1) {
		fprintf(stderr, "Error loading tile data from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Load items.
	if (item_data != NULL)
		if (fread(item_data, item_object_size, item_data_size, fp) == -1) {
			fprintf(stderr, "Error loading item data from %s\n", file_path);
			res = -1;
			goto clean_mem;
		}

	// Load npcs.
	if (npc_list != NULL)
		if (fread(npc_list, npc_object_size, npc_list_size, fp) == -1) {
			fprintf(stderr, "Error loading npc list from %s\n", file_path);
			res = -1;
			goto clean_mem;
		}

	// Add tiles.
	i = 0;

	for (y = 0; y < header.h; y++)
		for (x = 0; x < header.w; x++) {
			tile(tile_data[i], x, y);
			i++;
		}

	// Add items.
	if (item_data != NULL)
		for (i = 0; i < item_data_size; i++) {
			curr_item = item_data[i];
			item(curr_item.item, curr_item.x, curr_item.y);
		}

	// Add npcs.
	if (npc_list != NULL) {
		npc_map->add_npcs_from_data(npc_list, npc_list_size);

		for (i = 0; i < npc_list_size; i++)
			npc_data.push_back(npc_list[i]);
	}

	printf("Map loaded from %s\n", file_path);
	gameTools::set_scrollbar_bounds(mdata, width, height);

	// Run first command in item commands.
	if (item_commands.size() > 0 && mdata->chat_box != NULL)
		if (mdata->chat_box->run_command(item_commands[0].c_str()) == -1)
			res = -1;

clean_mem:
	fclose(fp);

	if (tile_map != NULL)
		delete [] tile_data;
	if (item_data != NULL)
		delete [] item_data;
	if (command_list != NULL)
		delete [] command_list;
	if (npc_list != NULL)
		delete [] npc_list;

	return res;
}

int Map::dump(const char * file_path, size_t n) {
	int res = 0;
	FILE * fp = NULL;
	MapFileHeader header;

	size_t map_size;
	Tile::TileObject * tile_data = NULL;

	size_t item_data_size;
	ItemDataWithCoord * item_data = NULL;

	size_t command_list_size;
	char * command_list = NULL;

	size_t npc_list_size;
	NpcData * npc_list = NULL;

	Npc * curr_npc = NULL;
	NpcData curr_npc_data;

	CommonItem::ItemData curr_item;

	int x, y;
	long int i = 0, j = 0;

	if (tile_map == NULL) {
		fprintf(stderr, "Tile data is null can't write to %s\n", file_path);
		return -1;
	}

	if (item_map == NULL) {
		fprintf(stderr, "Item data is null can't write to %s\n", file_path);
		return -1;
	}

	// Open file.
	fp = fopen(file_path, "wb");

	if (fp == NULL) {
		fprintf(stderr, "Error opening %s\n", file_path);
		return -1;
	}

	// Get 'command_list_size'.
	command_list_size = 0;

	if (item_commands.size() <= 0) { // No command list.
		command_list_size = 0;
		goto skip_create_command_list;
	}

	for (auto s : item_commands)
		command_list_size += s.length() + 1; // Plus one for null byte.
	
	// Create 'command_list'.
	// Format command1\0command2\0command2\0
	command_list = new char[command_list_size];

	if (command_list == NULL) {
		fputs("Error creating 'command_list'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	memset(command_list, 0, command_list_size);

	// Add commands.
	i = 0;
	j = 0;

	for (auto s : item_commands) {
		for (j = 0; j < s.length(); j++) {
			command_list[i] = s.c_str()[j];
			i++;
		}

		command_list[i] = '\0';
		i++;
	}

skip_create_command_list:

	// Create item_data
	item_data_size = 0;

	// Number of items that are not none id.
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			if (item(x, y).id != CommonItem::NONE)
				item_data_size++;

	// Skip if zero.
	if (item_data_size <= 0)
		goto skip_creating_item_data;

	item_data = new ItemDataWithCoord[item_data_size];

	if (item_data == NULL) {
		fputs("Error getting memory for 'item_data'\n", stderr);
		res = -1;
		goto clean_mem;
	}

skip_creating_item_data:

	npc_list_size = npc_data.size();
	
	// No npcs.
	if (npc_data.empty())
		goto skip_creating_npc_list;

	// Create 'npc_list'.
	npc_list = new NpcData[npc_list_size];

	if (npc_list == NULL) {
		fputs("Error getting memory for 'npc_list'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	// Add npcs.
	for (i = 0; i < npc_list_size; i++)
		npc_list[i] = npc_data[i];

skip_creating_npc_list:

	// Create header.
	header.magic_number = MAP_MAGIC_NUM;
	header.w = (MAP_SIZE)width;
	header.h = (MAP_SIZE)height;

	header.tile_object_size = sizeof(Tile::TileObject);
	header.item_object_size = sizeof(ItemDataWithCoord);
	header.npc_object_size = sizeof(NpcData);

	header.item_data_size = (uint32_t)item_data_size;
	header.command_list_size = (uint32_t)command_list_size;
	header.npc_list_size = (uint32_t)npc_list_size;

	// Create tile array.
	map_size = header.w * header.h;
	tile_data = new Tile::TileObject[map_size];

	if (tile_data == NULL) {
		fputs("Error getting memory for 'tile_data'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	// Add tiles and items.
	i = 0;
	j = 0;

	for (y = 0; y < header.h; y++)
		for (x = 0; x < header.w; x++) {
			tile_data[i] = tile(x, y);

			// Add items.
			if (item_data != NULL) {
				curr_item = item(x, y);

				if (curr_item.id != CommonItem::NONE) {
					item_data[j].item = curr_item;
					item_data[j].x = (int16_t)x;
					item_data[j].y = (int16_t)y;
					j++;
				}
			}

			i++;
		}

	// Write header.
	if (fwrite(&header, sizeof(MapFileHeader), 1, fp) == -1) {
		fprintf(stderr, "Error writing header to %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Write command list.
	if (command_list != NULL)
		if (fwrite(command_list, sizeof(char), command_list_size, fp) == -1) {
			fprintf(stderr, "Error writing command list to %s\n", file_path);
			res = -1;
			goto clean_mem;
		}

	// Write tile data
	if (fwrite(tile_data, sizeof(Tile::TileObject), map_size, fp) == -1) {
		fprintf(stderr, "Error writing tile data to %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Write item data.
	if (item_data != NULL)
		if (fwrite(item_data, sizeof(ItemDataWithCoord), item_data_size, fp) == -1) {
			fprintf(stderr, "Error writing item data to %s\n", file_path);
			res = -1;
			goto clean_mem;
		}
	
	// Write npc list.
	if (npc_list != NULL)
		if (fwrite(npc_list, sizeof(NpcData), npc_list_size, fp) == -1) {
			fprintf(stderr, "Error writing npc list to %s\n", file_path);
			res = -1;
			goto clean_mem;
		}

	// Set path for map object.
	set_file_path(file_path, n);

	printf("Dumped map into %s width: %d, height: %d\n", file_path, header.w, header.h);

clean_mem:
	fclose(fp);

	if (tile_data != NULL)
		delete [] tile_data;
	if (item_data != NULL)
		delete [] item_data;
	if (command_list != NULL)
		delete [] command_list;
	if (npc_list != NULL)
		delete [] npc_list;

	return res;
}

int Map::run_item_command(CommonItem::ItemData item) {
	if (item.command_location == CommonItem::NONE || mdata->chat_box == NULL)
		return -1;
	if (item.command_location >= item_commands.size())
		return -1;

	return mdata->chat_box->run_command(item_commands[item.command_location].c_str());
}
