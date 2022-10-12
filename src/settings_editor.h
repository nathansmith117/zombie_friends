#include "program_data.h"

#define SETTINGS_FILE_MAGIC_NUMBER 27

struct SettingsFileHeader {
	uint8_t magic_number;
	uint32_t settings_object_size;
};

class SettingsEditor : public Fl_Menu_Window {
	public:
		SettingsEditor(MainData * md) : Fl_Menu_Window(0, 0, 0, 0) {
			main_init(md);
		}

		void show_and_position();

		int set_file_path(const char * fpath, size_t n);
		int get_file_path(char * fpath, size_t n);

		int load(const char * file_path, size_t n);
		int dump(const char * file_path, size_t n);

		void apply_changes();
		void reload_settings();
	protected:
		MainData * mdata;
		char file_path[NAME_MAX];

		void main_init(MainData * md);

		Fl_Native_File_Chooser * file_chooser = NULL;

		// Widgets.
		Fl_Scroll * scroll = NULL;
		Fl_Button * apply_button = NULL;
		Fl_Button * reload_button = NULL;
		Fl_Button * load_file_button = NULL;
		Fl_Button * save_file_button = NULL;
		Fl_Button * save_as_file_button = NULL;

		// Settings widgets.
		void add_settings_widgets();
		
		// Speed.
		Fl_Box * speed_box = NULL;
		Fl_Input * update_fps_input = NULL;
		Fl_Input * draw_fps_input = NULL;
		Fl_Input * player_speed_input = NULL;
		Fl_Input * player_update_speed_input = NULL;

		// Video.
		Fl_Box * video_box = NULL;
		Fl_Counter * scale_input = NULL;
		Fl_Input * background_color_input = NULL;
		Fl_Input * view_overscan_input = NULL;
		Fl_Input * diolog_width_input = NULL;
		Fl_Input * diolog_height_input = NULL;
		Fl_Input * input_height_input = NULL;

		// Tab menu.
		Fl_Box * tab_menu_box = NULL;
		Fl_Input * tab_menu_thickness_input = NULL;
		Fl_Input * tab_menu_tab_size_input = NULL;
		Fl_Choice * tab_menu_locat_input = NULL;
		Fl_Check_Button * tab_menu_default_hidden_input = NULL;

		// Settings editor.
		Fl_Box * settings_editor_box = NULL;
		Fl_Input * settings_editor_width_input = NULL;
		Fl_Input * settings_editor_height_input = NULL;

		// Call backs.
		static void apply_cb(Fl_Widget * w, void * d);
		static void reload_cb(Fl_Widget * w, void * d);
		static void load_file_cb(Fl_Widget * w, void * d);
		static void save_file_cb(Fl_Widget * w, void * d);
		static void save_as_file_cb(Fl_Widget * w, void * d);
};
