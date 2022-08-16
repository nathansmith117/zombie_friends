#include "tab_menu.h"

void TabMenu::menu_side(MENU_SIDE menu_location, bool reset_items) {
	menu_locat = menu_location;

	switch (menu_location) {
		case MENU_RIGHT:
			menu_right();
			break;
		case MENU_LEFT:
			menu_left();
			break;
		case MENU_TOP:
			menu_top();
			break;
		case MENU_BOTTOM:
			menu_bottom();
			break;
		case MENU_FILL:
			menu_fill();
			break;
		case MENU_HIDDEN:
			menu_hidden();
			break;
		default:
			fprintf(stderr, "enum MENU_SIDES does not go to %d\n", menu_location);
			break;
	}

	if (reset_items)
		reset_items_size();
}

void TabMenu::menu_right() {
	show();

	resize(
		mdata->win->w() - mdata->settings.tab_menu_thickness,
		mdata->top_menu->h(),
		mdata->settings.tab_menu_thickness,
		mdata->win->h() - mdata->top_menu->h()
	);

	if (mdata->view_win != NULL)
		mdata->view_win->resize(
			0, 
			mdata->top_menu->h(), 
			mdata->win->w() - w(), 
			mdata->win->h() - mdata->top_menu->h()
		);

	// Check is value is correct.
	if (menu_locat != MENU_RIGHT)
		fputs("menu_locat is set to the wrong value", stderr);
}

void TabMenu::menu_left() {
	show();

	resize(
		0,
		mdata->top_menu->h(), 
		mdata->settings.tab_menu_thickness, 
		mdata->win->h() - mdata->top_menu->h()
	);

	if (mdata->view_win != NULL)
		mdata->view_win->resize(
			x() + w(), 
			mdata->top_menu->h(), 
			mdata->win->w() - w(), 
			mdata->win->h() - mdata->top_menu->h()
		);

	if (menu_locat != MENU_LEFT)
		fputs("menu_locat is set to the wrong value", stderr);
}

void TabMenu::menu_top() {
	show();

	resize(
		0,
		mdata->top_menu->h(), 
		mdata->win->w(), 
		mdata->settings.tab_menu_thickness
	);

	if (mdata->view_win != NULL)
		mdata->view_win->resize(
			0, 
			h(), 
			mdata->win->w(), 
			mdata->win->h() - h()
		);

	if (menu_locat != MENU_TOP)
		fputs("menu_locat is set to the wrong value", stderr);
}

void TabMenu::menu_bottom() {
	show();

	resize(
		0,
		mdata->win->h() - mdata->settings.tab_menu_thickness, 
		mdata->win->w(), 
		mdata->settings.tab_menu_thickness
	);

	if (mdata->view_win != NULL)
		mdata->view_win->resize(
			0, 
			mdata->top_menu->h(), 
			mdata->win->w(), 
			mdata->win->h() - h() - mdata->top_menu->h()
		);

	if (menu_locat != MENU_BOTTOM)
		fputs("menu_locat is set to the wrong value", stderr);
}

void TabMenu::menu_fill() {
	show();

	resize(
		0,
		mdata->top_menu->h(), 
		mdata->win->w(), 
		mdata->win->h() - mdata->top_menu->h()
	);

	if (menu_locat != MENU_FILL)
		fputs("menu_locat is set to the wrong value", stderr);
}

void TabMenu::menu_hidden() {
	hide();

	// Resize if size is 0.
	if (w() == 0 || h() == 0)
		resize(
			0,
			mdata->top_menu->h(), 
			mdata->win->w(), 
			mdata->win->h() - mdata->top_menu->h()
		);

	if (mdata->view_win != NULL)
		mdata->view_win->resize(
			0,
			mdata->top_menu->h(),
			mdata->win->w(),
			mdata->win->h() - mdata->top_menu->h()
		);

	if (menu_locat != MENU_HIDDEN)
		fputs("menu_locat is set to the wrong value", stderr);
}
