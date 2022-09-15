#pragma once

// FLTK headers.
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Check_Button.H>
#include <FL/names.h>
#include <FL/x.H>

#define _USE_MATH_DEFINES

// C/C++ headers.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <climits>
#include <vector>
#include <exception>
#include <cerrno>
#include <thread>
#include <chrono>
#include <bits/stdc++.h>

// OS headers.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef _WIN32
#include <Windows.h>
#include <Lmcons.h>
#define NAME_MAX MAX_PATH
#else
#include <unistd.h>
#endif

// Current game version. Use string.
#define GAME_VERSION "Still in building"

// M D Y format.
#define GAME_RELEASE_DATE "0 0 0"

// Editor.
#define EDITOR_VERSION "Still in building"
#define EDITOR_RELEASE_DATE "0 0 0"

// Max size stuff.
#define MAIN_DIR_SIZE NAME_MAX
#define LOG_PATH_MAX NAME_MAX * 3

#ifdef _WIN32
#define MAIN_DIR_FORMAT "C:\\Users\\%s\\Desktop\\zombie_friends\\"
#define PSLASH "\\"
#else
#define MAIN_DIR_FORMAT "/home/%s/Desktop/zombie_friends/"
#define PSLASH "/"
#endif

// Tab menu and stuff like that.
enum MENU_SIDES {
	MENU_RIGHT,
	MENU_LEFT,
	MENU_TOP,
	MENU_BOTTOM,
	MENU_FILL, // Fill the whole window.
	MENU_HIDDEN
};

typedef uint8_t MENU_SIDE;

struct MenuSettings {
	int x = 0;
	int y = 0;
	int w = -1; // -1 for window width.
	int h = 20;
};

struct EditorSettings {
	int side_menu_width = 300;
	int tab_size = 20;

	// Scroll.
	int scrollbar_thickness = 20;
	int scrollbar_output_width = 100;
	int scrollbar_output_height = 20;

	// Input.
	int input_thinkness = 20;

	// Item command list.
	int item_command_list_button_width = 50;
	int item_command_list_button_height = 30;

	// Map start size.
	bool start_with_created_map = false;
	int start_width = 10;
	int start_height = 10;
};

struct WeaponSettings {
	float sword_speed = 0.08;
	float submachine_gun_speed = 0.0001;
};

struct Settings {
	// Speed.
	float update_fps = 60.0;
	float draw_fps = 60.0;
	float player_speed = 0.0625;
	float player_update_speed = 0.1;

	WeaponSettings weapons;

	// Scale for sprites.
	int scale = 2;

	MenuSettings menu;
	EditorSettings editor;

	// Rendering.
	Fl_Color background_color = FL_BLACK;

	// Debug window.
	int debug_win_update_speed = 5.0;
	bool show_error_as_popup = true;

	// Diologs for creating new map and stuff like that.
	int diolog_width = 300;
	int diolog_height = 150;

	int input_height = 25;

	// View window.
	int view_border_size = 50;
	int view_overscan = 2; // Drawing stuff outside of window.
	
	// Map searching.
	int map_search_overscan = 1;
	
	// Tab menu.
	int tab_menu_thickness = 200;
	int tab_menu_tab_size = 20;
	MENU_SIDE tab_menu_locat = MENU_BOTTOM;
	bool tab_menu_default_hidden = false;

	// Chat box.
	int chat_box_input_height = 30;

	// Play info.
	int player_info_output_height = 20;

	// Npc map.
	int npc_map_overscan = 2;

	// Smaller number means better collision correction but slower game.
	float collision_correction = 0.3;
};

struct WeaponImages {
	std::vector<Fl_PNG_Image*> sword;
	std::vector<Fl_PNG_Image*> submachine_gun;
};

struct NpcImages {
	std::vector<Fl_PNG_Image*> evil_puppy;
};

struct GameImages {
	std::vector<Fl_PNG_Image*> player;

	WeaponImages weapons;
	NpcImages npc;

	std::vector<Fl_PNG_Image*> basic_world;
	std::vector<Fl_PNG_Image*> curr_world;
	std::vector<Fl_PNG_Image*> addon_images;

	Fl_PNG_Image * icon = NULL;
};

struct MainData {
	Settings settings;

	Fl_Double_Window * win = NULL;
	Fl_Double_Window * view_win = NULL;

	class DebugWindow * debug_win = NULL;
	class ChatBox * chat_box = NULL;

	// Menu (For editor if not added to game later).
	Fl_Menu_Bar * top_menu = NULL;

	// Tab menu.
	Fl_Tabs * tab_menu = NULL;

	// Also editor only stuff.
	Fl_Scrollbar * map_vertical_scrollbar = NULL;
	Fl_Scrollbar * map_horizontal_scrollbar = NULL;

	// Scrollbar value display.
	Fl_Output * map_vertical_scrollbar_output = NULL;
	Fl_Output * map_horizontal_scrollbar_output = NULL;

	// For menu diologs.
	struct EditMenuDiologs * edit_menu_diologs = NULL;

	// TILE_SIZE * scale.
	int scale_tile_size;

	// Maps.
	class Map * map = NULL;

	// Player.
	class Character * player = NULL;

	// Tool display.
	class PlayerInfoDisplay * player_info_display = NULL;

	// Images.
	GameImages images;
	GameImages scaled_images;

	// Username and main dir.
	char MAIN_DIR[MAIN_DIR_SIZE];
#ifdef _WIN32
	DWORD user_name_len = (UNLEN + 1);
	TCHAR user_name[(UNLEN + 1)];
#else
	size_t user_name_len = LOGIN_NAME_MAX / 2;
	char user_name[LOGIN_NAME_MAX / 2];
#endif

	struct tm * timeinfo;
	char log_file_path[LOG_PATH_MAX];
	char error_log_file_path[LOG_PATH_MAX];
};
