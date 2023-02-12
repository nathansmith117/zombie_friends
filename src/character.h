#pragma once

#include "program_data.h"
#include "tile.h"
#include "weapon.h"
#include "common_item.h"
#include "game_tools.h"
#include "map.h"
#include "common_tool.h"

#define NO_MOVEMENT {false, false, false, false}

struct CharacterHitData {
	ThingsHit things_hit;
	HIT_TYPE type = HIT_NONE;
	gameTools::Coord coord;
	bool hit_handled = false;
};

struct CharacterQuestionData {
	char question[NAME_MAX] = "\0";
	class Character * asker = NULL;
};

enum QuestionStates {
	WAITING_FOR_ANSWER,
	GOT_ANSWER,
	NO_QUESTION
};

class Character : public Fl_Widget {
	public:
		Character(MainData * md) : Fl_Widget(0, 0, 0, 0) {
			mdata = md;

			dir = NO_MOVEMENT;
			frame = 0;

			world_x = 0.0;
			world_y = 0.0;

			coins = 0;
			heath = 0;

			old_world_x = 0.0;
			old_world_y = 0.0;
			old_direction = NO_MOVEMENT;

			current_tool = 0;

			speed = 0.0;

			set_name("unnamed");

			memset(question_answer, 0, NAME_MAX);
		}

		~Character();

		virtual void draw();
		virtual void update() = 0;
		virtual void handle_hit_data() = 0;

		gameTools::Direction * direction() { return &dir; }
		gameTools::Direction get_direction() { return dir; }

		std::vector<Fl_PNG_Image*> images() { return character_images; }
		void images(std::vector<Fl_PNG_Image*> character_images);

		int get_frame() { return frame; }
		void set_frame(int frame) { this->frame = frame; }
		Fl_PNG_Image * get_current_image();

		virtual void center() {};
		virtual void use_tool();
		virtual void stop_using_tool();

		virtual void go_right() = 0;
		virtual void go_left() = 0;

		virtual bool facing_right() = 0;
		virtual bool facing_left() = 0;

		virtual void refresh_images() = 0;
		virtual void refresh_tool_images();

		// Use these for getting character size.
		virtual int get_width();
		virtual int get_height();

		// Gets or sets world position.
		float wx() { return world_x; }
		void wx(float world_x) { this->world_x = world_x; }

		float wy() { return world_y; }
		void wy(float world_y) { this->world_y = world_y; }

		int wx_rounded() { return roundf(world_x); }
		int wy_rounded() { return roundf(world_y); }

		bool is_moving();

		// Put this in update function.
		virtual void keep_position() = 0;

		bool hit_tile(Tile::TileObject tile, int x, int y);
		bool hit_item(CommonItem::ItemData item, int x, int y);
		bool hit_character(Character * character);
		std::vector<CharacterHitData> get_hit_data();

		void add_item(CommonItem::ItemData item);

		void set_heath(int heath) { this->heath = heath; }
		void add_heath(int heath_gain) { heath += heath_gain; }
		int get_heath() { return heath; }

		void set_coins(int coins) { this->coins = coins; }
		void add_coins(int coin_gain) { coins += coin_gain; }
		int get_coins() { return coins; }

		std::vector<CommonTool*> get_tools() { return tools; }
		void set_tools(std::vector<CommonTool*> tools) { this->tools = tools; }
		void clear_tools() { tools.clear(); }

		int tool_count() { return tools.size(); }
		void add_tool(CommonTool * tl) { tools.push_back(tl); }
		int insert_tool(CommonTool * tl, int pos);

		int get_current_tool_local() { return current_tool; }
		void set_current_tool_local(int current_tool);

		void move_tool_to_location();

		CommonTool * get_tool_at(int pos);
		CommonTool * get_current_tool();

		void next_tool();
		void pre_tool();

		// Return the tool id if owned or -1.
		int tool_owned(int tool_type);

		virtual void handle_collision();

		virtual void handle_collision(float obj_x, float obj_y, int obj_width, int obj_height, float correction_speed=0.0);
		virtual void handle_collision(float obj_x, float obj_y);
		virtual void handle_collision(CommonItem::ItemData item, int item_x, int item_y);
		virtual void handle_collision(Character * character);

		float get_speed() { return speed; }
		float get_scaled_speed();
		void set_speed(float speed) { this->speed = speed; }

		// NAME_MAX for size.
		void get_name(char * name_buf);
		void set_name(const char * new_name);

		// NAME_MAX for question size.
		void ask_question(const char * question); // Ask player a question.
		void answer_question(const char * answer);
		void close_question();

		// Takes list of posible answers.
		// Returns true of anwsered.
		bool wait_for_answer(const char ** answers, size_t n);
	protected:
		MainData * mdata;
		gameTools::Direction dir;
		int frame;

		// For saying something in chatbox.
		void say(const char * msg, size_t n);

		char character_name[NAME_MAX];

		std::vector<CommonTool*> tools;
		int current_tool;

		float world_x, world_y;

		int heath;
		int coins;

		float old_world_x, old_world_y;
		gameTools::Direction old_direction;

		// Question stuff.
		CharacterQuestionData question_data;
		char question_state = NO_QUESTION;
		char question_answer[NAME_MAX];

		std::vector<Fl_PNG_Image*> character_images;

		float speed;

		virtual void update_world_position(); // Put this in update.
		virtual void update_old_values(); // Put this at top of 'update_world_position'.
};
