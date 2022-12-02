#include "view_window.h"
#include "common_item.h"
#include "debug_window.h"
#include "weapon.h"
#include "character.h"
#include "tab_menu.h"
#include "chat_box.h"
#include "player.h"
#include "map.h"
#include "npc_map.h"

void ViewWindow::main_init(MainData * md, int X, int Y, int W, int H, const char * l) {
	mdata = md;

	old_w = W;
	old_h = H;

	inlayed = true;

	Fl::add_timeout(1.0 / mdata->settings.update_fps, update_cb, (void*)this);
	Fl::add_timeout(1.0 / mdata->settings.draw_fps, draw_cb, (void*)this);
}

void ViewWindow::draw() {
	if (!visible_r())
		return;

	fl_rectf(0, 0, w(), h(), mdata->settings.background_color);

	if (mdata->map != NULL)
		mdata->map->draw();
	if (mdata->player != NULL)
		mdata->player->draw();
}

int ViewWindow::handle(int event) {
	switch (event) {
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		case FL_KEYDOWN:
		case FL_KEYUP:

			// Pass events to main window.
			mdata->win->handle(event);
			return 1;
		default:
			return 0;
	}
}

void ViewWindow::update_cb(void * d) {
	ViewWindow * win = (ViewWindow*)d;
	win->real_update_cb();
	Fl::repeat_timeout(1.0 / win->mdata->settings.update_fps, update_cb, d);
}

void ViewWindow::draw_cb(void * d) {
	ViewWindow * win = (ViewWindow*)d;
	win->redraw();
	Fl::repeat_timeout(1.0 / win->mdata->settings.draw_fps, draw_cb, d);
}

void ViewWindow::real_update_cb() {
	mdata->state = GAME_RUNNING;
	NpcMap * npc_map;

	// Should close.
	if (mdata->should_close) {
		mdata->win->hide();
		hide();
		return;
	}

	// Resizing window.
	if (old_w != w() || old_h != h())
		resize_callback();

	if (mdata->player != NULL)
		mdata->player->update();

	// Update npc map.
	if (mdata->map == NULL)
		return;

	npc_map = mdata->map->get_npc_map();

	if (npc_map == NULL)
		return;

	npc_map->update();
}

void ViewWindow::resize_callback() {
	Player * player = NULL;
	CommonTool * player_item = NULL;
	TabMenu * tab_menu = NULL;

	// Top menu.
	if (mdata->top_menu != NULL)
		mdata->top_menu->size(mdata->win->w(), mdata->settings.menu.height);

	player = (Player*)mdata->player;

	// Player.
	if (player != NULL) {
		player->center();
		player->wx(player->wx());
		player->wy(player->wy());

		// Weapon stuff.
		player_item = player->get_current_tool();

		if (player_item != NULL)
			player_item->move_to_location();
	}

	// Tab menu.
	tab_menu = (TabMenu*)mdata->tab_menu;

	if (tab_menu != NULL)
		tab_menu->menu_side(tab_menu->menu_side());

	// Debug window.
	if (mdata->debug_win != NULL)
		mdata->debug_win->scroll_to_bottom();

	// Chat box.
	if (mdata->chat_box != NULL)
		mdata->chat_box->scroll_to_bottom();

	old_w = w();
	old_h = h();

	mdata->win->redraw();
}
