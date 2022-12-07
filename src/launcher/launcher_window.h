#pragma once

#include "program_data.h"

class LauncherWindow : public Fl_Double_Window {
	public:
		LauncherWindow(MainData * md, int W, int H, const char * l=0) : Fl_Double_Window(W, H, l) {
			main_init(md, W, H);
		}

		int handle(int event);
	private:
		MainData * mdata;
		void main_init(MainData * md, int W, int H);

		void closing_window();

		// Handling window resize.
		int old_w, old_h;
		void resize_callback();

		// Callbacks.
		static void update_cb(void * d);

		void real_update_cb();
};
