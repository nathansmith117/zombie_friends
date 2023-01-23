#pragma once

#include "program_data.h"
#include "common_item.h"

typedef int8_t TOOL_TYPE;

class CommonTool : public Fl_Widget {
	public:
		CommonTool(MainData * md, Fl_Widget * item_holder) : Fl_Widget(0, 0, 0, 0) {
			main_init(md, item_holder);
		}

		~CommonTool();

		std::vector<Fl_PNG_Image*> images() { return image_frames; }
		void images(std::vector<Fl_PNG_Image*> image_frames);

		int get_frame() { return frame; }
		void set_frame(int frame) { this->frame = frame; }

		std::vector<CommonItem::ItemLocation> item_location() { return location; }
		void item_location(std::vector<CommonItem::ItemLocation> location) { this->location = location; }

		void set_fuel(int fuel) { this->fuel = fuel; }
		void add_fuel(int add_by) { fuel += add_by; }
		int get_fuel() { return fuel; }

		bool does_need_fuel() { return needs_fuel; }

		virtual void draw();
		virtual void update() = 0;

		virtual void set_right() = 0;
		virtual void set_left() = 0;

		virtual void refresh_images() = 0;

		virtual void set_on_side() = 0;

		virtual void use();
		virtual void stop_using(); // Override for tools that continue after keyup.
		bool being_used() { return is_being_used; }

		int move_to_location();

		void holder(Fl_Widget * item_holder) { this->item_holder = item_holder; }

		Fl_PNG_Image * get_current_image();

		void set_type(TOOL_TYPE type) { this->type = type; }
		TOOL_TYPE get_type() { return type; }
	protected:
		MainData * mdata;
		int frame;
		std::vector<Fl_PNG_Image*> image_frames;
		std::vector<CommonItem::ItemLocation> location;

		bool needs_fuel = false;
		int fuel = 0;

		Fl_Widget * item_holder = NULL;

		void main_init(MainData * md, Fl_Widget * item_holder);

		bool is_being_used;
		int last_count;
		int action_frame;

		TOOL_TYPE type;
};
