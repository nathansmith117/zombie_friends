#pragma once

#include "program_data.h"
#include "tile.h"
#include "common_item.h"

#define MAP_MAGIC_NUM 7

enum HitTypes {
	HIT_NONE = 0,
	HIT_TILE = 0x1 << 0,
	HIT_ITEM = 0x1 << 1
};

typedef uint16_t HIT_TYPE;
typedef int16_t MAP_SIZE;

struct ItemDataWithCoord {
	CommonItem::ItemData item;
	int16_t x, y;
};

struct ThingsHit {
	Tile::TileObject tile;
	CommonItem::ItemData item;
};

struct MapFileHeader {
	uint8_t magic_number;
	MAP_SIZE w, h;

	uint32_t tile_object_size;
	uint32_t item_object_size;

	uint32_t item_data_size;
	uint32_t command_list_size; // 0 for no commands.
};

class Map {
	public:
		Map(MainData * md);
		~Map();

		int create_map(int width, int height);
		void delete_map();

		Tile::TileObject ** map_data() { return tile_map; }

		int get_width() { return width; }
		int get_height() { return height; }

		Tile::TileObject tile(int x, int y);
		int tile(Tile::TileObject new_tile, int x, int y);
		int remove_tile(int x, int y);

		int item(CommonItem::ItemData new_item, int x, int y);
		CommonItem::ItemData item(int x, int y);
		int remove_item(int x, int y);

		int offset_x() { return x_offset; }
		void offset_x(int offset) { x_offset = offset; }

		int offset_y() { return y_offset; }
		void offset_y(int offset) { y_offset = offset; }

		int fill_with(Tile::TileObject full_tile);
		int fill_with(Tile::TileObject full_tile, int start_x, int start_y, int end_x, int end_y);

		void add_item_command(std::string item_command) { item_commands.push_back(item_command); }
		void add_item_commands(std::vector<std::string> new_items);
		void clear_item_commands() { item_commands.clear(); }
		std::string get_item_command(int i) { return item_commands[i]; }
		std::vector<std::string> get_item_commands() { return item_commands; }
		void set_item_commands(std::vector<std::string> item_commands) { this->item_commands = item_commands; }

		void draw();

		int set_file_path(const char * fpath, size_t n);
		int get_file_path(char * fpath, size_t n);

		int load(const char * file_path, size_t n);
		int dump(const char * file_path, size_t n);

		int run_item_command(CommonItem::ItemData item);

	private:
		MainData * mdata;
		Tile::TileObject ** tile_map = NULL;
		CommonItem::ItemData ** item_map = NULL;

		class NpcMap * npc_map = NULL;

		std::vector<std::string> item_commands;

		char file_path[NAME_MAX];

		int width, height;
		int x_offset, y_offset;
};
