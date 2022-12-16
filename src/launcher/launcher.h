#pragma once

#include "program_data.h"
#include "dll_format.h"
#include "dll_loader.h"

class Launcher {
	public:
		Launcher();
		~Launcher();

		int load(const char * file_path);
		void close();

		const char * get_path() { return dll_path; }
		bool is_loaded() { return game_handle != NULL; }

		int run(GameArgs args, bool threaded_mode=false);
		bool run_has_failed() { return run_failed; }
		int test();

		void wait_for_game_to_close();
		void wait_for_game_to_start();

		void * get_item(int item_id, void * return_on_error=NULL, bool print_errors=true);

		// It is more usefull then you would think.
		static int int_nothing(...) { return -1; }
		static void void_nothing(...) {}
		static char * char_pointer_nothing(...) { return NULL; }
	protected:
		char dll_path[NAME_MAX];
		DllLoader::HANDLE game_handle = NULL;

		ADDR_LIST_SIZE address_list_size;
		ADDR_LIST address_list = NULL;

		bool run_failed = false;

		// Threaded mode.
		std::thread * run_thread = NULL;
		void stop_thread();
		static void run_cb(GameArgs args, Launcher * launcher);
};
