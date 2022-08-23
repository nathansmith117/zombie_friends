#include "astar_pathfinder.h"

namespace Astar {
	int get_dis(Point p1, Point p2) {
		return (int)(hypotf(p2.x - p1.x, p2.y - p1.y) * 10.0);
	}

	std::vector<Point> find_path(Map * map, Point start_point, Point end_point) {
		int i;
		int x, y;
		int new_x, new_y;

		bool in_closed, in_open;

		std::vector<Point> points;
		std::vector<Point> points_rev;

		std::vector<Node*> open_nodes;
		std::vector<Node*> closed_nodes;

		Node * curr_node = NULL;
		Node * current = NULL;
		Node * new_node = NULL;

		if (map == NULL)
			return points;
		if (map->map_data() == NULL)
			return points;

		// Add start node.
		new_node = new Node;
		new_node->pos = start_point;
		new_node->g = 0;
		new_node->h = get_dis(start_point, end_point);
		open_nodes.push_back(new_node);

		int node_index;

		while (!open_nodes.empty()) {
			curr_node = open_nodes[0];

			// Get node with lowest f cost.
			node_index = 0;

			for (i = 0; i < open_nodes.size(); i++)
				if (open_nodes[i]->f() < curr_node->f()) {
					curr_node = open_nodes[i];
					node_index = i;
				}

			open_nodes.erase(open_nodes.begin() + node_index);
			closed_nodes.push_back(curr_node);

			// At end.
			if (curr_node->pos == end_point) {
				current = curr_node;

				while (current != NULL) {
					points.push_back(current->pos);
					current = current->parent;
				}

				// Revserse points.
				for (i = points.size() - 1; i >= 0; i--)
					points_rev.push_back(points[i]);

				// Clean memory.
				for (auto n : open_nodes)
					delete n;
				for (auto n : closed_nodes)
					delete n;

				printf("Open size: %ld, closed size: %ld\n", open_nodes.size(), closed_nodes.size());
				return points_rev;
			}

			// Create child nodes.
			for (y = -1; y < 2; y++)
				for (x = -1; x < 2; x++) {
					new_x = curr_node->pos.x + x;
					new_y = curr_node->pos.y + y;

					// Same as 'curr_node'.
					if (new_x == 0 && new_y == 0)
						continue;

					// Out of range.
					if (new_x >= map->get_width() || new_x < 0)
						continue;
					if (new_y >= map->get_height() || new_y < 0)
						continue;

					// Not walkable.
					if ((map->tile(new_x, new_y).type & Tile::TYPE_NO_WALKTHROUGH) == Tile::TYPE_NO_WALKTHROUGH)
						continue;

					// Add new node.
					new_node = new Node();
					new_node->pos = {new_x, new_y};
					new_node->parent = curr_node;
					new_node->g = curr_node->g + get_dis(curr_node->pos, new_node->pos);
					new_node->h = get_dis(new_node->pos, end_point);

					in_closed = false;
					in_open = false;

					// Is in closed.
					for (auto n : closed_nodes)
						if (new_node->pos == n->pos) {
							in_closed = true;
							break;
						}

					if (in_closed) {
						delete new_node;
						continue;
					}

					// Is in open.
					for (auto n : open_nodes)
						if (new_node->pos == n->pos) {
							in_open = true;
							break;
						}

					if (in_open) {
						delete new_node;
						continue;
					}

					open_nodes.push_back(new_node);
				}
		}

		return points;
	}
};
