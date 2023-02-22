#include "object_base.h"
#include "game_tools.h"

ObjectBase::ObjectBase() {
	screen_x = 0;
	screen_y = 0;

	width = 0;
	height = 0;

	frame = 0;
}

ObjectBase::~ObjectBase() {
}

Fl_PNG_Image * ObjectBase::get_current_image() {
	if (frame >= image_frames.size() || frame < 0)
		return NULL;

	return image_frames[frame];
}

int ObjectBase::get_width() {
	Fl_PNG_Image * curr_image = get_current_image();

	if (curr_image == NULL)
		return 0;

	return curr_image->w();
}

int ObjectBase::get_height() {
	Fl_PNG_Image * curr_image = get_current_image();

	if (curr_image == NULL)
		return 0;

	return curr_image->h();
}

void ObjectBase::delete_images() {
	for (auto i : image_frames)
		if (i != NULL)
			delete i;

	image_frames.clear();
}

void ObjectBase::images(std::vector<Fl_PNG_Image*> image_frames) {
	if (image_frames.empty())
		return;

	gameTools::set_image_list(&this->image_frames, image_frames);
	size(image_frames[0]->w(), image_frames[0]->h());
}
