#pragma once

#include "program_data.h"
#include "weapon.h"
#include "game_tools.h"
#include "map.h"
#include "tile.h"
#include "common_item.h"

enum BulletDirections {
	BULLET_RIGHT = 1,
	BULLET_LEFT = -1
};

typedef int8_t BULLET_DIR;

struct Bullet {
	int x, y;
	BULLET_DIR dir;
	bool removed = false;

	int start_x;
	int start_y;

	int start_map_x_offset;
	int start_map_y_offset;

	int last_map_x_offset;
	int last_map_y_offset;
};

#define BULLET_MAX_NONE -1

struct BulletHitData {
	ThingsHit things_hit;
	HIT_TYPE type = HIT_NONE;
	gameTools::Coord coord; // Coord on map not screen.
	int bullet_id;
	int bullet_count_at_time;
	Bullet bullet;
	bool hit_handled = false;
};

class Gun : public Weapon {
	public:
		Gun(MainData * md, Fl_Widget * item_holder) : Weapon(md, item_holder) {
			type = TOOL_GUN;
		}

		void draw();

		void update_bullets();

		void set_bullets(std::vector<Bullet> bullets) { this->bullets = bullets; }
		std::vector<Bullet> get_bullets() { return bullets; }
		int get_bullet_count() { return bullets.size(); }

		void set_max_bullets(int max_bullets) { this->max_bullets = max_bullets; }
		int get_max_bullets() { return max_bullets; }

		std::vector<BulletHitData> get_bullet_hit_data() { return bullet_hit_data; }

		int add_bullet();
		int add_bullets(int num);
		int remove_bullet(int num);

		bool bullet_out_of_view(Bullet b);
	protected:
		std::vector<Bullet> bullets;
		std::vector<gameTools::Coord> bullet_start_locations;
		std::vector<BulletHitData> bullet_hit_data;

		// Bullet settings.
		int bullet_w, bullet_h, bullet_speed;
		int updates_per_add_bullet = 0;
		int max_bullets = BULLET_MAX_NONE;
		Fl_Color bullet_color = FL_BLACK;

		// Bullet add speed.
		int updates_after_bullet_added = 0;
		bool can_add_bullet = true;

		void update_and_test_bullet(int bullet_id);

		// Fuel == Ammo
};
