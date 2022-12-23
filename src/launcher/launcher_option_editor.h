#pragma once

#include "program_data.h"
#include "launcher.h"

class LauncherOptionEditor : public Fl_Group {
	public:
		LauncherOptionEditor(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		void reset_size();

		int dump_options(const char * file_path);
		int load_options(const char * file_path);
	private:
		MainData * mdata;
		void main_init(MainData * md, int X, int Y, int W, int H);

		Fl_Native_File_Chooser * file_browser = NULL;

		Fl_Input * launcher_dll_input = NULL;
		Fl_Button * launcher_dll_browser = NULL;

		Fl_Input * main_dir_input = NULL;
		Fl_Button * main_dir_browser = NULL;

		Fl_Input * launcher_location_input = NULL;
		Fl_Button * launcher_location_browser = NULL;

		Fl_Input * startup_script_input = NULL;
		Fl_Button * startup_script_browser = NULL;

		Fl_Button * launch_button = NULL;

		Fl_Button * dump_options_button = NULL;
		Fl_Button * load_options_button = NULL;

		std::vector<Fl_Input*> option_inputs;

		// Callbacks.
		static void file_browser_cb(Fl_Widget * w, void * d);
		static void directory_browser_cb(Fl_Widget * w, void * d);
		static void launch_cb(Fl_Widget * w, void * d);

		static void dump_options_cb(Fl_Widget * w, void * d);
		static void load_options_cb(Fl_Widget * w, void * d);
};
