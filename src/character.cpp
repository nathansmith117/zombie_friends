#include "character.h"
#include "npc_map.h"
#include "id_to_object.h"
#include "chat_box.h"

void Character::draw() {
	Fl_PNG_Image * current_image;
	current_image = get_current_image();
	CommonTool * tl = get_current_tool();

	if (current_image == NULL)
		return;

	current_image->draw(x(), y());

	// Draw weapon.
	if (tl == NULL)
		return;

	tl->draw();
}

Character::~Character() {
	delete_tools();
	delete_images();
}

void Character::delete_tools() {
	for (auto t : tools)
		if (t != NULL)
			delete t;

	tools.clear();
}

void Character::refresh_tool_images() {
	for (auto tl : tools)
		if (tl != NULL)
			tl->refresh_images();
}

bool Character::is_moving() {
	return dir.right || dir.left || dir.up || dir.down;
}

bool Character::hit_tile(Tile::TileObject tile, int x, int y) {
	if (mdata->map == NULL)
		return false;

	return gameTools::did_collide(
		world_x * mdata->scale_tile_size,
		world_y * mdata->scale_tile_size,
		get_width(),
		get_height(),
		x * mdata->scale_tile_size,
		y * mdata->scale_tile_size,
		mdata->scale_tile_size,
		mdata->scale_tile_size
	);
}

bool Character::hit_item(CommonItem::ItemData item, int x, int y) {
	Fl_PNG_Image * item_image = CommonItem::get_image(item, mdata);

	if (item_image == NULL || mdata->map == NULL)
		return false;

	return gameTools::did_collide(
		world_x * mdata->scale_tile_size,
		world_y * mdata->scale_tile_size,
		get_width(),
		get_height(),
		x * mdata->scale_tile_size,
		y * mdata->scale_tile_size,
		item_image->w(),
		item_image->h()
	);
}

bool Character::hit_character(Character * character) {
	if (character == NULL)
		return false;

	return gameTools::did_collide(
		world_x * mdata->scale_tile_size,
		world_y * mdata->scale_tile_size,
		get_width(),
		get_height(),
		character->wx() * mdata->scale_tile_size,
		character->wy() * mdata->scale_tile_size,
		character->get_width(),
		character->get_height()
	);
}

std::vector<CharacterHitData> Character::get_hit_data() {
	std::vector<CharacterHitData> hits_data;
	int x, y;
	
	int start_x, start_y, end_x, end_y;
	int char_tile_w, char_tile_h;
	int map_w, map_h;

	int char_width, char_height;

	char_width = get_width();
	char_height = get_height();

	CharacterHitData curr_hit_data;
	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	if (mdata->map == NULL)
		return hits_data;

	// Get item map width and height.
	map_w = mdata->map->get_width();
	map_h = mdata->map->get_height();

	// Get character tile width and height.
	char_tile_w = (int)roundf((float)char_width / mdata->scale_tile_size);
	char_tile_h = (int)roundf((float)char_height / mdata->scale_tile_size);

	// Get start and end locations.
	start_x = wx_rounded() - mdata->settings.map_search_overscan;
	start_y = wy_rounded() - mdata->settings.map_search_overscan;
	end_x = wx_rounded() + char_tile_w + (mdata->settings.map_search_overscan + char_tile_w);
	end_y = wy_rounded() + char_tile_h + (mdata->settings.map_search_overscan + char_tile_h);

	// Check values.
	start_x = (start_x < 0) ? 0 : start_x;
	start_y = (start_y < 0) ? 0 : start_y;
	end_x = (end_x > map_w) ? map_w : end_x;
	end_y = (end_y > map_h) ? map_h : end_y;

	//printf("%d %d | %d %d\n", start_x, end_x, start_y, end_y);

	// Find tiles/items hitting character.
	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++) {
			curr_tile = mdata->map->tile(x, y);
			curr_item = mdata->map->item(x, y);

			// Set type and coord.
			curr_hit_data.type = HIT_NONE;
			curr_hit_data.coord = {(int)x, (int)y};

			// Tile.
			if (hit_tile(curr_tile, x, y) && curr_tile.id != Tile::NONE) {
				curr_hit_data.things_hit.tile = curr_tile;
				curr_hit_data.type |= HIT_TILE;
			}

			// Item.
			if (hit_item(curr_item, x, y) && curr_item.id != CommonItem::NONE) {
				curr_hit_data.things_hit.item = curr_item;
				curr_hit_data.type |= HIT_ITEM;
			}

			// If something hits.
			if (curr_hit_data.type != HIT_NONE)
				hits_data.push_back(curr_hit_data);
		}

	// Npcs.
	for (auto n : mdata->map->get_npc_map()->get_npcs_in_use()) {
		if (n == NULL)
			continue;
		if (n == this)
			continue;

		if (hit_character(n)) {
			curr_hit_data.coord = {n->wx_rounded(), n->wy_rounded()};
			curr_hit_data.things_hit.character = n;
			curr_hit_data.type = HIT_CHARACTER;
			hits_data.push_back(curr_hit_data);
			break;
		}
	}

	// Hits player.
	if (mdata->player == NULL || mdata->player == this)
		return hits_data;

	if (hit_character(mdata->player)) {
		curr_hit_data.coord = {mdata->player->wx_rounded(), mdata->player->wy_rounded()};
		curr_hit_data.things_hit.character = mdata->player;
		curr_hit_data.type = HIT_CHARACTER;
		hits_data.push_back(curr_hit_data);
	}

	return hits_data;
}

