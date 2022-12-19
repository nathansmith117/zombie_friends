#pragma once

#include "program_data.h"

#define TILE_SIZE 16

namespace Tile {
	enum IDS {
		NONE = -1,
		NULL_TILE = 0,
		GRASS = 1
	};

	enum LOAD_TYPES {
		BASIC = 0,
		LOADED = 1,
		ADDON = 2
	};

	enum TILE_TYPES {
		TYPE_NO_WALKTHROUGH = 0x1
	};

	typedef int8_t tile_id;

	struct __attribute__((__packed__)) TileObject {
		tile_id id = NONE;
		tile_id type = 0;
		tile_id load_type = NONE;
	};

	void draw_tile(MainData * mdata, TileObject tile_obj, int x, int y, bool scaled=true);
	Fl_PNG_Image * get_image(TileObject tile_obj, MainData * mdata, bool scaled=true);

	class TilePreview : public Fl_Widget {
		public:
			TilePreview(MainData * md, int X, int Y, int W, int H, const char * l=0) : Fl_Widget(X, Y, W, H, l) {
				mdata = md;

				// Set default tile image.
				image(mdata->images.basic_world[NULL_TILE]->copy(W, H));
			}

			void draw();

			void tile(TileObject tile);
			TileObject tile() { return current_tile; }
		protected:
			MainData * mdata;
			TileObject current_tile;
	};
}
