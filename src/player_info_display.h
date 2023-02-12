#pragma once

#include "program_data.h"
#include "common_item.h"
#include "common_tool.h"
#include "player.h"

class ToolDisplay : public Fl_Widget {
	public:
		ToolDisplay(MainData * md, int X, int Y, int W) : Fl_Widget(X, Y, 0, 0) {
			main_init(md, X, Y, W);
		}

		virtual void draw();
		int handle(int event);

		void reset_size();
	private:
		MainData * mdata;
		int tool_size;

		std::vector<Fl_PNG_Image*> get_tool_images();

		void main_init(MainData * md, int X, int Y, int W);
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
