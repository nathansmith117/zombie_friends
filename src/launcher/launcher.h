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

		int wait_for_game_to_close();
	protected:
		char dll_path[NAME_MAX];
		DllLoader::HANDLE game_handle = NULL;
		RUN_GAME_CB run_game_cb = NULL;

		// Threaded mode.
		std::thread * run_thread = NULL;
		void stop_thread();
		static void run_cb(GameArgs args, Launcher * launcher);
};
