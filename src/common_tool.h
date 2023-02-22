#pragma once

#include "program_data.h"
#include "object_base.h"
#include "common_item.h"

typedef int8_t TOOL_TYPE;

class CommonTool : public ObjectBase {
	public:
		CommonTool(MainData * md, ObjectBase * item_holder) : ObjectBase()  {
			main_init(md, item_holder);
		}

		~CommonTool();

		std::vector<CommonItem::ItemLocation> item_location() { return location; }
		void item_location(std::vector<CommonItem::ItemLocation> location) { this->location = location; }

		void set_fuel(int fuel) { this->fuel = fuel; }
		void add_fuel(int add_by) { fuel += add_by; }
		int get_fuel() { return fuel; }

		bool does_need_fuel() { return needs_fuel; }

		virtual void draw();

		virtual void set_right() = 0;
		virtual void set_left() = 0;

		virtual void set_on_side() = 0;

		virtual void use();
		virtual void stop_using(); // Override for tools that continue after keyup.
		bool being_used() { return is_being_used; }

		int move_to_location();

		void holder(ObjectBase * item_holder) { this->item_holder = item_holder; }

		void set_type(TOOL_TYPE type) { this->type = type; }
		TOOL_TYPE get_type() { return type; }
	protected:
		MainData * mdata;
		std::vector<CommonItem::ItemLocation> location;

		bool needs_fuel = false;
		int fuel = 0;

		ObjectBase * item_holder = NULL;

		void main_init(MainData * md, ObjectBase * item_holder);

		bool is_being_used;
		int last_count;
		int action_frame;

		TOOL_TYPE type;
};
