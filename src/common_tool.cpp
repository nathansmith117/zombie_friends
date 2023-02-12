#include "common_tool.h"
#include "game_tools.h"
#include "id_to_object.h"

// Tools/Weapons.
#include "gun.h"
#include "sword.h"
#include "submachine_gun.h"

CommonTool::~CommonTool() {
	for (auto i : image_frames)
		if (i != NULL)
			delete i;

	image_frames.clear();
}

void CommonTool::main_init(MainData * md, Fl_Widget * item_holder) {
	mdata = md;

	this->item_holder = item_holder;

	frame = 0;
	is_being_used = false;
	type = TOOL_NONE;
}

void CommonTool::draw() {
	Fl_PNG_Image * current_img;
	current_img = get_current_image();
	
	if (current_img == NULL)
		return;

	current_img->draw(x(), y());
}

void CommonTool::images(std::vector<Fl_PNG_Image*> image_frames) {
	gameTools::set_image_list(&this->image_frames, image_frames);
	size(image_frames[0]->w(), image_frames[0]->h());
}

int CommonTool::move_to_location() {
	if (frame >= location.size() || frame < 0)
		return -1;
	if (item_holder == NULL)
		return -1;

	position(item_holder->x() + (location[frame].holder_x * mdata->settings.scale) 
			- (location[frame].x * mdata->settings.scale), 
			item_holder->y() + (location[frame].holder_y * mdata->settings.scale) 
			- (location[frame].y * mdata->settings.scale));

	return 0;
}

Fl_PNG_Image * CommonTool::get_current_image() {
	if (frame >= location.size() || frame < 0)
		return NULL;

	return image_frames[frame];
}

void CommonTool::use() {
	if (is_being_used)
		return;

	is_being_used = true;
	action_frame = 0;
	last_count = 0;
}

void CommonTool::stop_using() {
	if (!is_being_used)
		return;

	is_being_used = false;
	action_frame = 0;
	last_count = 0;
}
