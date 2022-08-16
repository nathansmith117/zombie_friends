#include "edit_item_creater.h"
#include <FL/Enumerations.H>

void EditItemCreater::main_init(MainData * md, Map * map, int X, int Y, int W, int H) {
	int wx, wy, ww, wh;

	mdata = md;
	this->map = map;

	// Init widgets.
	item_preview = new CommonItem::ItemPreview(
		mdata,
		X,
		Y,
		H / 5,
		H / 5
	);

	// Widget positions and sizes.
	wx = X + (item_preview->w() * 2);
	wy = Y + item_preview->h() + mdata->settings.editor.input_thinkness;
	ww = W / 8;
	wh = mdata->settings.editor.input_thinkness;

	// Roll one.

	// Id.
	id_choice = new Fl_Input(
		wx,
		wy,
		ww,
		wh,
		"Id"
	);

	id_choice->type(FL_INT_INPUT);
	id_choice->callback(id_choice_cb, (void*)this);

	// Type.
	type_choice = new Fl_Input(
		id_choice->x() + (ww * 2),
		wy,
		ww,
		wh,
		"Type"
	);

	// Make sure it is in order with Tile::TYPES.
	type_choice->type(FL_INT_INPUT);
	type_choice->callback(type_choice_cb, (void*)this);

	// Load type.
	load_type_choice = new Fl_Choice(
		type_choice->x() + (ww * 2),
		wy,
		ww,
		wh,
		"Load type"
	);
	
	// Make sure it is in order with Tile::LOAD_TYPES.
	load_type_choice->add(ITEM_LOAD_TYPE_BASIC_STR);
	load_type_choice->add(ITEM_LOAD_TYPE_LOADED_STR);
	load_type_choice->add(ITEM_LOAD_TYPE_ADDON_STR);
	load_type_choice->callback(load_type_choice_cb, (void*)this);

	// Heath gain.
	heath_gain_choice = new Fl_Input(
		load_type_choice->x() + (ww * 2),
		wy,
		ww,
		wh,
		"Heath gain"
	);

	heath_gain_choice->type(FL_INT_INPUT);
	heath_gain_choice->callback(heath_gain_choice_cb, (void*)this);

	// Roll two.
	
	// Coin gain.
	coin_gain_choice = new Fl_Input(
		wx,
		wy + wh,
		ww,
		wh,
		"Coin gain"
	);

	coin_gain_choice->type(FL_INT_INPUT);
	coin_gain_choice->callback(coin_gain_choice_cb, (void*)this);

	// Tool gain.
	tool_gain_choice = new Fl_Input(
		coin_gain_choice->x() + (ww * 2),
		coin_gain_choice->y(),
		ww,
		wh,
		"Tool gain"
	);

	tool_gain_choice->type(FL_INT_INPUT);
	tool_gain_choice->callback(tool_gain_choice_cb, (void*)this);

	// Fuel gain.
	fuel_gain_choice = new Fl_Input(
		tool_gain_choice->x() + (ww * 2),
		coin_gain_choice->y(),
		ww,
		wh,
		"Fuel gain"
	);

	fuel_gain_choice->type(FL_INT_INPUT);
	fuel_gain_choice->callback(fuel_gain_choice_cb, (void*)this);

	// Command.
	command_choice = new Fl_Choice(
		fuel_gain_choice->x() + (ww * 2),
		fuel_gain_choice->y(),
		ww,
		wh,
		"Command"
	);

	command_choice->add("None");
	command_choice->when(FL_WHEN_RELEASE_ALWAYS);
	command_choice->callback(command_choice_cb, (void*)this);

	enter_button = new Fl_Button(
		wx,
		command_choice->y() + wh + 10,
		ww,
		wh * 2,
		"Enter"
	);

	enter_button->callback(enter_button_cb, (void*)this);

	end();
}

int EditItemCreater::handle(int event) {
	Fl_Group::handle(event); // Pass events to widgets in group.
	return 0;
}

void EditItemCreater::id_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::type_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::load_type_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::heath_gain_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::coin_gain_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::tool_gain_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::fuel_gain_choice_cb(Fl_Widget * w, void * d) {
}

void EditItemCreater::command_choice_cb(Fl_Widget * w, void * d) {
	EditItemCreater * item_creater = (EditItemCreater*)d;
	item_creater->reset_command_choice();
}

