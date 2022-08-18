#include "evil_puppy.h"

void EvilPuppy::main_init(MainData * md) {
	mdata = md;
}

void EvilPuppy::update() {
}

void EvilPuppy::handle_items() {
}

void EvilPuppy::go_right() {
}

void EvilPuppy::go_left() {
}

bool EvilPuppy::facing_right() {
	return true;
}

bool EvilPuppy::facing_left() {
	return true;
}

void EvilPuppy::refresh_images() {
	images(gameTools::copy_image_list(mdata->scaled_images.npc.evil_puppy));
	refresh_tool_images();
}
