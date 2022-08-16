#pragma once

#include "program_data.h"

class TabMenu : public Fl_Tabs {
	public:
		TabMenu(MainData * md, MENU_SIDE menu_location) : Fl_Tabs(0, 0, 0, 0) {
			mdata = md;
		}

		void menu_side(MENU_SIDE menu_location, bool reset_items=true); // set menu to location.
		MENU_SIDE menu_side() { return menu_locat; }

		// Reset items position and size if location changes.
		virtual void reset_items_size() = 0;
	protected:
		MainData * mdata;
		MENU_SIDE menu_locat;

		virtual void main_init(MainData * md, MENU_SIDE menu_location) = 0;
		virtual void add_items() = 0;

		// Settings location.
		void menu_right();
		void menu_left();
		void menu_top();
		void menu_bottom();
		void menu_fill();
		void menu_hidden();
};