void Character::add_item(CommonItem::ItemData item) {
	CommonTool * tool_gained = NULL;
	int tool_id;

	if (item.id == CommonItem::NONE)
		return;

	add_heath(item.heath_gain);
	add_coins(item.coin_gain);

	// Run item command.
	if (item.command_location != CommonItem::NONE)
		mdata->map->run_item_command(item);

	// Gain Tool.
	tool_id = tool_owned(item.gain_tool);

	// Tool already owned.
	if (tool_id != -1) {
		tools[tool_id]->add_fuel(item.gain_fuel);
		return;
	}

	// Gain tool.
	tool_gained = get_tool_from_type(item.gain_tool, mdata, this);

	if (tool_gained == NULL)
		return;

	tool_gained->add_fuel(item.gain_fuel);

	// Set direction.
	if (facing_right())
		tool_gained->set_right();
	else
		tool_gained->set_left();

	tool_gained->move_to_location();
	tools.push_back(tool_gained);
}

int Character::insert_tool(CommonTool * tl, int pos) {
	if (pos >= tools.size() || pos < 0)
		return -1;

	tools[pos] = tl;
	return 0;
}

void Character::set_current_tool_local(int current_tool) {
	this->current_tool = current_tool;
	move_tool_to_location();
}

void Character::move_tool_to_location() {
	CommonTool * curr_tool = get_current_tool();

	if (curr_tool == NULL)
		return;

	if (facing_right())
		curr_tool->set_right();
	else
		curr_tool->set_left();

	curr_tool->move_to_location();
}

CommonTool * Character::get_tool_at(int pos) {
	if (pos >= tools.size() || pos < 0)
		return NULL;

	return tools[pos];
}

CommonTool * Character::get_current_tool() {
	if (current_tool >= tools.size() || current_tool < 0)
		return NULL;

	return tools[current_tool];
}

void Character::next_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool < tools.size() - 1)
		current_tool++;
	else
		current_tool = 0;

	set_current_tool_local(current_tool);
}

void Character::pre_tool() {
	if (tools.size() <= 0)
		return;

	if (current_tool > 0)
		current_tool--;
	else
		current_tool = tools.size() - 1;

	set_current_tool_local(current_tool);
}

void Character::use_tool() {
	CommonTool * tl = get_current_tool();

	if (tl == NULL)
		return;

	tl->use();
}

void Character::stop_using_tool() {
	CommonTool * tl = get_current_tool();

	if (tl == NULL)
		return;

	tl->stop_using();
}

int Character::tool_owned(int tool_type) {
	int i;

	for (i = 0; i < tools.size(); i++)
		if (tools[i]->get_type() == tool_type)
			return i;

	return -1;
}

void Character::handle_collision() {
	wx(old_world_x);
	wy(old_world_y);
}

