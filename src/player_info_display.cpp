#include "player_info_display.h"
#include <FL/fl_draw.H>

// ToolDisplay functions.
void ToolDisplay::main_init(MainData * md, int X, int Y, int W) {
	mdata = md;
	reset_size();
}

void ToolDisplay::reset_size() {
	tool_size = mdata->settings.tool_display_scale * TILE_SIZE;
	h(tool_size);
}

void ToolDisplay::draw() {
	int i;
	Player * player = (Player*)mdata->player;
	std::vector<Fl_PNG_Image*> images;
	int tool_count;
	int current_tool_locat;
	Fl_PNG_Image * draw_img = NULL;

	int draw_x;
	int draw_y;

	// Draw background.
	fl_rectf(x(), y(), w(), h(), color());

	// Draw outline.
	fl_rect(x(), y(), w(), h(), FL_BLACK);

	if (player == NULL)
		return;

	// Get tool info.
	tool_count = player->tool_count();
	current_tool_locat = player->get_current_tool_local();

	if (tool_count <= 0)
		return;

	// Get images.
	images = get_tool_images();

	draw_x = x() + mdata->settings.tool_display_border;

	for (i = 0; i < tool_count; i++) {
		draw_img = images[i];

		draw_y = y() + ((tool_size / 2) - (draw_img->h() / 2));

		draw_img->draw(
			draw_x,
			draw_y
		);

		// Is current image.
		if (i == current_tool_locat)
			fl_rect(
				draw_x,
				draw_y,
				draw_img->w(),
				draw_img->h()
			);

		draw_x += draw_img->w() + mdata->settings.tool_display_border;
	}

	gameTools::delete_image_list(&images);
}

std::vector<Fl_PNG_Image*> ToolDisplay::get_tool_images() {
	Player * player = (Player*)mdata->player;
	std::vector<Fl_PNG_Image*> images;
	Fl_PNG_Image * curr_img;

	int scale_width;
	int scale_height;

	// Add images.
	for (auto tool : mdata->player->get_tools()) {

		// Get image.
		curr_img = (Fl_PNG_Image*)tool->image();

		// Is null.
		if (curr_img == NULL) {
			images.push_back(
				(Fl_PNG_Image*)mdata->scaled_images.basic_world[0]->copy(tool_size, tool_size)
			);

			continue;
		}

		// Get scaled width and height.
		scale_width = curr_img->w() * mdata->settings.tool_display_scale;
		scale_height = curr_img->h() * mdata->settings.tool_display_scale;

		// Scale and add to list.
		images.push_back(
			(Fl_PNG_Image*)curr_img->copy(scale_width, scale_height)
		);
	}

	return images;
}

int ToolDisplay::handle(int event) {
	Fl::focus(mdata->view_win);
	return 0;
}

// PlayerInfoDisplay functions.
void PlayerInfoDisplay::main_init(MainData * md, int X, int Y, int W, int H) {
	mdata = md;

	// Tool display.
	tool_display = new ToolDisplay(mdata, 0, 0, 0);

	// Coin output.
	coin_output = new Fl_Output(0, 0, 0, 0, "Coins");

	// Heath output.
	heath_output = new Fl_Output(0, 0, 0, 0, "Heath");

	// Fuel output.
	fuel_output = new Fl_Output(0, 0, 0, 0, "Fuel/Ammo");

	end();

	reset_size();
	update();
}

void PlayerInfoDisplay::reset_size() {
	int output_w, output_y, output_x;
	int tool_size = mdata->settings.tool_display_scale * TILE_SIZE;

	// Tool display.
	tool_display->resize(
		x() + tool_size,
		y() + (tool_size / 2),
		w() - (tool_size * 2),
		0
	);

	tool_display->reset_size();

	// Output size.
	output_w = w() / 10;
	output_x = output_w;
	output_y = tool_display->y() + tool_display->h() + 2;

	// Coin output.
	coin_output->resize(
		output_x,
		output_y,
		output_w,
		mdata->settings.player_info_output_height
	);

	// Health output.
	heath_output->resize(
		output_x + (coin_output->w() * 2),
		output_y,
		output_w,
		mdata->settings.player_info_output_height
	);

	// Fuel output.
	fuel_output->resize(
		heath_output->x() + (heath_output->w() * 2),
		output_y,
		output_w,
		mdata->settings.player_info_output_height
	);
}

int PlayerInfoDisplay::handle(int event) {
	Fl_Group::handle(event);
	return 0;
}

void PlayerInfoDisplay::update() {
	// Get player info.
	Player * player = (Player*)mdata->player;
	CommonTool * curr_tool;

	if (player == NULL)
		return;

	char output_buf[NAME_MAX];

	// Get coins.
	memset(output_buf, 0, NAME_MAX);
	snprintf(output_buf, NAME_MAX, "%d", player->get_coins());
	coin_output->value(output_buf);

	// Get heath.
	memset(output_buf, 0, NAME_MAX);
	snprintf(output_buf, NAME_MAX, "%d", player->get_heath());
	heath_output->value(output_buf);

	// Get fuel.
	memset(output_buf, 0, NAME_MAX);
	curr_tool = player->get_current_tool();

	// Write something to 'output_buf'.
	if (curr_tool == NULL)
		strncat(output_buf, "No tool/weapon", NAME_MAX - 1);
	else {

		if (curr_tool->does_need_fuel())
			snprintf(output_buf, NAME_MAX, "%d", curr_tool->get_fuel());
		else
			strncat(output_buf, "Unlimited", NAME_MAX - 1);

	}

	fuel_output->value(output_buf);
	mdata->win->redraw();
}
