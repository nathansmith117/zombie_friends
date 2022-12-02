#include "settings_editor.h"
#include "player.h"
#include "tab_menu.h"

void SettingsEditor::main_init(MainData * md) {
	mdata = md;

	int i;

	memset(file_path, 0, NAME_MAX);
	
	// Set size and position.
	resize(
		Fl::event_x_root(), 
		Fl::event_y_root(),
		mdata->settings.settings_editor.width,
		mdata->settings.settings_editor.height
	);

	file_chooser = new Fl_Native_File_Chooser(Fl_Native_File_Chooser::BROWSE_FILE);
	
	// Add widgets.
	
	// Scroll.
	scroll = new Fl_Scroll(0, 0, w(), h());
	scroll->type(Fl_Scroll::BOTH);

	int input_height = mdata->settings.input_height;

	// Apply button.
	apply_button = new Fl_Button(
		0,
		0,
		input_height * 2.5,
		input_height * 1.5,
		"Apply"
	);

	apply_button->callback(apply_cb, (void*)this);

	// Reload button.
	reload_button = new Fl_Button(
		apply_button->x() + apply_button->w(),
		apply_button->y(),
		input_height * 2.5,
		input_height * 1.5,
		"Reload"
	);

	reload_button->callback(reload_cb, (void*)this);

	// Load file.
	load_file_button = new Fl_Button(
		reload_button->x() + reload_button->w(),
		reload_button->y(),
		input_height * 2.5,
		input_height * 1.5,
		"Load file"
	);

	load_file_button->callback(load_file_cb, (void*)this);

	// Save file.
	save_file_button = new Fl_Button(
		load_file_button->x() + load_file_button->w(),
		load_file_button->y(),
		input_height * 2.5,
		input_height * 1.5,
		"Save file"
	);

	save_file_button->callback(save_file_cb, (void*)this);

	// Save as file.
	save_as_file_button = new Fl_Button(
		save_file_button->x() + save_file_button->w(),
		save_file_button->y(),
		input_height * 2.5,
		input_height * 1.5,
		"Save as"
	);

	save_as_file_button->callback(save_as_file_cb, (void*)this);

	// Settings widgets.
	add_settings_widgets();

	// End adding widgets.
	scroll->end();
	end();
}

void SettingsEditor::show_and_position() {
	reload_settings();
	position(Fl::event_x_root(), Fl::event_y_root());
	show();
}

int SettingsEditor::set_file_path(const char * fpath, size_t n) {
	if (fpath == NULL)
		return -1;

	memset(file_path, 0, NAME_MAX);
	strncat(file_path, fpath, n);
	return 0;
}

int SettingsEditor::get_file_path(char * fpath, size_t n) {
	if (fpath == NULL)
		return -1;
	if (n < NAME_MAX)
		return -1;

	memset(fpath, 0, n);
	strncat(fpath, file_path, n);
	return 0;
}

