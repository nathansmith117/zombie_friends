#include "astar_pathfinder.h"
#include "npc_map.h"

namespace Astar {
	int get_dis(Point p1, Point p2) {
		return (int)(hypotf(p2.x - p1.x, p2.y - p1.y) * 10.0);
	}

	std::vector<Point> find_path(Map * map, Point start_point, Point end_point, int safe_zone_width, int safe_zone_height, bool go_around_characters, int scale_tile_size) {
		int i;
		int x, y;
		int new_x, new_y;

		int hypot_safe_zone_size;

		bool in_closed, in_open;

		std::vector<Point> points;
		std::vector<Point> points_rev;

		std::vector<Node*> open_nodes;
		std::vector<Node*> closed_nodes;

		Node * curr_node = NULL;
		Node * current = NULL;
		Node * new_node = NULL;

		Point new_pos;

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
		bool close_to_end;

		while (!open_nodes.empty()) {
			curr_node = open_nodes[0];
			node_index = 0;

			// Get node with lowest f cost.
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

					in_closed = false;
					in_open = false;
					close_to_end = false;

					new_pos = {new_x, new_y};

					if (go_around_characters)
						if (point_hit_npc(
							map, 
							scale_tile_size, 
							new_pos, 
							start_point, 
							end_point, 
							safe_zone_width, 
							safe_zone_height))
							continue;

					// Check safe zone.
					// Safe zone not used when close to end.
					if (safe_zone_width > 0 || safe_zone_height > 0) {
						close_to_end = gameTools::did_collide(
							new_pos.x - (safe_zone_width / 2),
							new_pos.y - (safe_zone_height / 2),
							safe_zone_width,
							safe_zone_height,
							end_point.x,
							end_point.y,
							1,
							1
						);

						if (!is_walkable_zone(map, new_pos, safe_zone_width, safe_zone_height) && !close_to_end)
							continue;
					}

					// Is in closed.
					for (auto n : closed_nodes)
						if (new_pos == n->pos) {
							in_closed = true;
							break;
						}

					if (in_closed)
						continue;

					// Is in open.
					for (auto n : open_nodes)
						if (new_pos == n->pos) {
							in_open = true;
							break;
						}

					if (in_open)
						continue;

					// Add new node.
					new_node = new Node();
					new_node->pos = new_pos;
					new_node->parent = curr_node;
					new_node->g = curr_node->g + get_dis(curr_node->pos, new_node->pos);
					new_node->h = get_dis(new_node->pos, end_point);

					open_nodes.push_back(new_node);
				}
		}

