#include "editWindow.h"
#include "map.h"
#include "view_window.h"
#include "edit_menu.h"
#include "edit_tab_menu.h"
#include "edit_tile_creater.h"
#include "edit_npc_creater.h"

void EditWindow::main_init(int W, int H, MainData * md) {
	mdata = md;

	// Center.
	position((Fl::w() / 2) - (W / 2), (Fl::h() / 2) - (H / 2));

	world_x = 0;
	world_y = 0;

	Fl::add_timeout(1.0 / mdata->settings.update_fps, update_cb, (void*)this);
	Fl::add_timeout(1.0 / mdata->settings.draw_fps, draw_cb, (void*)this);
}

int EditWindow::handle(int event) {
	switch (event) {
		case FL_HIDE:
			window_closing();
			return 1;
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;
		// To get debug_window and other widgets to work.
		case FL_PUSH:
		case FL_RELEASE:
		case FL_DRAG:
		case FL_MOUSEWHEEL:
			Fl_Group::handle(event);

			if (event == FL_PUSH || event == FL_DRAG)
				handle_push();

			return 1;
		case FL_KEYDOWN:
			// A hack to make keyboard shortcuts for top menu to work correctly.
			mdata->top_menu->handle(FL_SHORTCUT);
			return 1;
		case FL_KEYUP:
			return 1;
		default:
			return 0;
	}
}

void EditWindow::update_cb(void * d) {
	EditWindow * win = (EditWindow*)d;
	win->real_update_cb();
	Fl::repeat_timeout(1.0 / win->mdata->settings.update_fps, update_cb, d);
}

void EditWindow::draw_cb(void * d) {
	EditWindow * win = (EditWindow*)d;
	win->mdata->view_win->redraw();
	Fl::repeat_timeout(1.0 / win->mdata->settings.draw_fps, draw_cb, d);
}

void EditWindow::real_update_cb() {
	wx(mdata->map_horizontal_scrollbar->value());
	wy(mdata->map_vertical_scrollbar->value());
}

void EditWindow::wx(int world_x) {
	this->world_x = world_x;
	ViewWindow * view_win = (ViewWindow*)mdata->view_win;

	if (view_win == NULL)
		return;
	if (mdata->map == NULL)
		return;
	
	mdata->map->offset_x((view_win->w() / 2) + (-world_x * mdata->scale_tile_size));
}

void EditWindow::wy(int world_y) {
	this->world_y = world_y;
	ViewWindow * view_win = (ViewWindow*)mdata->view_win;

	if (view_win == NULL)
		return;
	if (mdata->map == NULL)
		return;

	mdata->map->offset_y((view_win->h() / 2) + (world_y * mdata->scale_tile_size));
}

void EditWindow::window_closing() {
	puts("\nClosing window.");

	if (mdata->map != NULL)
		mdata->map->delete_map();

	if (mdata->edit_menu_diologs != NULL)
		delete_editor_diologs(mdata->edit_menu_diologs);
}

void EditWindow::handle_push() {
	if (mdata->view_win == NULL)
		return;

	// Get mouse pos.
	int mx, my;
	mx = Fl::event_x();
	my = Fl::event_y();

	// Get view window size and position.
	int vx, vy, vw, vh;
	vx = mdata->view_win->x();
	vy = mdata->view_win->y();
	vw = mdata->view_win->w();
	vh = mdata->view_win->h();

	// Check if mouse is on view window.
	if (!(mx >= vx &&
		mx <= vx + vw &&
		my >= vy &&
		my <= vy + vh))
		return;

	if (mdata->map == NULL)
		return;

	// Get what tile the mouse if over.
	int tile_x, tile_y;
	tile_x = int((float)(((float)mx - vx) / mdata->scale_tile_size) - (mdata->map->offset_x() / mdata->scale_tile_size));
	tile_y = int((float)(((float)my - vy) / mdata->scale_tile_size) - (mdata->map->offset_y() / mdata->scale_tile_size));

	if (tile_x < 0 || tile_y < 0)
		return;
	if (tile_x >= mdata->map->get_width() || tile_y >= mdata->map->get_height())
		return;

	EditTabMenu * tab_menu = (EditTabMenu*)mdata->tab_menu;

	if (tab_menu == NULL)
		return;

	// Get widgets from tab menu.
	Fl_Widget * current_tab = tab_menu->value();
	EditTileCreater * tile_creater = tab_menu->tile_creater;
	EditItemCreater * item_creater = tab_menu->item_creater;
	EditNpcCreater * npc_creater = tab_menu->npc_creater;

	switch (Fl::event_button()) {
		case FL_RIGHT_MOUSE:

			// Get tile/item.
			if (strncmp(ITEM_CREATER_TAB_LABEL, current_tab->label(), sizeof(ITEM_CREATER_TAB_LABEL)) == 0)
				item_creater->item(mdata->map->item(tile_x, tile_y));
			else if (strncmp(TILE_CREATER_TAB_LABEL, current_tab->label(), sizeof(TILE_CREATER_TAB_LABEL)) == 0)
				tile_creater->tile(mdata->map->tile(tile_x, tile_y));

			break;
		case FL_LEFT_MOUSE:

			// Set tile/item in map.
			if (strncmp(ITEM_CREATER_TAB_LABEL, current_tab->label(), sizeof(ITEM_CREATER_TAB_LABEL)) == 0)
				mdata->map->item(item_creater->item(), tile_x, tile_y);
			else if (strncmp(TILE_CREATER_TAB_LABEL, current_tab->label(), sizeof(TILE_CREATER_TAB_LABEL)) == 0)
				mdata->map->tile(tile_creater->tile(), tile_x, tile_y);
			else if (strncmp(NPC_CREATER_TAB_LABEL, current_tab->label(), sizeof(NPC_CREATER_TAB_LABEL)) == 0)
				npc_creater->apply_npc(tile_x, tile_y);

			break;
		default:
			break;
	}
}
