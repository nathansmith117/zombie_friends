#include "weapon.h"
#include "character.h"

void Weapon::main_init(MainData * md, Fl_Widget * item_holder) {
}

void Weapon::set_on_side() {
	if (((Character*)item_holder)->direction()->right)
		set_right();
	else
		set_left();
}
