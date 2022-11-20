#include "npc.h"
#include "character.h"
#include "program_data.h"
#include "tile.h"
#include "npc_map.h"
#include "chat_box.h"

int switch_following_direction(int dir) {
	return dir * -1;
}

void Npc::keep_position() {
	CommonTool * curr_tool = NULL;

	if (mdata->map == NULL)
		return;

	x((world_x * mdata->scale_tile_size) + mdata->map->offset_x());
	y((world_y * mdata->scale_tile_size) + mdata->map->offset_y());

	curr_tool = get_current_tool();

	// Set tool position.
	if (curr_tool != NULL) {
		if (facing_right())
			curr_tool->set_right();
		else
			curr_tool->set_left();

		curr_tool->move_to_location();
	}
}

bool Npc::is_on_view_window() {
	if (mdata->view_win == NULL)
		return false;

	keep_position();

	return gameTools::did_collide(
		x(),
		y(),
		get_width(),
		get_height(),
		0,
		0,
		mdata->view_win->w(),
		mdata->view_win->h()
	);
}

Astar::Point Npc::get_current_point() {
	if (current_point >= points.size() || current_point < 0)
		return {0, 0};

	return points[current_point];
}

bool Npc::at_end_point() {
	switch (following_direction) {
		case FOLLOW_FORWARDS:
			return current_point >= points.size();
		case FOLLOW_BACKWARDS:
			return current_point < 0;
		default:
			return true;
	}
}

bool Npc::follow_points() {
	int rx, ry;
	Astar::Point curr_point;

	rx = wx_rounded();
	ry = wy_rounded();

	dir = NO_MOVEMENT;

	// No points.
	if (points.empty())
		return false;

	// At end.
	if (at_end_point()) {

		// Handling following type.
		switch (following_type) {
			case POINT_FOLLOWING_REPEAT:
				current_point = 0;
				following_direction = FOLLOW_FORWARDS;
				break;
			case POINT_FOLLOWING_BACK_AND_FORTH:
				following_direction = switch_following_direction(following_direction);

				// Wrap 'current_point'.
				current_point = (current_point < 0) ? 0 : current_point;
				current_point = (current_point >= points.size()) ? points.size() - 1 : current_point;
				break;
			case POINT_FOLLOWING_STOP_AT_END:
				break;
		}

		return true;
	}

	curr_point = get_current_point();

	// Next point.
	if (rx == curr_point.x && ry == curr_point.y) {
		current_point += following_direction;
		return false;
	}

	// Right and left.
	if (rx < curr_point.x)
		dir.right = true;
	else if (rx > curr_point.x)
		dir.left = true;

	// Up and down.
	if (ry > curr_point.y)
		dir.up = true;
	else if (ry < curr_point.y)
		dir.down = true;

	return false;
}


