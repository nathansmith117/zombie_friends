#include "tile.h"

namespace Tile {
	void draw_tile(MainData * mdata, TileObject tile_obj, int x, int y, bool scaled) {
		Fl_PNG_Image * draw_image;

		if (tile_obj.id == NONE)
			return;

		draw_image = get_image(tile_obj, mdata, scaled);

		if (draw_image == NULL)
			return;

		draw_image->draw(x, y);
	}

	Fl_PNG_Image * get_image(TileObject tile_obj, MainData * mdata, bool scaled) {
		std::vector<Fl_PNG_Image*> get_list;
		GameImages images;

		if (scaled)
			images = mdata->scaled_images;
		else
			images = mdata->images;

		switch(tile_obj.load_type) {
			case BASIC:
				get_list = images.basic_world;
				break;
			case LOADED:
				get_list = images.curr_world;
			case ADDON:
				get_list = images.addon_images;
				break;
			default:
				return NULL;
		}

		if (tile_obj.id == NONE || tile_obj.id >= get_list.size())
			return NULL;
		
		return get_list[tile_obj.id];
	}

	// TilePreview functions.
	void TilePreview::draw() {
		// Fill background.
		fl_rectf(x(), y(), w(), h(), color());

		if (image() == NULL) {
			fl_rectf(x(), y(), w(), h(), mdata->settings.background_color);
			return;
		}

		image()->draw(x(), y());
	}

	void TilePreview::tile(TileObject tile) {
		current_tile = tile;

		// Check if id is NONE.
		if (tile.id == NONE) {
			image(NULL);
			return;
		}

		Fl_PNG_Image * tile_img = get_image(tile, mdata, false);

		if (tile_img == NULL) {
			fputs("Tile image is NULL\n", stderr);
			return;
		}

		image(tile_img->copy(w(), h()));
	}
}
