#include "edit_tile_creater.h"

void EditTileCreater::main_init(MainData * md, Map * map, int X, int Y, int W, int H) {
	mdata = md;
	this->map = map;

	// Init widgets.
	tile_preview = new Tile::TilePreview(
		mdata,
		X,
		Y,
		H / 5,
		H / 5
	);
	
	id_choice = new Fl_Input(
		X + (W / 4), 
		Y + mdata->settings.editor.input_thinkness, 
		W / 2, 
		mdata->settings.editor.input_thinkness,
		"Id"
	);

	id_choice->type(FL_INT_INPUT);
	id_choice->callback(id_choice_cb, (void*)this);

	type_choice = new Fl_Input(
		X + (W / 4),
		id_choice->y() + id_choice->h(),
		W / 2,
		mdata->settings.editor.input_thinkness,
		"Type"
	);

	// Make sure it is in order with Tile::TYPES.
	type_choice->type(FL_INT_INPUT);
	type_choice->callback(type_choice_cb, (void*)this);

	load_type_choice = new Fl_Choice(
		X + (W / 4),
		type_choice->y() + type_choice->h(),
		W / 2,
		mdata->settings.editor.input_thinkness,
		"Load type"
	);
	
	// Make sure it is in order with Tile::LOAD_TYPES.
	load_type_choice->add(TILE_LOAD_TYPE_BASIC_STR);
	load_type_choice->add(TILE_LOAD_TYPE_LOADED_STR);
	load_type_choice->add(TILE_LOAD_TYPE_ADDON_STR);
	load_type_choice->callback(load_type_choice_cb, (void*)this);

	enter_button = new Fl_Button(
		X + (W / 4),
		load_type_choice->y() + (load_type_choice->h() * 2),
		mdata->settings.editor.input_thinkness * 4,
		mdata->settings.editor.input_thinkness * 2,
		"Enter"
	);

	enter_button->callback(enter_button_cb, (void*)this);

	end();
}

int EditTileCreater::handle(int event) {
	Fl_Group::handle(event); // Pass events to widgets in group.
	return 0;
}

// Callbacks.
void EditTileCreater::id_choice_cb(Fl_Widget * w, void * d) {
}

void EditTileCreater::type_choice_cb(Fl_Widget * w, void * d) {
}

void EditTileCreater::load_type_choice_cb(Fl_Widget * w, void * d) {
}

void EditTileCreater::enter_button_cb(Fl_Widget * w, void * d) {
	EditTileCreater * tile_creator = (EditTileCreater*)d;
	MainData * mdata = (MainData*)tile_creator->mdata;

	// Get values.
	tile_creator->current_tile.id = (Tile::tile_id)gameTools::valuestr_to_int(tile_creator->id_choice);
	Tile::tile_id type_value = (Tile::tile_id)gameTools::valuestr_to_int(tile_creator->type_choice);
	Tile::tile_id load_type_value = (Tile::tile_id)tile_creator->load_type_choice->value();

	// Set tile type.
	if (type_value == -1) {
		fprintf(stderr, "There is not tile type\n");
		return;
	}

	tile_creator->current_tile.type = type_value;

	// Set tile load type.
	if (load_type_value == -1) {
		fprintf(stderr, "There is not tile load type\n");
		return;
	}

	tile_creator->current_tile.load_type = load_type_value;

	// Set preview tile.
	tile_creator->tile_preview->tile(tile_creator->current_tile);
	tile_creator->tile_preview->redraw();
}

void EditTileCreater::tile(Tile::TileObject tile) {
	current_tile = tile;
	tile_preview->tile(current_tile);

	// id number to string.
	char id_buf[NAME_MAX];
	memset(id_buf, 0, NAME_MAX);
	snprintf(id_buf, NAME_MAX, "%d", current_tile.id);

	// Type number to string
	char type_buf[NAME_MAX];
	memset(type_buf, 0, NAME_MAX);
	snprintf(type_buf, NAME_MAX, "%d", current_tile.type);

	id_choice->value(id_buf);
	type_choice->value(type_buf);
	load_type_choice->value((int)current_tile.load_type);

	mdata->win->redraw();
}