void Character::handle_collision(float obj_x, float obj_y, int obj_width, int obj_height, float correction_speed) {
	float rise, run;
	float hit_angle;

	float corr_speed;

	float dis_traveled_x, dis_traveled_y;
	float max_dis_traveled;

	// Center coords.
	float obj_cx, obj_cy;
	float cx, cy;

	float obj_w, obj_h;
	float tile_width, tile_height;

	float curr_width, curr_height;

	// Pixal x and y.
	float obj_px, obj_py;

	obj_px = obj_x * mdata->scale_tile_size;
	obj_py = obj_y * mdata->scale_tile_size;

	curr_width = get_width();
	curr_height = get_height();

	// Get width and height.
	obj_w = (float)obj_width / 2 / mdata->scale_tile_size;
	obj_h = (float)obj_height / 2 / mdata->scale_tile_size;

	max_dis_traveled = (obj_w * 2) + (obj_h * 2);

	tile_width = (float)get_width() / 2 / mdata->scale_tile_size;
	tile_height = (float)get_height() / 2 / mdata->scale_tile_size;

	// Get center position
	obj_cx = obj_x + obj_w;
	obj_cy = obj_y + obj_h;

	cx = world_x + tile_width;
	cy = world_y + tile_height;

	// Get rise over run and angle.
	rise = obj_cy - cy;
	run = obj_cx - cx;

	hit_angle = atan2(rise, run);

	if (correction_speed == 0.0)
		corr_speed = get_scaled_speed();
	else
		corr_speed = correction_speed;

	rise = sinf(hit_angle) * corr_speed;
	run = cosf(hit_angle) * corr_speed;

	dis_traveled_x = 0.0;
	dis_traveled_y = 0.0;

	// Reset position.
	while (true) {

		// No longer colliding.
		if (!gameTools::did_collide(
			obj_px,
			obj_py,
			obj_width,
			obj_height,
			world_x * mdata->scale_tile_size,
			world_y * mdata->scale_tile_size,
			curr_width,
			curr_height))
			break;

		// Give up.
		if (dis_traveled_x >= max_dis_traveled || dis_traveled_y >= max_dis_traveled)
			break;

		world_x -= run;
		world_y -= rise;

		dis_traveled_x += abs(run);
		dis_traveled_y += abs(rise);
	}
}

void Character::handle_collision(float obj_x, float obj_y) {
	handle_collision(obj_x, obj_y, mdata->scale_tile_size, mdata->scale_tile_size);
}

void Character::handle_collision(CommonItem::ItemData item, int item_x, int item_y) {
	Fl_PNG_Image * item_image = CommonItem::get_image(item, mdata);

	if (item_image == NULL)
		return;

	handle_collision(
		item_x,
		item_y,
		item_image->w(),
		item_image->h()
	);
}

void Character::handle_collision(Character * character) {
	if (character == NULL)
		return;

	handle_collision(
		character->wx(),
		character->wy(),
		character->get_width(),
		character->get_heath(),
		character->get_scaled_speed()
	);
}

float Character::get_scaled_speed() {
	return (float)speed / mdata->settings.update_fps;
}

void Character::update_world_position() {
	float s = get_scaled_speed();
	update_old_values();

	if (dir.right)
		world_x += s;
	if (dir.left)
		world_x -= s;
	if (dir.up)
		world_y -= s;
	if (dir.down)
		world_y += s;
}

void Character::update_old_values() {
	old_world_x = world_x;
	old_world_y = world_y;
	old_direction = dir;
}

void Character::get_name(char * name_buf) {
	if (name_buf == NULL)
		return;

	memset(name_buf, 0, NAME_MAX);
	strncat(name_buf, character_name, NAME_MAX - 1);
}

void Character::set_name(const char * new_name) {
	if (new_name == NULL)
		return;

	memset(character_name, 0, NAME_MAX);
	strncat(character_name, new_name, NAME_MAX - 1);
}

void Character::say(const char * msg, size_t n) {
	if (msg == NULL || n <= 0)
		return;

	mdata->chat_box->post(character_name, msg, n + NAME_MAX);
}

void Character::ask_question(const char * question) {
	if (question == NULL)
		return;

	CharacterQuestionData question_data;

	// Set question.
	memset(question_data.question, 0, NAME_MAX);
	strncat(question_data.question, question, NAME_MAX - 1);

	// Set asker.
	question_data.asker = this;

	// Send to chatbox.
	mdata->chat_box->set_question_data(question_data);

	this->question_data = question_data;

	// Set other stuff.
	question_state = WAITING_FOR_ANSWER;
	memset(question_answer, 0, NAME_MAX);

	// Say question.
	say(question, NAME_MAX);
}

