#pragma once

#include "program_data.h"

class EditWindow : public Fl_Double_Window {
	public:
		EditWindow(MainData * md, int W, int H, const char * l=0) : Fl_Double_Window(W, H, l) {
			main_init(W, H, md);
		}

		void main_init(int W, int H, MainData * md);
		int handle(int event);

		static void update_cb(void * d);
		static void draw_cb(void * d);

		void real_update_cb();

		int wx() { return world_x; }
		void wx(int world_x);

		int wy() { return world_y; }
		void wy(int world_y);

		void window_closing();
	private:
		MainData * mdata;
		int world_x, world_y;
		
		void handle_push();
};
