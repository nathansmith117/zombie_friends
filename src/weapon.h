#pragma once

#include "program_data.h"
#include "common_item.h"
#include "common_tool.h"

class Weapon : public CommonTool {
	public:
		Weapon(MainData * md, Fl_Widget * item_holder) : CommonTool(md, item_holder) {
			main_init(md, item_holder);
			type = TOOL_WEAPON;
		}

		void set_on_side();
	protected:
		void main_init(MainData * md, Fl_Widget * item_holder);
		int damage_per_hit;
};
