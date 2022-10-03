#pragma once

#include "program_data.h"
#include "game_tools.h"
#include "map.h"

class EditItemCommandList : public Fl_Group {
	public:
		EditItemCommandList(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		void reset_size();

		int reset_command_list();
		int save_command_list();
	private:
		MainData * mdata;

		Fl_Text_Editor * text_editor = NULL;
		Fl_Text_Buffer * text_buffer = NULL;
		Fl_Button * reset_button = NULL;
		Fl_Button * save_button = NULL;

		static void reset_button_cb(Fl_Widget * w, void * d);
		static void save_button_cb(Fl_Widget * w, void * d);

		void main_init(MainData * md, int X, int Y, int W, int H);
};
