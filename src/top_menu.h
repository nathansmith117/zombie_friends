#pragma once

#include "program_data.h"

// Callbacks.
void exit_cb(Fl_Widget * w, void * d);
void settings_cb(Fl_Widget * w, void * d);
void open_cb(Fl_Widget * w, void * d);
void fullscreen_cb(Fl_Widget * w, void * d);
void view_window_cb(Fl_Widget * w, void * d);
void tab_menu_cb(Fl_Widget * w, void * d);
void zoom_in_cb(Fl_Widget * w, void * d);
void zoom_out_cb(Fl_Widget * w, void * d);

void add_menu_items(Fl_Menu_Bar * menu, MainData * mdata);
