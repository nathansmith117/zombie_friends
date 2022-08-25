#pragma once

#include "program_data.h"
#include "game_tools.h"
#include "tile.h"
#include "map.h"
#include "character.h"

namespace Astar {
	struct Point {
		int x, y;

		bool operator==(Point p) {
			return x == p.x && y == p.y;
		}

		bool operator!=(Point p) {
			return x != p.x || y != p.y;
		}
	};

	struct Node {
		Point pos;
		Node * parent = NULL;

		// g = distance from start or parent.
		// h = distance from end. 
		int g, h;

		int f() { return g + h; }

		bool operator==(Node n) {
			return pos == n.pos;
		}

		bool operator!=(Node n) {
			return pos != n.pos;
		}

		void print_info() {
			printf("x: %d, y: %d, g: %d, h: %d, f: %d\n", pos.x, pos.y, g, h, f());
		}
	};

	int get_dis(Point p1, Point p2);
	std::vector<Point> find_path(Map * map, Point start_point, Point end_point, int safe_zone_width=0, int safe_zone_height=0);

	bool is_walkable_zone(Map * map, Point pos, int safe_zone_width, int safe_zone_height);
	Point get_character_point(Character * character);

	struct PathFinderSettings {
		int dis_intel_update = 0;

		int safe_zone_width = 1;
		int safe_zone_height = 1;
	};

	class PathFinder {
		public:
			PathFinder(MainData * md);

			void set_character(Character * character) { this->character = character; }
			Character * get_character() { return character; }

			void set_target(Point target) { this->target = target; }
			void set_target(Character * character);
			Point get_target() { return target; }

			void set_settings(PathFinderSettings settings) { this->settings = settings; }
			PathFinderSettings get_settings() { return settings; }

			void set_points(std::vector<Point> points) { this->points = points; }
			std::vector<Point> get_points() { return points; }
			int get_num_of_points() { return points.size(); }

			int get_current_point_locat() { return current_point; }
			Point get_current_point() { return points[current_point]; }

			void reset();
			bool finished() { return current_point >= points.size() - 1; }

			// Return true if finished.
			bool update();
		protected:
			MainData * mdata;

			Character * character = NULL;
			
			Point target;
			Point old_target;

			PathFinderSettings settings;

			void start_path();

			std::vector<Point> points;
			int current_point;
	};
}
