#include "player_info_display.h"
#include <FL/fl_draw.H>

// ToolDisplay functions.
void ToolDisplay::main_init(MainData * md, int X, int Y, int max_tool_count) {
	mdata = md;
	this->max_tool_count = max_tool_count;

	set_width_and_height();
}

void ToolDisplay::set_width_and_height() {
	size(
		mdata->scale_tile_size * max_tool_count,
		mdata->scale_tile_size
	);
}

void ToolDisplay::draw() {
	int i;
	Player * player = (Player*)mdata->player;
	std::vector<CommonTool*> tools;
	int tool_count;
	int current_tool_locat;
	Fl_PNG_Image * draw_img = NULL;

	// Draw background.
	fl_rectf(x(), y(), w(), h(), color());

	// Draw outline.
	fl_rect(x(), y(), w(), h(), FL_BLACK);

	if (player == NULL)
		return;

	// Get tools.
	tools = player->get_tools();
	tool_count = player->tool_count();
	current_tool_locat = player->get_current_tool_local();

	if (tool_count <= 0)
		return;

	for (i = 0; i < tool_count; i++) {
		// Get image to draw.
		draw_img = (Fl_PNG_Image*)tools[i]->image();

		if (draw_img == NULL)
			draw_img = mdata->scaled_images.basic_world[0]; // NULL.

		// Draw image.
		draw_img->draw(
			x() + (i * mdata->scale_tile_size),
			y()
		);

		// Is current image.
		if (i == current_tool_locat)
			fl_rect(
				x() + (i * mdata->scale_tile_size),
				y(),
				draw_img->w(),
				draw_img->h()
			);
	}
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

	reset_size();
	update();
}

void PlayerInfoDisplay::reset_size() {
	int output_w, output_y, output_x;

	// Tool display.
	tool_display->position(
		x() + mdata->scale_tile_size,
		y() + (mdata->scale_tile_size / 2)
	);

	tool_display->set_max_tool_count((w() - (mdata->scale_tile_size * 2)) / mdata->scale_tile_size);
	tool_display->set_width_and_height();

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
