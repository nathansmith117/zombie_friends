#pragma once

#include "program_data.h"
#include "character.h"
#include "map.h"

class ViewWindow : public Fl_Double_Window {
	public:
		ViewWindow(MainData * md, int X, int Y, int W, int H, const char * l=0) 
			: Fl_Double_Window(X, Y, W, H, l) {
			main_init(md, X, Y, W, H, l);
		}

		void main_init(MainData * md, int X, int Y, int W, int H, const char * l);
		void draw();

		int handle(int event);

		bool is_inlayed() { return inlayed; }
		void is_inlayed(bool inlayed) { this->inlayed = inlayed; }
	private:
		MainData * mdata;

		// Handling window resize.
		int old_w, old_h;
		void resize_callback();

		// Callbacks.
		static void update_cb(void * d);
		static void draw_cb(void * d);

		void real_update_cb();

		bool inlayed;
};