		return points;
	}

	bool is_walkable_zone(Map * map, Point pos, int safe_zone_width, int safe_zone_height) {
		int x, y;
		int map_x, map_y;
		Tile::TileObject curr_tile;

		// Check stuff.
		if (map == NULL)
			return false;
		if (map->map_data() == NULL)
			return false;
		if (safe_zone_width <= 0 && safe_zone_height <= 0)
			return true;

		// Check map around pos.
		for (y = -safe_zone_height; y < safe_zone_height + 1; y++)
			for (x = -safe_zone_width; x < safe_zone_width + 1; x++) {
				map_x = pos.x + x;
				map_y = pos.y + y;

				curr_tile = map->tile(map_x, map_y);

				if (curr_tile.id == Tile::NONE)
					continue;

				// Is walkable.
				if ((curr_tile.type & Tile::TYPE_NO_WALKTHROUGH) == Tile::TYPE_NO_WALKTHROUGH)
					return false;
			}

		return true;
	}

	bool point_hit_npc(Map * map, int scale_tile_size, Point pos, Point start, Point end, int safe_zone_width, int safe_zone_height) {
		int pos_pixel_x, pos_pixel_y;
		int pos_width, pos_height;
		Point char_point;

		if (map == NULL || scale_tile_size <= 0)
			return false;

		pos_pixel_x = pos.x * scale_tile_size;
		pos_pixel_y = pos.y * scale_tile_size;
		pos_width = scale_tile_size + (scale_tile_size * safe_zone_width);
		pos_height = scale_tile_size + (scale_tile_size * safe_zone_height);

		pos_pixel_x -= (safe_zone_width > 0) ? pos_width / 2 : 0;
		pos_pixel_y -= (safe_zone_height > 0) ? pos_height / 2 : 0;

		// Npcs.
		for (auto n : map->get_npc_map()->get_npcs_in_use()) {
			if (n == NULL)
				continue;

			char_point = get_character_point(n);

			// Same as start or end.
			if (char_point == start || char_point == end)
				continue;

			if (gameTools::did_collide(
				pos_pixel_x,
				pos_pixel_y,
				pos_width,
				pos_height,
				roundf((float)n->wx() * scale_tile_size),
				roundf((float)n->wy() * scale_tile_size),
				n->get_width(),
				n->get_height()))
				return true;
		}

		return false;
	}
	
	Point get_character_point(Character * character) {
		if (character == NULL)
			return {0, 0};

		return {character->wx_rounded(), character->wy_rounded()};
	}

	// PathFinder stuff.
	PathFinder::PathFinder(MainData * md) {
		mdata = md;

		target = {0, 0};
		target_at_start = {0, 0};
		reset();
	}

	PathFinder::~PathFinder() {
		stop_thread();
	}

	void PathFinder::set_target(Character * character) {
		if (character == NULL)
			return;

		target = get_character_point(character);
	}

	void PathFinder::update_thread_cb(PathFinder * path_finder) {
		while (!path_finder->should_stop_thread) {
			std::this_thread::sleep_for(std::chrono::milliseconds(path_finder->settings.thread_speed));
			// If npc is not being updated.
			if (path_finder->settings.character_is_npc && path_finder->character != NULL)
				if (!((Npc*)path_finder->character)->get_being_updated())
					continue;

			path_finder->update();
		}
	}

	void PathFinder::start_thread() {

		// Already using thread.
		if (update_thread != NULL)
			return;

		should_stop_thread = false;
		update_thread = new std::thread(update_thread_cb, this);
	}

	void PathFinder::stop_thread() {
		if (update_thread == NULL)
			return;

		should_stop_thread = true;
		update_thread->join();

		delete update_thread;
		update_thread = NULL;
	}
	
	void PathFinder::reset() {
		points.clear();
		current_point = 0;
		old_target = target;
		path_failed = false;
	}

	bool PathFinder::update() {
		int char_x, char_y;
		gameTools::Direction * dir = NULL;
		Point curr_point;

		if (character == NULL)
			return false;
		if (mdata->map == NULL)
			return false;

		dir = character->direction();
		*dir = NO_MOVEMENT;

		char_x = character->wx_rounded();
		char_y = character->wy_rounded();

		// To far from target.
		if (settings.max_dis != -1)
			if (hypotf(target.x - character->wx(), target.y - character->wy()) > settings.max_dis)
				return false;

		// Target changed.
		if (target != old_target) {
			if (hypotf(target.x - target_at_start.x, target.y - target_at_start.y) >= settings.dis_intel_update) {
				start_path();
				return false;
			}
		}

		old_target = target;

		if (path_failed && !settings.try_after_path_failed)
			return false;

		// Start path.
		if (points.empty()) {
			start_path();
			return false;
		}

		// Finished.
		if (finished())
			return true;

		curr_point = points[current_point];

		// Next point.
		if (char_x == curr_point.x && char_y == curr_point.y) {
			current_point++;
			return false;
		}

		// Right and left.
		if (char_x < curr_point.x)
			dir->right = true;
		else if (char_x > curr_point.x)
			dir->left = true;

		// Up and down.
		if (char_y > curr_point.y)
			dir->up = true;
		else if (char_y < curr_point.y)
			dir->down = true;

		return false;
	}

	void PathFinder::start_path() {
		reset();

		if (character == NULL)
			return;

		points = find_path(
			mdata->map, 
			get_character_point(character), 
			target,
			settings.safe_zone_width,
			settings.safe_zone_height,
			settings.go_around_characters,
			mdata->scale_tile_size
		);

		if (points.empty())
			path_failed = true;

		target_at_start = target;
		old_target = target;
	}
}
