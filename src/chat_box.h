#pragma once

#include "program_data.h"
#include "game_tools.h"

struct VariableData {
	char name[NAME_MAX];
	char value[NAME_MAX];
};

// Command callback function, return -1 for error.
#define COM_CB_ARGS int arg_count, const char ** args, MainData * mdata, ChatBox * chat_box, std::vector<VariableData> * variable_list, void * d

typedef int (*COM_CB)(COM_CB_ARGS);

struct ChatCommand {
	char name[NAME_MAX];
	COM_CB callback;
	void * data = NULL;
	bool show_command = true;
};

class ChatBox : public Fl_Group {
	public:
		ChatBox(MainData * md, int X, int Y, int W, int H) : Fl_Group(X, Y, W, H) {
			main_init(md, X, Y, W, H);
		}

		int handle(int event);

		void printline(const char * msg);

		// n is the size of both name and msg.
		void post(const char * name, const char * msg, size_t n);
		bool is_command(const char * msg);

		void add_command(ChatCommand new_comm) { command_list.push_back(new_comm); }

		std::vector<ChatCommand> commands() { return command_list; }
		void commands(std::vector<ChatCommand> new_commands) { command_list = new_commands; }

		int add_command_list(const ChatCommand * new_command_list, size_t n);
		void add_command_list(std::vector<ChatCommand> new_command_list);

		ChatCommand command(int index) { return command_list[index]; }
		void command(ChatCommand new_comm, int index) { command_list[index] = new_comm; }

		void clear() { text_buffer->text(""); }

		int run_command(const char * comm, std::vector<VariableData> * var_list=NULL); // Size will allways be NAME_MAX.
		int get_arg_count(const char * comm_buf, size_t n);

		int run_script(const char * file_path);

		void set_script_location(const char * script_location);
		const char * get_script_location() { return script_location; }
		
		void scroll_to_bottom();

		static int variable_str_to_data(const char * variable_str, size_t n, VariableData * variable_data);
		static int add_or_set_variable(VariableData variable_data, std::vector<VariableData> * var_list);

		// NAME_MAX for variable name and value. Returns true if variable found.
		static bool get_value_from_list(const char * variable_name, std::vector<VariableData> var_list, char * variable_value);

		std::vector<VariableData> get_global_vars() { return global_vars; }
		void set_global_vars(std::vector<VariableData> global_vars) { this->global_vars = global_vars; }
		void clear_global_vars() { global_vars.clear(); }

		bool is_in_globals(VariableData var_data);
		int add_or_set_global_var(VariableData var_data);
	protected:
		MainData * mdata;
		std::vector<ChatCommand> command_list;

		// Global variables.
		std::vector<VariableData> global_vars;

		// Widgets.
		Fl_Text_Display * text_display = NULL;
		Fl_Text_Buffer * text_buffer = NULL;
		Fl_Check_Button * input_on_button = NULL;
		Fl_Input * input_line = NULL;

		// Callbacks.
		static void input_on_button_cb(Fl_Widget * w, void * d);
		static void input_line_cb(Fl_Widget * w, void * d);

		// For running scripts
		char script_location[NAME_MAX];
		
		void main_init(MainData * md, int X, int Y, int W, int H);
};
