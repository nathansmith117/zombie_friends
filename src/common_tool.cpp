#include "common_tool.h"
#include "game_tools.h"
#include "id_to_object.h"

// Tools/Weapons.
#include "gun.h"
#include "sword.h"
#include "submachine_gun.h"

CommonTool::~CommonTool() {
	delete_images();
}

void CommonTool::main_init(MainData * md, ObjectBase * item_holder) {
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
