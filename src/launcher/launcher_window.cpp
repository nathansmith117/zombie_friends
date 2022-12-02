#include "launcher_window.h"
#include "launcher.h"

void LauncherWindow::main_init(MainData * md, int W, int H) {
	mdata = md;

	old_w = W;
	old_h = H;

	Fl::add_timeout(1.0 / mdata->settings.update_fps, update_cb, (void*)this);
}

int LauncherWindow::handle(int event) {
	switch (event) {
		case FL_HIDE:
			window_closing();
			return 1;
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		case FL_PUSH:
		case FL_RELEASE:
		case FL_DRAG:
		case FL_MOUSEWHEEL:
			Fl_Group::handle(event);
			return 1;
		case FL_SHORTCUT:
			// A hack to make keyboard shortcuts for top menu to work correctly.
			mdata->top_menu->handle(FL_SHORTCUT);
			return 1;
		default:
			return 0;
	}
}

void LauncherWindow::update_cb(void * d) {
	LauncherWindow * win = (LauncherWindow*)d;
	win->real_update_cb();
	Fl::repeat_timeout(1.0 / win->mdata->settings.update_fps, update_cb, d);
}

void LauncherWindow::real_update_cb() {

	// Resize.
	if (old_w != w() || old_h != h())
		resize_callback();
}

void LauncherWindow::resize_callback() {

	// Top menu.
	if (mdata->top_menu != NULL)
		mdata->top_menu->size(mdata->win->w(), mdata->settings.menu.height);

	old_w = w();
	old_h = h();

	redraw();
}

void LauncherWindow::window_closing() {
	if (mdata->launcher != NULL)
		delete mdata->launcher;

	mdata->state = GAME_STOPPED;
}
