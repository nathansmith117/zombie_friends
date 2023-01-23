#include "edit_npc_creater.h"
#include "npc.h"
#include "id_to_object.h"

void EditNpcToolEditor::main_init(int X, int Y, int W, int H) {
	int row, col;

	// Inputs.
	inputs = gameTools::create_2d_array<Fl_Input*>(NPC_TOOL_EDITOR_COLS, NPC_TOOL_EDITOR_ROWS);

	if (inputs == NULL) {
		fputs("Error getting memory for 'inputs'\n", stderr);
		end();
		return;
	}

	// Create the widgets in 'inputs'.
	for (row = 0; row < NPC_TOOL_EDITOR_ROWS; row++)
		for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++) {
			inputs[row][col] = new Fl_Input(0, 0, 0, 0);
			inputs[row][col]->type(FL_INT_INPUT);
		}

	reset_size();
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

void EditNpcToolEditor::reset_size() {
	int row, col;
	int input_w = w() / NPC_TOOL_EDITOR_COLS;
	int input_h = h() / NPC_TOOL_EDITOR_ROWS;

	if (inputs == NULL)
		return;

	for (row = 0; row < NPC_TOOL_EDITOR_ROWS; row++)
		for (col = 0; col < NPC_TOOL_EDITOR_COLS; col++)
			inputs[row][col]->resize(
				x() + (input_w * col),
				y() + (input_h * row),
				input_w,
				input_h
			);
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
	
	// Data follow type input.
	data_follow_type_input = new Fl_Input(
		wx,
		coins_input->y() + coins_input->h(),
		ww,
		wh,
		"Follow type"
	);

	data_follow_type_input->type(FL_INT_INPUT);
	data_follow_type_input->callback(data_follow_type_input_cb);
	
	// Follow data file input.
	follow_data_file_input = new Fl_Input(
		data_follow_type_input->x() + (ww * 2),
		data_follow_type_input->y(),
		ww * 2,
		wh,
		"Follow file"
	);

	follow_data_file_input->callback(follow_data_file_input_cb);

	// Next row.

	// Enter button.
	enter_button = new Fl_Button(
		wx,
		follow_data_file_input->y() + (follow_data_file_input->h() * 1.5),
		ww / 2,
		wh * 2,
		"Enter"
	);

	enter_button->callback(enter_button_cb, this);

	// Tools and fuel input.
	int tools_and_fuel_input_height = mdata->settings.editor.input_thinkness * NPC_TOOL_EDITOR_ROWS;

	tools_and_fuel_input = new EditNpcToolEditor(
		0,
		0,
		W,
		tools_and_fuel_input_height
	);

	reset_size();
	end();

	// Set default current npc.
	npc(get_clear_npc_data());
}

void EditNpcCreater::reset_size() {

	// Tools and fuel input.
	int tools_and_fuel_input_height = mdata->settings.editor.input_thinkness * NPC_TOOL_EDITOR_ROWS;
	int tools_and_fuel_input_y = (h() + y()) - tools_and_fuel_input_height;

	// If at top.
	if (mdata->settings.tab_menu_locat == MENU_TOP)
		tools_and_fuel_input_y -= mdata->top_menu->h();

	tools_and_fuel_input->resize(
		x(),
		tools_and_fuel_input_y,
		w(),
		tools_and_fuel_input_height
	);

	tools_and_fuel_input->reset_size();
}

EditNpcCreater::~EditNpcCreater() {
	if (npc_preview != NULL)
		delete npc_preview;
}

void EditNpcCreater::apply_npc(int x, int y) {
	current_npc.x = x;
	current_npc.y = y;

	// Remove npcs.
	if (current_npc.type == NPC_TYPE_NONE) {
		npc_map->remove(x, y);
		return;
	}

	// Add npc to map.
	if (npc_map->npc(current_npc) == -1) {
		fputs("Error putting npc in map.\n", stderr);
		return;
	}

	// Add to npc data for dumping to file.
	map->get_npc_data()->push_back(current_npc);
}

void EditNpcCreater::reset_npc_preview() {
	CommonTool * tool = NULL;

	// Delete old npc preview.
	if (npc_preview != NULL)
		delete npc_preview;

	// Create npc for npc preview.
	npc_preview = create_npc_from_data(mdata, npc_map, current_npc);

	if (npc_preview == NULL) {
		mdata->win->redraw();
		return;
	}

	// Set position.
	npc_preview->position(x(), y());

	// Handle direction and tool.
	npc_preview->facing_right();
	tool = npc_preview->get_current_tool();

	if (tool != NULL) {
		tool->move_to_location();
		tool->set_right();
	}

	// Add as a widget.
	add(npc_preview);
	mdata->win->redraw();
}

void EditNpcCreater::npc(NpcData current_npc) {
	char buf[NAME_MAX];

	this->current_npc = current_npc;
	reset_npc_preview();

	// Type input.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%d", current_npc.type);
	type_input->value(buf);

	// Health input.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%d", current_npc.health);
	health_input->value(buf);

	// Coins input.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%d", current_npc.coins);
	coins_input->value(buf);

	// Data follow type input.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%d", current_npc.data_follow_type);
	data_follow_type_input->value(buf);

	// Follow data file input.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%s", current_npc.follow_data_file);
	follow_data_file_input->value(buf);

	// Tools and fuel input.
	tools_and_fuel_input->get_from_npc_data(current_npc);
}

void EditNpcCreater::get_npc_from(int x, int y) {
	std::vector<NpcData> npc_data = *map->get_npc_data();

	// Find npc.
	for (auto n : npc_data)
		if (n.x == x && n.y == y) {
			npc(n);
			return;
		}

	// No npc found.
	npc(get_clear_npc_data());
}

// Callbacks.
void EditNpcCreater::type_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::health_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::coins_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::data_follow_type_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::follow_data_file_input_cb(Fl_Widget * w, void * d) {
}

void EditNpcCreater::enter_button_cb(Fl_Widget * w, void * d) {
	EditNpcCreater * npc_creater = (EditNpcCreater*)d;
	MainData * mdata = npc_creater->mdata;
	NpcData * current_npc = &npc_creater->current_npc;

	// Most items.
	current_npc->type = (NPC_TYPE)gameTools::valuestr_to_int(npc_creater->type_input);
	current_npc->health = gameTools::valuestr_to_int(npc_creater->health_input);
	current_npc->coins = gameTools::valuestr_to_int(npc_creater->coins_input);
	current_npc->data_follow_type = gameTools::valuestr_to_int(npc_creater->data_follow_type_input);

	// Follow data file.
	memset(current_npc->follow_data_file, 0, NPC_FOLLOW_DATA_PATH_SIZE);

	// White space as file name would make debuging hard.
	if (npc_creater->follow_data_file_input->value()[0] != ' ')
		strncat(
			(char*)current_npc->follow_data_file, 
			npc_creater->follow_data_file_input->value(), 
			NPC_FOLLOW_DATA_PATH_SIZE - 1
		);
	

	// Get tools and fuel.
	npc_creater->tools_and_fuel_input->set_to_npc_data(current_npc);

	// Npc preview.
	npc_creater->reset_npc_preview();
}

void EditNpcCreater::tools_and_fuel_cb(Fl_Widget * w, void * d) {
}
