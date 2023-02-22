#pragma once

#include "program_data.h"
#include "weapon.h"
#include "gun.h"
#include "game_tools.h"
#include "object_base.h"

class SubmachineGun : public Gun {
	public:
		SubmachineGun(MainData * md, ObjectBase * item_holder) : Gun(md, item_holder) {
			main_init(md, item_holder);
		}

		void update();

		void set_right() { frame = 0; }
		void set_left() { frame = 1; }

		void refresh_images();
	protected:
		void main_init(MainData * md, ObjectBase * item_holder);

		float submachine_gun_speed;
};
