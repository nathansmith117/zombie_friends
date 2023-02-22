#pragma once

#include "program_data.h"
#include "common_item.h"
#include "common_tool.h"
#include "object_base.h"

class Weapon : public CommonTool {
	public:
		Weapon(MainData * md, ObjectBase * item_holder) : CommonTool(md, item_holder) {
			main_init(md, item_holder);
		}

		void set_on_side();
	protected:
		void main_init(MainData * md, ObjectBase * item_holder);
		int damage_per_hit;
};