int SettingsEditor::load(const char * file_path, size_t n) {
	int res = 0; // Return value.
	FILE * fp = NULL;
	SettingsFileHeader header;
	Settings new_settings;

	uint32_t settings_object_size = sizeof(Settings);

	if (file_path == NULL || n <= 0)
		return -1;

	// Open file.
	fp = fopen(file_path, "r");

	if (fp == NULL) {
		fprintf(stderr, "Error opening %s\n", file_path);
		return -1;
	}

	// Load header.
	if (fread(&header, sizeof(SettingsFileHeader), 1, fp) == -1) {
		fprintf(stderr, "Error loading header from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Check magic number.
	if (header.magic_number != SETTINGS_FILE_MAGIC_NUMBER) {
		fprintf(stderr, "Incorrect magic number in %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Check settings object size.
	if (header.settings_object_size != settings_object_size) {
		fprintf(stderr, "settings object size is not correct. current: %u, correct %u\n",
			header.settings_object_size,
			settings_object_size
		);
		res = -1;
		goto clean_mem;
	}

	// Load settings.
	if (fread(&new_settings, settings_object_size, 1, fp) == -1) {
		fprintf(stderr, "Error loading settings from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	set_file_path(file_path, n);
	mdata->settings = new_settings;
	reload_settings();
	apply_changes();

	printf("Settings loaded from %s\n", file_path);

clean_mem:
	fclose(fp);

	return res;
}

int SettingsEditor::dump(const char * file_path, size_t n) {
	int res = 0; // Return value.
	FILE * fp = NULL;
	SettingsFileHeader header;

	if (file_path == NULL || n <= 0)
		return -1;

	// Set header.
	header.magic_number = SETTINGS_FILE_MAGIC_NUMBER;
	header.settings_object_size = sizeof(Settings);

	// Open file.
	fp = fopen(file_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "Error opening %s\n", file_path);
		return -1;
	}

	// Write header.
	if (fwrite(&header, sizeof(SettingsFileHeader), 1, fp) == -1) {
		fprintf(stderr, "Error writting header to %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Write settings.
	if (fwrite(&mdata->settings, sizeof(Settings), 1, fp) == -1) {
		fprintf(stderr, "Error writting settings to %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	set_file_path(file_path, n);
	printf("Settings dumped to %s\n", file_path);

clean_mem:
	fclose(fp);

	return res;
}

void SettingsEditor::add_settings_widgets() {
	int input_width = w() / 2;
	int input_height = mdata->settings.input_height;
	
	int box_w, box_h;
	box_w = input_height * 4;
	box_h = input_height * 2;

	int spacing_size = box_h * 2;
	int input_x = (w() / 2) - (input_width / 2);

	// Speed.
	speed_box = new Fl_Box(
		(w() / 2) - (box_w / 2),
		spacing_size,
		box_w,
		box_h,
		"Speed"
	);

	speed_box->box(FL_ROUNDED_BOX);
	speed_box->labeltype(FL_SHADOW_LABEL);

	// Update fps.
	update_fps_input = new Fl_Input(
		input_x,
		speed_box->y() + (speed_box->h() * 1.5),
		input_width,
		input_height,
		"Update fps"
	);

	update_fps_input->type(FL_FLOAT_INPUT);

	// Draw fps.
	draw_fps_input = new Fl_Input(
		input_x,
		update_fps_input->y() + update_fps_input->h(),
		input_width,
		input_height,
		"Draw fps"
	);

	draw_fps_input->type(FL_FLOAT_INPUT);

	// Player speed.
	player_speed_input = new Fl_Input(
		input_x,
		draw_fps_input->y() + draw_fps_input->h(),
		input_width,
		input_height,
		"Player speed"
	);

	player_speed_input->type(FL_FLOAT_INPUT);

	// Player update speed.
	player_update_speed_input = new Fl_Input(
		input_x,
		player_speed_input->y() + player_speed_input->h(),
		input_width,
		input_height,
		"Player fps"
	);

	player_update_speed_input->type(FL_FLOAT_INPUT);

	// Video.
	video_box = new Fl_Box(
		(w() / 2) - (box_w / 2),
		player_update_speed_input->y() + spacing_size,
		box_w,
		box_h,
		"Video"
	);

	video_box->box(FL_ROUNDED_BOX);
	video_box->labeltype(FL_SHADOW_LABEL);

	// Scale.
	scale_input = new Fl_Counter(
		input_x,
		video_box->y() + (video_box->h() * 1.5),
		input_width,
		input_height,
		"Scale"
	);

	scale_input->minimum(1.0);
	scale_input->maximum(MAX_SCALE);
	scale_input->step(1.0);
	scale_input->type(FL_SIMPLE_COUNTER);

	// Background color.
	background_color_input = new Fl_Input(
		input_x,
		scale_input->y() + (scale_input->h() * 2),
		input_width,
		input_height,
		"BG color"
	);

	background_color_input->type(FL_INT_INPUT);

	// View overscan.
	view_overscan_input = new Fl_Input(
		input_x,
		background_color_input->y() + background_color_input->h(),
		input_width,
		input_height,
		"View overscan"
	);

	view_overscan_input->type(FL_INT_INPUT);

	// Diolog size.
	diolog_width_input = new Fl_Input(
		input_x,
		view_overscan_input->y() + view_overscan_input->h(),
		input_width,
		input_height,
		"Diolog width"
	);

	diolog_height_input = new Fl_Input(
		input_x,
		diolog_width_input->y() + diolog_width_input->h(),
		input_width,
		input_height,
		"Diolog height"
	);

	diolog_width_input->type(FL_INT_INPUT);
	diolog_height_input->type(FL_INT_INPUT);

	// Input height.
	input_height_input = new Fl_Input(
		input_x,
		diolog_height_input->y() + diolog_height_input->h(),
		input_width,
		input_height,
		"Input height"
	);

	input_height_input->type(FL_INT_INPUT);

	// Scheme input.
	scheme_input = new Fl_Choice(
		input_x,
		input_height_input->y() + input_height_input->h(),
		input_width,
		input_height,
		"Scheme"
	);

	for (auto s : SCHEMES)
		scheme_input->add(s);

	// Tab menu.
	tab_menu_box = new Fl_Box(
		(w() / 2) - (box_w / 2),
		scheme_input->y() + spacing_size,
		box_w,
		box_h,
		"Tab menu"
	);

	tab_menu_box->box(FL_ROUNDED_BOX);
	tab_menu_box->labeltype(FL_SHADOW_LABEL);

	// Tab menu thickness.
	tab_menu_thickness_input = new Fl_Input(
		input_x,
		tab_menu_box->y() + (tab_menu_box->h() * 1.5),
		input_width,
		input_height,
		"Thickness"
	);

	tab_menu_thickness_input->type(FL_INT_INPUT);

	// Tab menu tab size.
	tab_menu_tab_size_input = new Fl_Input(
		input_x,
		tab_menu_thickness_input->y() + tab_menu_thickness_input->h(),
		input_width,
		input_height,
		"Tab size"
	);

	tab_menu_tab_size_input->type(FL_INT_INPUT);

	// Tab menu location.
	tab_menu_locat_input = new Fl_Choice(
		input_x,
		tab_menu_tab_size_input->y() + tab_menu_tab_size_input->h(),
		input_width,
		input_height,
		"Location"
	);

	// Use 'MENU_SIDES' for order.
	tab_menu_locat_input->add("Right");
	tab_menu_locat_input->add("Left");
	tab_menu_locat_input->add("Top");
	tab_menu_locat_input->add("Bottom");
	tab_menu_locat_input->add("Fill");
	tab_menu_locat_input->add("Hidden");

	// Tab menu default hidden.
	tab_menu_default_hidden_input = new Fl_Check_Button(
		input_x,
		tab_menu_locat_input->y() + tab_menu_locat_input->h(),
		input_width,
		input_height,
		"Default hidden"
	);

	// Settings editor.
	settings_editor_box = new Fl_Box(
		(w() / 2) - (box_w / 2),
		tab_menu_default_hidden_input->y() + spacing_size,
		box_w,
		box_h,
		"Settings editor"
	);

	settings_editor_box->box(FL_ROUNDED_BOX);
	settings_editor_box->labeltype(FL_SHADOW_LABEL);

	// Settings editor width.
	settings_editor_width_input = new Fl_Input(
		input_x,
		settings_editor_box->y() + (settings_editor_box->h() * 1.5),
		input_width,
		input_height,
		"Width"
	);

	settings_editor_width_input->type(FL_INT_INPUT);

	// Settings editor height.
	settings_editor_height_input = new Fl_Input(
		input_x,
		settings_editor_width_input->y() + settings_editor_width_input->h(),
		input_width,
		input_height,
		"Height"
	);

	settings_editor_height_input->type(FL_INT_INPUT);
}

void SettingsEditor::apply_changes() {
	char buf[NAME_MAX];
	int int_value;

	TabMenu * tab_menu = (TabMenu*)mdata->tab_menu;

	// Speed.
	mdata->settings.update_fps = strtof(update_fps_input->value(), NULL);
	mdata->settings.draw_fps = strtof(draw_fps_input->value(), NULL);
	mdata->settings.player_speed = strtof(player_speed_input->value(), NULL);
	mdata->settings.player_update_speed = strtof(player_update_speed_input->value(), NULL);

	if (mdata->player != NULL)
		mdata->player->set_speed(mdata->settings.player_speed);

	// Video.
	mdata->settings.scale = (int)scale_input->value();
	mdata->settings.background_color = gameTools::valuestr_to_int(background_color_input);
	mdata->settings.view_overscan = gameTools::valuestr_to_int(view_overscan_input);
	mdata->settings.diolog_width = gameTools::valuestr_to_int(diolog_width_input);
	mdata->settings.diolog_height = gameTools::valuestr_to_int(diolog_height_input);
	mdata->settings.input_height = gameTools::valuestr_to_int(input_height_input);
	mdata->settings.scheme_id = scheme_input->value();

	// Apply scheme.
	if (mdata->settings.scheme_id >= 0 && mdata->settings.scheme_id < SCHEMES_SIZE)
		Fl::scheme(SCHEMES[mdata->settings.scheme_id]);
	
	gameTools::scale_all(mdata);

	// Tab menu.
	mdata->settings.tab_menu_thickness = gameTools::valuestr_to_int(tab_menu_thickness_input);
	mdata->settings.tab_menu_tab_size = gameTools::valuestr_to_int(tab_menu_tab_size_input);
	mdata->settings.tab_menu_locat = (MENU_SIDE)tab_menu_locat_input->value();
	mdata->settings.tab_menu_default_hidden = (bool)tab_menu_default_hidden_input->value();

	if (tab_menu != NULL) {
		tab_menu->menu_side(mdata->settings.tab_menu_locat);

		if (mdata->settings.tab_menu_default_hidden)
			tab_menu->menu_side(MENU_HIDDEN);
	}

	// Settings editor.
	mdata->settings.settings_editor.width = gameTools::valuestr_to_int(settings_editor_width_input);
	mdata->settings.settings_editor.height = gameTools::valuestr_to_int(settings_editor_height_input);
	
	size(
		mdata->settings.settings_editor.width,
		mdata->settings.settings_editor.height
	);

	scroll->size(
		mdata->settings.settings_editor.width,
		mdata->settings.settings_editor.height
	);

	redraw();
	mdata->win->redraw();
}

void SettingsEditor::reload_settings() {
	char buf[NAME_MAX];

	// Speed.
	update_fps_input->value(gcvt(mdata->settings.update_fps, FLT_DIG, buf));
	draw_fps_input->value(gcvt(mdata->settings.draw_fps, FLT_DIG, buf));
	player_speed_input->value(gcvt(mdata->settings.player_speed, FLT_DIG, buf));
	player_update_speed_input->value(gcvt(mdata->settings.player_update_speed, FLT_DIG, buf));

	// Video.
	scale_input->value(mdata->settings.scale);

	snprintf(buf, NAME_MAX, "0x%x", mdata->settings.background_color);
	background_color_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.view_overscan);
	view_overscan_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.diolog_width);
	diolog_width_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.diolog_height);
	diolog_height_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.input_height);
	input_height_input->value(buf);

	scheme_input->value(mdata->settings.scheme_id);

	// Tab menu.
	snprintf(buf, NAME_MAX, "%d", mdata->settings.tab_menu_thickness);
	tab_menu_thickness_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.tab_menu_tab_size);
	tab_menu_tab_size_input->value(buf);

	tab_menu_locat_input->value(mdata->settings.tab_menu_locat);
	tab_menu_default_hidden_input->value(mdata->settings.tab_menu_default_hidden);

	// Settings editor.
	snprintf(buf, NAME_MAX, "%d", mdata->settings.settings_editor.width);
	settings_editor_width_input->value(buf);

	snprintf(buf, NAME_MAX, "%d", mdata->settings.settings_editor.height);
	settings_editor_height_input->value(buf);
}

// Callbacks.
void SettingsEditor::apply_cb(Fl_Widget * w, void * d) {
	SettingsEditor * settings_editor = (SettingsEditor*)d;
	settings_editor->apply_changes();
}

void SettingsEditor::reload_cb(Fl_Widget * w, void * d) {
	SettingsEditor * settings_editor = (SettingsEditor*)d;
	settings_editor->reload_settings();
}

void SettingsEditor::load_file_cb(Fl_Widget * w, void * d) {
	SettingsEditor * settings_editor = (SettingsEditor*)d;
	MainData * mdata = settings_editor->mdata;
	Fl_Native_File_Chooser * file_chooser = settings_editor->file_chooser;

	size_t file_name_size;
	const char * file_name = NULL;

	if (file_chooser == NULL) {
		fputs("File chooser is null\n", stderr);
		return;
	}

	// Config 'file_chooser'.
	file_chooser->type(Fl_Native_File_Chooser::BROWSE_FILE);
	file_chooser->title("Load settings");
	file_chooser->directory(mdata->MAIN_DIR);

	// Show file chooser.
	switch (file_chooser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_chooser->errmsg());
			return;
		case 1: // cancel.
			return;
		default:
			file_name = file_chooser->filename();
			break;
	}

	if (file_name == NULL)
		return;

	// Get size.
	file_name_size = strnlen(file_name, NAME_MAX);

	// Load file.
	settings_editor->load(file_name, file_name_size);
}

void SettingsEditor::save_file_cb(Fl_Widget * w, void * d) {
	SettingsEditor * settings_editor = (SettingsEditor*)d;
	char file_path[NAME_MAX];

	// Get file path.
	if (settings_editor->get_file_path(file_path, NAME_MAX) == -1) {
		fputs("Cant get file path\n", stderr);
		return;
	}

	// Call save as if no file path.
	if (file_path[0] == '\0') {
		settings_editor->save_as_file_cb(NULL, d);
		return;
	}

	// Save.
	settings_editor->dump(file_path, NAME_MAX);
}

void SettingsEditor::save_as_file_cb(Fl_Widget * w, void * d) {
	SettingsEditor * settings_editor = (SettingsEditor*)d;
	MainData * mdata = settings_editor->mdata;
	Fl_Native_File_Chooser * file_chooser = settings_editor->file_chooser;

	size_t file_name_size;
	const char * file_name = NULL;

	if (file_chooser == NULL) {
		fputs("File chooser is null\n", stderr);
		return;
	}

	// Config 'file_chooser'.
	file_chooser->type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	file_chooser->title("Save as");
	file_chooser->directory(mdata->MAIN_DIR);

	// Show file chooser.
	switch (file_chooser->show()) {
		case -1: // Error
			fprintf(stderr, "Error with Fl_Native_File_Chooser: %s\n", file_chooser->errmsg());
			return;
		case 1: // cancel.
			return;
		default:
			file_name = file_chooser->filename();
			break;
	}

	if (file_name == NULL)
		return;

	// Get size.
	file_name_size = strnlen(file_name, NAME_MAX);

	// Save file.
	settings_editor->dump(file_name, file_name_size);
}
