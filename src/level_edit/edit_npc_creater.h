#pragma once

#include "program_data.h"
#include "npc.h"
#include "npc_map.h"
#include "map.h"
#include "game_tools.h"

#define NPC_TOOL_EDITOR_COLS NPC_DATA_TOOLS_SIZE
#define NPC_TOOL_EDITOR_ROWS 2

class EditNpcToolEditor : public Fl_Group {
	public:
		EditNpcToolEditor(int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(X, Y, W, H);
		}

		~EditNpcToolEditor();

		int set_to_npc_data(NpcData * npc_data);
		int get_from_npc_data(NpcData npc_data);
	private:
		void main_init(int X, int Y, int W, int H);

		Fl_Input *** inputs = NULL;
};

class EditNpcCreater : public Fl_Group {
	public:
		EditNpcCreater(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		~EditNpcCreater();

		void apply_npc(int x, int y);
		void reset_npc_preview();

		NpcData npc() { return current_npc; }
		void npc(NpcData current_npc) { this->current_npc = current_npc; }
	private:
		MainData * mdata;
		Map * map = NULL;
		NpcMap * npc_map = NULL;

		Npc * npc_preview = NULL;

		NpcData current_npc;

		// Widgets.
		Fl_Input * type_input = NULL;
		Fl_Input * health_input = NULL;
		Fl_Input * coins_input = NULL;
		Fl_Button * enter_button = NULL;
		EditNpcToolEditor * tools_and_fuel_input = NULL;

		void main_init(MainData * md, int X, int Y, int W, int H);

		// Callbacks.
		static void type_input_cb(Fl_Widget * w, void * d);
		static void health_input_cb(Fl_Widget * w, void * d);
		static void coins_input_cb(Fl_Widget * w, void * d);
		static void tools_and_fuel_cb(Fl_Widget * w, void * d);
		static void enter_button_cb(Fl_Widget * w, void * d);
};
