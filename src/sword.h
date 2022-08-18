#pragma once

#include "program_data.h"
#include "weapon.h"

#define SWORD_ATTACK_FRAMES 2

class Sword : public Weapon {
	public:
		Sword(MainData * md, Fl_Widget * item_holder) : Weapon(md, item_holder) {
			main_init(md, item_holder);
			type = TOOL_SWORD;
		}

		void update();

		void stop_using() {}

		void set_right() { frame = 0; }
		void set_left() { frame = 3; }

		void refresh_images();
	protected:
		void main_init(MainData * md, Fl_Widget * item_holder);
};
