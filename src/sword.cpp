#include "sword.h"
#include "character.h"

void Sword::main_init(MainData * md, Fl_Widget * item_holder) {
	mdata = md;

	type = TOOL_SWORD;

	refresh_images();

	// Location data.
	item_location({
		{0, 12, 14, 19},
		{0, 12, 14, 19},
		{0, 6, 14, 19},
		{16, 12, 0, 19},
		{16, 12, 0, 19},
		{16, 6, 0, 19}
	});

	move_to_location();
}

void Sword::update() {
	if (!is_being_used)
		return;

	last_count++;

	// Slow stuff down.
	if (last_count < (int)roundf(mdata->settings.update_fps 
				* mdata->settings.weapons.sword_speed))
		return;
	else
		last_count = 0;

	// Update frames.
	if (action_frame <= SWORD_ATTACK_FRAMES) {

		if (((Character*)item_holder)->facing_right())
			frame = action_frame;
		else
			frame = action_frame + 3;

		action_frame++;
		move_to_location();
	} else {
		action_frame = 0;

		if (((Character*)item_holder)->facing_right())
			frame = 0;
		else
			frame = 3;

		is_being_used = false;
		move_to_location();
	}
}

void Sword::refresh_images() {
	images(gameTools::copy_image_list(mdata->scaled_images.weapons.sword));
	image(mdata->scaled_images.weapons.sword[0]->copy());
}
