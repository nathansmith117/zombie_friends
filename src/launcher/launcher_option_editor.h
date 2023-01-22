#pragma once

#include "program_data.h"
#include "launcher.h"

union LauncherOptionInput {
	Fl_Input * input;
	Fl_Choice * choice;
	Fl_Check_Button * check;
};

enum LAUNCHER_OPTION_INPUT_TYPES {
	LAUNCHER_OPTION_INPUT,
	LAUNCHER_OPTION_CHOICE,
	LAUNCHER_OPTION_CHECK
};

// Format for each line in the options file.
struct LauncherOptionData {
	char option[NAME_MAX];
	LauncherOptionInput input;
	int8_t input_type;

	LauncherOptionData(const char * option, LauncherOptionInput input, int8_t input_type) {
		memset(this->option, 0, NAME_MAX);
		strncat(this->option, option, NAME_MAX - 1);

		this->input = input;
		this->input_type = input_type;
	}

	LauncherOptionData() {
		memset(this->option, 0, NAME_MAX);
	}
};

struct LaunchOptions {
	char launcher_dll[NAME_MAX];
	char main_dir[NAME_MAX];
	char launcher[NAME_MAX];
	char startup_script[NAME_MAX];

	LaunchOptions() {
		clear();
	}

	void clear() {
		memset(launcher_dll, 0, NAME_MAX);
		memset(main_dir, 0, NAME_MAX);
		memset(launcher, 0, NAME_MAX);
		memset(startup_script, 0, NAME_MAX);
	}
};

// A file path tag goes infront of the path and gets set to something else.
struct FilePathTag {
	char tag[NAME_MAX];
	const char * value = NULL;

	FilePathTag(const char * tag, const char * value) {
		memset(this->tag, 0, NAME_MAX);
		strncat(this->tag, tag, NAME_MAX - 1);

		this->value = value;
	}

	FilePathTag() {
		memset(this->tag, 0, NAME_MAX - 1);
	}
};

// Launcher option file format.
// Option value
// Option value
// ...

class LauncherOptionEditor : public Fl_Group {
	public:
		LauncherOptionEditor(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		void reset_size();

		int dump_options(const char * file_path);
		int load_options(const char * file_path);

		void clear_option_inputs();

		// Returns false if there is no option file.
		bool get_option_filepath(char * file_path); // NAME_MAX for file_path size.

		int get_launch_options(LaunchOptions * launch_options);
	private:
		MainData * mdata;
		void main_init(MainData * md, int X, int Y, int W, int H);

		void clear_option_input(LauncherOptionData data);

		int handle_tag_in_filepath(char * file_path);
		int replace_tag_with_value(FilePathTag tag, char * file_path);

		// Location of the option file.
		char option_file_dir[NAME_MAX] = "\0";

		char default_launcher[NAME_MAX];

		// For writting to options file.
		// NAME_MAX for size.
		static int format_option_data(LauncherOptionData option_data, char * output_buf);

		// For reading from options file.
		int read_option_line(const char * line);
		int handle_option_and_value(const char * option_buf, const char * value_buf);

		void refresh_options();

		Fl_Native_File_Chooser * file_browser = NULL;

		Fl_File_Browser * instance_chooser = NULL;
		Fl_Input * instance_chooser_dir_input = NULL;
		Fl_Button * instance_chooser_dir_browser = NULL;

		Fl_Button * refresh_instance_chooser = NULL;

		Fl_Input * launcher_dll_input = NULL;
		Fl_Button * launcher_dll_browser = NULL;

		Fl_Input * main_dir_input = NULL;
		Fl_Button * main_dir_browser = NULL;

		Fl_Input * launcher_location_input = NULL;
		Fl_Button * launcher_location_browser = NULL;

		Fl_Input * startup_script_input = NULL;
		Fl_Button * startup_script_browser = NULL;

		Fl_Button * dump_button = NULL;
		Fl_Button * launch_button = NULL;

		Fl_Input * new_options = NULL;
		Fl_Button * create_new_options = NULL;

		std::vector<LauncherOptionData> option_inputs;
		std::vector<FilePathTag> file_path_tags;

		// Callbacks.
		static void file_browser_cb(Fl_Widget * w, void * d);
		static void directory_browser_cb(Fl_Widget * w, void * d);
		static void launch_cb(Fl_Widget * w, void * d);
		static void instance_chooser_cb(Fl_Widget * w, void * d);
		static void dump_options_cb(Fl_Widget * w, void * d);
		static void refresh_instance_chooser_cb(Fl_Widget * w, void * d);
		static void new_options_cb(Fl_Widget * w, void * d);
};