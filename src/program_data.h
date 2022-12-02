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
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Table_Row.H>
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
#include <dlfcn.h>
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

#define MAX_SCALE 20

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
	int32_t height = 20;
};

// Schemes.
const char SCHEMES[][NAME_MAX] = {
	"none",
	"plastic",
	"gtk+",
	"gleam"
};

#define SCHEMES_SIZE sizeof(SCHEMES) / NAME_MAX

enum SCHEME_IDS {
	SCHEME_OTHER = -1,
	SCHEME_NONE,
	SCHEME_PLASTIC,
	SCHEME_GTK_PLUS,
	SCHEME_GLEAM
};

typedef int8_t SCHEME_ID;

enum GAME_STATES {
	GAME_NOT_STARTED,
	GAME_RUNNING,
	GAME_STOPPED
};

typedef unsigned int GAME_STATE;

struct EditorSettings {
	// Scroll.
	int32_t scrollbar_thickness = 20;
	int32_t scrollbar_output_width = 100;
	int32_t scrollbar_output_height = 20;

	// Input.
	int32_t input_thinkness = 20;

	// Item command list.
	int32_t item_command_list_button_width = 50;
	int32_t item_command_list_button_height = 30;

	// Map start size.
	bool start_with_created_map = false;
	int32_t start_width = 10;
	int32_t start_height = 10;
};

struct ProgressBarSettings {
	int32_t width = 200;
	int32_t height = 100;
};

struct SettingsEditorSettings {
	int32_t width = 480;
	int32_t height = 600;
};

// Always use stdint for settings because the data in 'Settings' will be writen to a file.
struct Settings {
	// Speed.
	float update_fps = 60.0;
	float draw_fps = 60.0;

	// Player speed.
	float player_speed = 7.5; // Tiles per second.
	float player_update_speed = 0.1;

	// Scale for sprites.
	int32_t scale = 2;

	MenuSettings menu;
	EditorSettings editor;
	ProgressBarSettings progress_bar;
	SettingsEditorSettings settings_editor;

	// Rendering.
	uint32_t background_color = FL_BLACK;

	// Debug window.
	int32_t debug_win_update_speed = 5.0;
	bool show_error_as_popup = true;

	// Diologs for creating new map and stuff like that.
	int32_t diolog_width = 300;
	int32_t diolog_height = 150;

	int32_t input_height = 25;

	// View window.
	int32_t view_overscan = 2; // Drawing stuff outside of window.
	
	// Map searching.
	int32_t map_search_overscan = 1;
	
	// Tab menu.
	int32_t tab_menu_thickness = 200;
	int32_t tab_menu_tab_size = 20;
	MENU_SIDE tab_menu_locat = MENU_BOTTOM;
	bool tab_menu_default_hidden = false;

	// Chat box.
	int32_t chat_box_input_height = 30;

	// Play info.
	int32_t player_info_output_height = 20;

	// Npc map.
	int32_t npc_map_overscan = 2;

	// Scheme.
	SCHEME_ID scheme_id = SCHEME_NONE;
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

	// Flags.
	bool should_close = false; // Make sure to close game when set.
	GAME_STATE state = GAME_NOT_STARTED; // Make sure at corrected times.

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

	// For game launcher only.
	class Launcher * launcher = NULL;

	// TILE_SIZE * scale.
	int scale_tile_size;

	// Settings editor of course.
	class SettingsEditor * settings_editor = NULL;

	// Maps.
	class Map * map = NULL;

	// Player.
	class Character * player = NULL;

	// Tool display.
	class PlayerInfoDisplay * player_info_display = NULL;

	// Images.
	GameImages images;
	GameImages scaled_images;

	int argc = 0;
	char ** argv = NULL;

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
