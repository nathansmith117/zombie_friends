#pragma once

#include "program_data.h"
#include "common_item.h"
#include "common_tool.h"
#include "player.h"

class ToolDisplay : public Fl_Widget {
	public:
		ToolDisplay(MainData * md, int X, int Y, int max_tool_count) : Fl_Widget(X, Y, 0, 0) {
			main_init(md, X, Y, max_tool_count);
		}

		virtual void draw();
		int handle(int event);

		void set_width_and_height();

		int get_max_tool_count() { return max_tool_count; }
		void set_max_tool_count(int max_tool_count) { this->max_tool_count = max_tool_count; }
	private:
		MainData * mdata;
		int max_tool_count;

		void main_init(MainData * md, int X, int Y, int max_tool_count);
};

class PlayerInfoDisplay : public Fl_Group {
	public:
		PlayerInfoDisplay(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		void reset_size();

		int handle(int event);
		void update();
	private:
		MainData * mdata;

		// Widgets.
		ToolDisplay * tool_display = NULL;
		Fl_Output * coin_output = NULL;
		Fl_Output * heath_output = NULL;
		Fl_Output * fuel_output = NULL;

		void main_init(MainData * md, int X, int Y, int W, int H);
};
