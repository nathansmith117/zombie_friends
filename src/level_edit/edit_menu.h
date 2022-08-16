#pragma once

#include "program_data.h"

// Diologs.
struct NewDiolog {
	MainData * mdata = NULL;
	Fl_Input * width_input = NULL;
	Fl_Input * height_input = NULL;
	Fl_Menu_Window * diolog_win = NULL;
};

struct EditMenuDiologs {
	NewDiolog * new_diolog;
};

void delete_editor_diologs(EditMenuDiologs * menu_diologs);
void create_diologs(MainData * mdata);

// Callbacks.
void editor_new_cb(Fl_Widget * w, void * d);
void editor_create_cb(Fl_Widget * w, void * d);
void editor_save_cb(Fl_Widget * w, void * d);
void editor_save_as_cb(Fl_Widget * w, void * d);
int editor_save_as(Fl_Menu_Bar * menu_bar, MainData * mdata);


void add_editor_menu_items(MainData * mdata, Fl_Menu_Bar * menu);
