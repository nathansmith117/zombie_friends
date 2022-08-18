#include "appWindow.h"
#include "image_loader.h"
#include "player.h"
#include "debug_window.h"
#include "player_info_display.h"

void AppWindow::main_init(MainData * md, int W, int H) {
	mdata = md;

	// Center.
	position((Fl::w() / 2) - (W / 2), (Fl::h() / 2) - (H / 2));
}

int AppWindow::handle(int event) {
	//printf("Event type: %s\n", fl_eventnames[event]);

	switch (event) {
		case FL_HIDE:
			window_closing();
			return 1;
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		// To get debug_window and other widgets to work.
		case FL_PUSH:
		case FL_DRAG:
		case FL_MOUSEWHEEL:
			Fl_Group::handle(event);
			return 0;
		case FL_KEYDOWN:
			switch (Fl::event_key()) {
				case ' ':
					mdata->player->use_tool();
					break;
				case 'd':
				case 'D':
				case FL_Right:
					mdata->player->go_right();
					break;
				case 'a':
				case 'A':
				case FL_Left:
					mdata->player->go_left();
					break;
				case 'w':
				case 'W':
				case FL_Up:
					mdata->player->direction()->up = true;
					break;
				case 's':
				case 'S':
				case FL_Down:
					mdata->player->direction()->down = true;
					break;
				case 'm':
					((Player*)mdata->player)->next_tool();
					break;
				case 'n':
					((Player*)mdata->player)->pre_tool();
					break;
				case 'r':
					((Player*)mdata->player)->refresh_images();
					break;
				default:
					break;
			}

			// A hack to make keyboard shortcuts for top menu to work correctly.
			mdata->top_menu->handle(FL_SHORTCUT);
			return 1;
		case FL_KEYUP:
			switch (Fl::event_key()) {
				case ' ':
					mdata->player->stop_using_tool();
					break;
				case 'd':
				case 'D':
				case FL_Right:
					mdata->player->direction()->right = false;
					break;
				case 'a':
				case 'A':
				case FL_Left:
					mdata->player->direction()->left = false;
					break;
				case 'w':
				case 'W':
				case FL_Up:
					mdata->player->direction()->up = false;
					break;
				case 's':
				case 'S':
				case FL_Down:
					mdata->player->direction()->down = false;
					break;
				default:
					break;
			}

			return 1;
		default:
			return 0;
	}
}

void AppWindow::window_closing() {
	puts("\nClosing window.");
	delete mdata->map;
}
