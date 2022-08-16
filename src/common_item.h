#pragma once

#include "program_data.h"

namespace CommonItem {
	enum IDS {
		NONE = -1
	};

	enum LOAD_TYPES {
		BASIC = 0,
		LOADED = 1,
		ADDON = 2
	};

	struct ItemData { // Please keep size at 64 bits.
		int8_t id = NONE;
		int8_t type = NONE;
		int8_t load_type = NONE;

		int8_t heath_gain = 0;
		int8_t coin_gain = 0;

		int8_t gain_tool = NONE;
		int8_t gain_fuel;

		int8_t command_location = NONE; // NONE for no command.
	};

	struct ItemLocation {
		int x, y;
		int holder_x, holder_y;
	};

	void draw_item(MainData * mdata, ItemData item, int x, int y, bool scaled=true, bool map_offset=true);
	Fl_PNG_Image * get_image(ItemData item, MainData * mdata, bool scaled=true);

	class ItemPreview : public Fl_Widget {
		public:
			ItemPreview(MainData * md, int X, int Y, int W, int H, const char * l=0) : Fl_Widget(X, Y, W, H, l) {
				mdata = md;
				image(mdata->images.basic_world[0]->copy(W, H));
			}

			void draw();

			void item(ItemData current_item);
			ItemData item() { return current_item; }
		protected:
			MainData * mdata;
			ItemData current_item;
	};
}
