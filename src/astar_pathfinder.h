#include "program_data.h"
#include "game_tools.h"
#include "tile.h"
#include "map.h"

namespace Astar {
	struct Point {
		int x, y;

		bool operator==(Point p) {
			return x == p.x && y == p.y;
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

		void print_info() {
			printf("x: %d, y: %d, g: %d, h: %d, f: %d\n", pos.x, pos.y, g, h, f());
		}
	};

	int get_dis(Point p1, Point p2);
	std::vector<Point> find_path(Map * map, Point start_point, Point end_point);
};
