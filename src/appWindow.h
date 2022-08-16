#pragma once

#include "program_data.h"
#include "map.h"

class AppWindow : public Fl_Double_Window {
	public:
		AppWindow(MainData * md, int W, int H, const char * l=0) : Fl_Double_Window(W, H ,l) {
			main_init(md, W, H);
		}

		void main_init(MainData * md, int W, int H);
		int handle(int event);
	private:
		MainData * mdata;
		void window_closing();
};
