#include "edit_item_command_list.h"

void EditItemCommandList::main_init(MainData * md, int X, int Y, int W, int H) {
	mdata = md;

	// Init widgets.
	
	// Text editor.
	text_editor = new Fl_Text_Editor(0, 0, 0, 0);

	text_buffer = new Fl_Text_Buffer();
	text_editor->buffer(text_buffer);

	// Reset button.
	reset_button = new Fl_Button(0, 0, 0, 0);

	reset_button->label("Reset");
	reset_button->callback(reset_button_cb, (void*)this);

	// Save button.
	save_button = new Fl_Button(0, 0, 0, 0);

	save_button->label("Save");
	save_button->callback(save_button_cb, (void*)this);

	end();

	reset_size();
	reset_command_list();
}

void EditItemCommandList::reset_size() {
	int input_y;

	input_y = h() - mdata->settings.editor.item_command_list_button_height;

	if (mdata->settings.tab_menu_locat == MENU_TOP)
		input_y -= mdata->top_menu->h();

	// Text editor.
	text_editor->resize(
		x(),
		y(),
		w(),
		input_y
	);

	input_y = y() + text_editor->h();

	// Reset button.
	reset_button->resize(
		x(),
		input_y,
		mdata->settings.editor.item_command_list_button_width,
		mdata->settings.editor.item_command_list_button_height
	);

	// Save button.
	save_button->resize(
		x() + reset_button->w(),
		input_y,
		mdata->settings.editor.item_command_list_button_width,
		mdata->settings.editor.item_command_list_button_height
	);

}

int EditItemCommandList::reset_command_list() {
	int i;
	std::vector<std::string> item_commands;

	if (mdata->map == NULL)
		return -1;

	text_buffer->text("");
	item_commands = mdata->map->get_item_commands();

	if (item_commands.size() <= 0)
		return 0;

	for (i = 0; i < item_commands.size(); i++) {
		text_buffer->append(item_commands[i].c_str());

		// Add new line to all but last lines.
		if (i != item_commands.size() - 1)
			text_buffer->append("\n");
	}

	return 0;
}

int EditItemCommandList::save_command_list() {
	std::vector<std::string> new_item_commands;
	int i, j;
	char c;

	if (mdata->map == NULL)
		return -1;

	// No commands.
	if (text_buffer->length() <= 0) {
		mdata->map->clear_item_commands();
		return 0;
	}

	mdata->map->clear_item_commands();
	new_item_commands.push_back(std::string());
	j = 0;

	// Add item_commands.
	for (i = 0; i < text_buffer->length(); i++) {
		c = text_buffer->byte_at(i);

		if (c == '\n') { // New line.
			new_item_commands.push_back(std::string());
			j++;
			continue;
		}

		new_item_commands[j] += c;
	}

	mdata->map->set_item_commands(new_item_commands);
	return 0;
}

void EditItemCommandList::reset_button_cb(Fl_Widget * w, void * d) {
	EditItemCommandList * item_command_list = (EditItemCommandList*)d;
	item_command_list->reset_command_list();
}

void EditItemCommandList::save_button_cb(Fl_Widget * w, void * d) {
	EditItemCommandList * item_command_list = (EditItemCommandList*)d;
	item_command_list->save_command_list();
}
