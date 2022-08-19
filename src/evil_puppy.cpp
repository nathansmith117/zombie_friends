#include "evil_puppy.h"

void EvilPuppy::main_init(MainData * md) {
	mdata = md;
	
	refresh_images();

	// Config.
	always_updated = false;
}

void EvilPuppy::update() {
}

void EvilPuppy::handle_items() {
}

void EvilPuppy::go_right() {
	CommonTool * tl = get_current_tool();

	if (frame == EVIL_PUPPY_LEFT)
		frame = EVIL_PUPPY_RIGHT;

	dir.right = true;
	dir.left = false;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

void EvilPuppy::go_left() {
	CommonTool * tl = get_current_tool();

	if (frame == EVIL_PUPPY_RIGHT)
		frame = EVIL_PUPPY_LEFT;

	dir.right = false;
	dir.left = true;

	if (tl == NULL)
		return;

	tl->set_on_side();
	tl->move_to_location();
}

bool EvilPuppy::facing_right() {
	return frame < EVIL_PUPPY_LEFT;
}

bool EvilPuppy::facing_left() {
	return frame >= EVIL_PUPPY_LEFT;
}

void EvilPuppy::refresh_images() {
	images(gameTools::copy_image_list(mdata->scaled_images.npc.evil_puppy));
	refresh_tool_images();
}