int Npc::load_follow_data_from_file(const char * file_name, bool from_script_location) {
	long int i, j;
	int res = 0;
	FILE * fp = NULL;

	bool use_script_location;

	char file_path[NAME_MAX];

	size_t file_buf_size;
	char * file_buf = NULL;

	char line_buf[NAME_MAX];

	long int start_at;

	// This stores the type, direction, and start point.
	int follow_data_info[3];

	std::vector<Astar::Point> points;

	char x_buf[NAME_MAX];
	char y_buf[NAME_MAX];

	int curr_x, curr_y;

	bool getting_x = true;

	if (file_name == NULL)
		return -1;

	// Should use script location.
	use_script_location = false;

	if (from_script_location && mdata->chat_box != NULL)
		if (mdata->chat_box->get_script_location()[0] != '\0')
			use_script_location = true;

	// Get file path.
	memset(file_path, 0, NAME_MAX);

	if (use_script_location)
		snprintf(
			file_path,
			NAME_MAX,
			"%s%s",
			mdata->chat_box->get_script_location(),
			file_name
		);

	else
		strncat(file_path, file_name, NAME_MAX - 1);

	// Open file.
	fp = fopen(file_path, "r");

	if (fp == NULL) {
		fprintf(stderr, "Error opening %s\n", file_path);
		return -1;
	}

	// Create 'file_buf'.
	file_buf_size = gameTools::get_file_size(fp);
	file_buf = new char[file_buf_size];

	if (file_buf == NULL) {
		fputs("Error getting memory for 'file_buf'\n", stderr);
		res = -1;
		goto clean_mem;
	}

	memset(file_buf, 0, file_buf_size);

	// Read file.
	if (fread(file_buf, sizeof(char), file_buf_size, fp) == -1) {
		fprintf(stderr, "Error reading from %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Get header.
	start_at = gameTools::read_line_from_str(
		file_buf, 
		file_buf_size,
		line_buf,
		NAME_MAX
	);

	if (start_at == -1) goto clean_mem;

	// Check header.
	if (strncmp(FOLLOW_DATA_FILE_HEADER, line_buf, NAME_MAX) != 0) {
		fprintf(stderr, "Wrong header in %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Get type, direction, and current point.
	for (i = 0; i < sizeof(follow_data_info) / sizeof(int); i++) {

		// Get line.
		start_at = gameTools::read_line_from_str(
			file_buf, 
			file_buf_size,
			line_buf,
			NAME_MAX,
			start_at
		);

		if (start_at == -1) goto clean_mem;

		// Get int from line.
		if (gameTools::str_to_int(line_buf, NAME_MAX, &follow_data_info[i]) == -1) {
			fprintf(stderr, "Error getting int from %s\n", line_buf);
			res = -1;
			goto clean_mem;
		}
	}

	// Get points.
	while (start_at < file_buf_size) {

		// Get line.
		start_at = gameTools::read_line_from_str(
			file_buf, 
			file_buf_size,
			line_buf,
			NAME_MAX,
			start_at
		);

		// At end.
		if (start_at == -1) {
			start_at = 0; // Stop format error.
			break;
		}

		// Comment.
		if (line_buf[0] == '#')
			continue;

		// Get x and y.
		getting_x = true;
		j = 0;

		memset(x_buf, 0, NAME_MAX);
		memset(y_buf, 0, NAME_MAX);

		for (i = 0; i < NAME_MAX; i++) {
			if (line_buf[i] == '#') // Comment.
				break;
			if (j >= NAME_MAX) { // j to big.
				start_at = -1; // Format error.
				res = -1;
				goto clean_mem;
			} if (line_buf[i] == ' ') { // Done getting x.
				getting_x = false;
				j = 0;
				continue;
			}

			if (getting_x)
				x_buf[j] = line_buf[i];
			else
				y_buf[j] = line_buf[i];
			
			j++;
		}

		// X buf to int.
		if (gameTools::str_to_int(x_buf, NAME_MAX, &curr_x) == -1) {
			fputs("Error getting x\n", stderr);
			res = -1;
			goto clean_mem;
		}

		// Y buf to int.
		if (gameTools::str_to_int(y_buf, NAME_MAX, &curr_y) == -1) {
			fputs("Error getting y\n", stderr);
			res = -1;
			goto clean_mem;
		}

		// Add point.
		points.push_back({curr_x, curr_y});
	}

	// No points.
	if (points.empty()) {
		fprintf(stderr, "No points found in %s\n", file_path);
		res = -1;
		goto clean_mem;
	}

	// Set everything.
	this->points = points;
	this->following_type = (FOLLOWING_TYPE)follow_data_info[0];
	this->following_direction = follow_data_info[1];
	this->current_point = follow_data_info[2];

	printf("Npc follow data loaded from %s\n", file_path);

clean_mem:
	fclose(fp);

	if (file_buf != NULL)
		delete [] file_buf;

	// Format error.
	if (start_at == -1) {
		fprintf(stderr, "Format error in %s\n", file_path);
		res = -1;
	}

	return res;
}
