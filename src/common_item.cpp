#include "common_item.h"
#include "map.h"

namespace CommonItem {
	void draw_item(MainData * mdata, ItemData item, int x, int y, bool scaled, bool map_offset) {
		Fl_PNG_Image * draw_image;
		Map * the_map;
		int draw_x, draw_y;

		if (item.id == NONE)
			return;

		the_map = mdata->map;

		if (the_map == NULL)
			return;

		if (scaled) {
			draw_x = x * mdata->scale_tile_size;
			draw_y = y * mdata->scale_tile_size;
		} else {
			draw_x = x * TILE_SIZE;
			draw_y = y * TILE_SIZE;
		}

		draw_image = get_image(item, mdata, scaled);

		if (draw_image == NULL)
			return;

		if (map_offset)
			draw_image->draw(
				draw_x + the_map->offset_x(),
				draw_y + the_map->offset_y()
			);
		else
			draw_image->draw(draw_x, draw_y);
	}

	Fl_PNG_Image * get_image(ItemData item, MainData * mdata, bool scaled) {
		std::vector<Fl_PNG_Image*> get_list;
		GameImages images;

		if (scaled)
			images = mdata->scaled_images;
		else
			images = mdata->images;

		switch(item.load_type) {
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

		if (item.id == NONE || item.id >= get_list.size())
			return NULL;
		
		return get_list[item.id];
	}

	// TilePreview functions.
	void ItemPreview::draw() {
		// Fill background.
		fl_rectf(x(), y(), w(), h(), color());

		if (image() == NULL) {
			fl_rectf(x(), y(), w(), h(), mdata->settings.background_color);
			return;
		}

		image()->draw(x(), y());
	}

	void ItemPreview::item(ItemData current_item) {
		this->current_item = current_item;

		// Check if id is NONE.
		if (current_item.id == NONE) {
			image(NULL);
			return;
		}

		Fl_PNG_Image * item_img = get_image(current_item, mdata, false);

		if (item_img == NULL) {
			fputs("Tile image is NULL\n", stderr);
			return;
		}

		image(item_img->copy(w(), h()));
	}
}