void Character::answer_question(const char * answer) {
	if (answer == NULL)
		return;

	// Did not ask question.
	if (question_state != WAITING_FOR_ANSWER)
		return;

	question_state = GOT_ANSWER;

	memset(question_answer, 0, NAME_MAX);
	strncat(question_answer, answer, NAME_MAX - 1);
}

void Character::close_question() {
	question_state = NO_QUESTION;
	memset(question_answer, 0, NAME_MAX);
}

bool Character::wait_for_answer(char answers[MAX_QUESTION_ANSWERS][NAME_MAX], size_t n, int * answer_num) {
	int i;
	char buf[NAME_MAX];

	if (answers == NULL)
		return question_state == GOT_ANSWER;

	if (question_state != GOT_ANSWER)
		return false;

	// Check if answer is in answer list.
	for (i = 0; i < n; i++)
		if (strncmp(question_answer, answers[i], NAME_MAX) == 0) {
			if (answer_num != NULL)
				*answer_num = i;

			return true;
		}

	// Answer not one of the options.
	snprintf(buf, NAME_MAX, "'%s' is not a valid anwser. try one of these instead: ", question_answer);
	say(buf, NAME_MAX);

	for (i = 0; i < n; i++) {
		snprintf(buf, NAME_MAX, "--- %s", answers[i]);
		say(buf, NAME_MAX);
	}

	// Reask question.
	ask_question(question_data.question);

	return false;
}

bool Character::wait_for_y_n_answer(bool * answered_yes) {
	int i;

	const char * y_n_responces[NAME_MAX] = {
		"yes",
		"y",
		"no",
		"n"
	};

	size_t y_n_responces_size = 4;

	if (question_state != GOT_ANSWER)
		return false;

	// Check if it is a yes or no question.
	for (i = 0; i < y_n_responces_size; i++)
		if (strncasecmp(y_n_responces[i], question_answer, NAME_MAX) == 0) {
			if (answered_yes == NULL)
				return true;

			if (i <= 1)
				*answered_yes = true;
			else
				*answered_yes = false;

			return true;
		}

	char responce[] = "Its a yes or no question!!!";

	// Reask question.
	ask_question(question_data.question);
	say(responce, sizeof(responce));

	return false;
}

void Character::have_conversation(CharacterConversationData conversation_data) {
	in_conversation = true;
	current_conversation = conversation_data;
	close_question();

	if (question_state == NO_QUESTION &&
			current_conversation.type != MESSAGE) {
		ask_question(current_conversation.message);
	}
}

void Character::update_conversation() {
	bool got_answer = false;
	int answer_num;
	bool answered_yes;
	bool can_call_cb;

	// No conversation.
	if (!in_conversation)
		return;

	// Get answer.
	switch (current_conversation.type) {
		case YES_OR_NO_QUESTION:
			got_answer = wait_for_y_n_answer(&answered_yes);
			answer_num = (int)!answered_yes;
			break;
		case CUSTOM_ANSWERS:
			got_answer = wait_for_answer(
				current_conversation.possible_anwsers,
				current_conversation.answer_count,
				&answer_num
			);

			break;
		case ANY_ANSWER:
			got_answer = wait_for_answer(NULL, 0);
			answer_num = -1;
			break;
		case MESSAGE:
			close_question();
			answer_num = 0;
			got_answer = true;
			break;
		default:
			break;
	}

	if (!got_answer)
		return;

	can_call_cb = current_conversation.callback != NULL;

	if (answer_num == -1) { // Any answer.
		if (can_call_cb)
			answer_num = current_conversation.callback(this, mdata, question_answer);
		else
			answer_num = 0;
	} else { // Call callback.
		if (can_call_cb)
			current_conversation.callback(this, mdata, question_answer);
	}

	// Prevent segfault.
	if (answer_num >= current_conversation.answer_count 
			&& current_conversation.type != YES_OR_NO_QUESTION
			&& current_conversation.type != MESSAGE) {
		fputs("'answer_num' is too big\n", stderr);
		return;
	}

	// End conversation.
	if (current_conversation.next_conversations[answer_num] == NULL) {
		in_conversation = false;
		return;
	}

	// Next question.
	current_conversation = *current_conversation.next_conversations[answer_num];

	if (current_conversation.type == MESSAGE)
		say(current_conversation.message, NAME_MAX);
	else
		ask_question(current_conversation.message);
}
