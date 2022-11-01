#include "edit_npc_creater.h"

void EditNpcToolEditor::main_init(int X, int Y, int W, int H) {
	int row, col;

	// Inputs.
	inputs = gameTools::create_2d_array<Fl_Input*>(NPC_TOOL_EDITOR_COLS, NPC_TOOL_EDITOR_ROWS);

	if (inputs == NULL) {
		fputs("Error getting memory for 'inputs'\n", stderr);
		end();
		return;
	}

	int input_w = W / NPC_TOOL_EDITOR_COLS;
	int input_h = H / NPC_TOOL_EDITOR_ROWS;

	// Create the widgets in 'inputs'.
	for (row = 0; row < NPC_TOOL_EDITOR_ROWS; row++)
		for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++) {
			inputs[row][col] = new Fl_Input(
				X + (input_w * col),
				Y + (input_h * row),
				input_w,
				input_h
			);

			inputs[row][col]->type(FL_INT_INPUT);
		}

	end();
}

EditNpcToolEditor::~EditNpcToolEditor() {
	int row, col;

	if (inputs == NULL)
		return;

	// Delete 'inputs'.
	for (row = 0; row < NPC_TOOL_EDITOR_ROWS; row++)
		for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++)
			if (inputs[row][col] != NULL)
				delete inputs[row][col];

	gameTools::delete_2d_array<Fl_Input*>(inputs, NPC_TOOL_EDITOR_COLS, NPC_TOOL_EDITOR_ROWS);
}

int EditNpcToolEditor::set_to_npc_data(NpcData * npc_data) {
	int col;

	if (npc_data == NULL)
		return -1;
	if (inputs == NULL)
		return -1;

	for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++) {
		// Tools.
		npc_data->tools[col] = gameTools::valuestr_to_int(inputs[0][col]);

		// Fuel.
		npc_data->fuel[col] = gameTools::valuestr_to_int(inputs[1][col]);
	}

	return 0;
}

int EditNpcToolEditor::get_from_npc_data(NpcData npc_data) {
	int col;
	char buf[NAME_MAX];

	if (inputs == NULL)
		return -1;

	for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++) {
		// Tools.
		memset(buf, 0, NAME_MAX);
		snprintf(buf, NAME_MAX, "%d", npc_data.tools[col]);
		inputs[0][col]->value(buf);

		// Fuel.
		memset(buf, 0, NAME_MAX);
		snprintf(buf, NAME_MAX, "%d", npc_data.fuel[col]);
		inputs[1][col]->value(buf);
	}

	return 0;
}

void EditNpcCreater::main_init(MainData * md, int X, int Y, int W, int H) {
	int wx, wy, ww, wh;

	int x, y;

	mdata = md;
	map = mdata->map;
	npc_map = mdata->map->get_npc_map();

	// Clear npc data.
	current_npc = get_clear_npc_data();

	// Widget positions and sizes.
	wx = X + (W / 10);
	wy = Y + mdata->settings.editor.input_thinkness;
	ww = W / 8;
	wh = mdata->settings.editor.input_thinkness;

	// Add widgets.
	
	// Type input.
	type_input = new Fl_Input(
		wx,
		wy,
		ww,
		wh,
		"Type"
	);

	type_input->type(FL_INT_INPUT);
	type_input->callback(type_input_cb);

	// Health input.
	health_input = new Fl_Input(
		type_input->x() + (ww * 2),
		wy,
		ww,
		wh,
		"Health"
	);

	health_input->type(FL_INT_INPUT);
	health_input->callback(type_input_cb);

	// Coins input.
	coins_input = new Fl_Input(
		health_input->x() + (ww * 2),
		wy,
		ww,
		wh,
		"Coins"
	);

	coins_input->type(FL_INT_INPUT);
	coins_input->callback(coins_input_cb);

	// Next row.

	// Enter button.
	enter_button = new Fl_Button(
		wx,
		coins_input->y() + (coins_input->h() * 2),
		ww,
		wh * 2,
		"Enter"
	);

	enter_button->callback(enter_button_cb, this);

	// Tools and fuel input.
	int tools_and_fuel_input_height = mdata->settings.editor.input_thinkness * NPC_TOOL_EDITOR_ROWS;
	int tools_and_fuel_input_y = (H + Y) - tools_and_fuel_input_height;

	tools_and_fuel_input = new EditNpcToolEditor(
		X,
		tools_and_fuel_input_y,
		W,
		tools_and_fuel_input_height
	);

	end();
}

void EditNpcCreater::apply_npc(int x, int y) {
	current_npc.x = x;
	current_npc.y = y;

	// Add npc to map.
	if (npc_map->npc(current_npc) == -1) {
		fputs("Error putting npc in map.\n", stderr);
		return;
	}

	// Add to npc data for dumping to file.
	map->get_npc_data()->push_back(current_npc);
}

// Callbacks.
void EditNpcCreater::type_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::health_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::coins_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::tools_and_fuel_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::enter_button_cb(Fl_Widget * w, void * d) {
	EditNpcCreater * npc_creater = (EditNpcCreater*)d;
	MainData * mdata = npc_creater->mdata;
	NpcData * current_npc = &npc_creater->current_npc;

	// Get type, health, and coins.
	current_npc->type = (NPC_TYPE)gameTools::valuestr_to_int(npc_creater->type_input);
	current_npc->health = gameTools::valuestr_to_int(npc_creater->health_input);
	current_npc->coins = gameTools::valuestr_to_int(npc_creater->coins_input);

	// Get tools and fuel.
	npc_creater->tools_and_fuel_input->set_to_npc_data(current_npc);
}