void EditItemCreater::reset_command_choice() {
	std::vector<std::string> item_commands;
	bool same_as_last_time = true;
	int i;

	if (command_choice == NULL || mdata->map == NULL)
		return;

	// Get item commands.
	item_commands = mdata->map->get_item_commands();

	if (item_commands.size() <= 0) {
		command_choice->clear();
		command_choice->add("None");
		command_choice->value(0);
		return;
	}

	// Check if same as last time.
	if (item_commands.size() != old_item_commands.size())
		goto add_commands_branch;

	for (i = 0; i < item_commands.size(); i++)
		if (item_commands[i] != old_item_commands[i]) {
			same_as_last_time = false;
			break;
		}

	// Only change if different from last time.
	if (same_as_last_time)
		return;

add_commands_branch:

	command_choice->clear();
	command_choice->add("None");

	// Set Command choice.
	for (auto c : item_commands)
		command_choice->add(c.c_str());

	old_item_commands = item_commands;
}

void EditItemCreater::enter_button_cb(Fl_Widget * w, void * d) {
	EditItemCreater * item_creater = (EditItemCreater*)d;
	MainData * mdata = (MainData*)item_creater->mdata;

	// Create item.
	item_creater->current_item.id = gameTools::valuestr_to_int(item_creater->id_choice);
	item_creater->current_item.type = gameTools::valuestr_to_int(item_creater->type_choice);
	item_creater->current_item.load_type = item_creater->load_type_choice->value();
	item_creater->current_item.heath_gain = gameTools::valuestr_to_int(item_creater->heath_gain_choice);
	item_creater->current_item.coin_gain = gameTools::valuestr_to_int(item_creater->coin_gain_choice);
	item_creater->current_item.gain_tool = gameTools::valuestr_to_int(item_creater->tool_gain_choice);
	item_creater->current_item.gain_fuel = gameTools::valuestr_to_int(item_creater->fuel_gain_choice);

	// Command choice.
	int command_choice_value = item_creater->command_choice->value();

	if (command_choice_value <= item_creater->command_choice->size())
		item_creater->current_item.command_location = command_choice_value - 1;
	else
		item_creater->current_item.command_location = CommonItem::NONE;

	// Item preview.
	item_creater->item_preview->item(item_creater->current_item);
	item_creater->item_preview->redraw();
}

void EditItemCreater::item(CommonItem::ItemData current_item) {
	this->current_item = current_item;
	item_preview->item(current_item);

	// id number to string.
	char id_buf[NAME_MAX];
	memset(id_buf, 0, NAME_MAX);
	snprintf(id_buf, NAME_MAX, "%d", current_item.id);

	// Type number to string
	char type_buf[NAME_MAX];
	memset(type_buf, 0, NAME_MAX);
	snprintf(type_buf, NAME_MAX, "%d", current_item.type);

	// Heath gain number to string.
	char heath_buf[NAME_MAX];
	memset(heath_buf, 0, NAME_MAX);
	snprintf(heath_buf, NAME_MAX, "%d", current_item.heath_gain);

	// Coin gain number to string.
	char coin_buf[NAME_MAX];
	memset(coin_buf, 0, NAME_MAX);
	snprintf(coin_buf, NAME_MAX, "%d", current_item.coin_gain);

	// Tool number to string.
	char tool_buf[NAME_MAX];
	memset(tool_buf, 0, NAME_MAX);
	snprintf(tool_buf, NAME_MAX, "%d", current_item.gain_tool);

	// Fuel number to string.
	char fuel_buf[NAME_MAX];
	memset(fuel_buf, 0, NAME_MAX);
	snprintf(fuel_buf, NAME_MAX, "%d", current_item.gain_fuel);

	id_choice->value(id_buf);
	type_choice->value(type_buf);
	load_type_choice->value((int)current_item.load_type);
	heath_gain_choice->value(heath_buf);
	coin_gain_choice->value(coin_buf);
	tool_gain_choice->value(tool_buf);
	fuel_gain_choice->value(fuel_buf);

	// Set command.
	reset_command_choice();

	if (current_item.command_location < command_choice->size())
		command_choice->value((int)current_item.command_location + 1);
	else
		command_choice->value(0);

	mdata->win->redraw();
}
