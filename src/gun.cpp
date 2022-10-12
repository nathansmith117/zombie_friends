#include "gun.h"
#include "character.h"
#include "player.h"
#include "npc_map.h"

void Gun::draw() {
	int draw_w, draw_h;

	draw_w = bullet_width();
	draw_h = bullet_height();

	// Draw gun image
	Fl_PNG_Image * current_img;
	current_img = get_current_image();
	
	if (current_img == NULL)
		return;

	current_img->draw(x(), y());

	// Draw bullets.
	if (bullets.size() <= 0)
		return;

	for (auto b : bullets)
		if (!b.removed)
			fl_rectf(b.x, b.y, draw_w, draw_h, bullet_color);
}

void Gun::update_bullets() {
	int i;
	Bullet curr_bullet;

	// Bullet add speed.
	// Make sure there are no returns before this.
	if ((float)updates_after_bullet_added / mdata->settings.update_fps * bullets_per_second >= 1.0)
		can_add_bullet = true;
	else {
		can_add_bullet = false;
		updates_after_bullet_added++;
	}

	// Clear hit data.
	if (bullet_hit_data.size() > 0)
		bullet_hit_data.clear();

	// No bullets.
	if (bullets.size() <= 0)
		return;

	for (i = 0; i < bullets.size(); i++)
		update_and_test_bullet(i);

	// Remove last bullet if out of view window.
	curr_bullet = bullets.back();

	if (bullet_out_of_view(curr_bullet) || curr_bullet.removed)
		bullets.pop_back();
}

void Gun::update_and_test_bullet(int bullet_id) {
	BulletHitData new_hit_data;
	Bullet curr_bullet;

	int b_width, b_height;

	b_width = bullet_width();
	b_height = bullet_height();

	Player * player = (Player*)mdata->player;

	bool obj_collided;

	Fl_PNG_Image * item_image;
	Tile::TileObject curr_tile;
	CommonItem::ItemData curr_item;

	NpcMap * npc_map = mdata->map->get_npc_map();

	int tile_x, tile_y; // What tile coord the bullet is on.
	
	// Map offset.
	int offset_x, offset_y;
	offset_x = mdata->map->offset_x();
	offset_y = mdata->map->offset_y();

	// Get bullet.
	if (bullet_id >= bullets.size() || bullet_id < 0)
		return;

	curr_bullet = bullets[bullet_id];

	if (curr_bullet.removed)
		return;

	// Update position.
	curr_bullet.x += bullet_speed * mdata->settings.scale / mdata->settings.update_fps * curr_bullet.dir;

	// Keep bullet speed looking the same with map scrolling.
	if (player != NULL) {
		// Walking away from bullet.
		if ((player->facing_right() && curr_bullet.dir == BULLET_LEFT)
				|| (player->facing_left() && curr_bullet.dir == BULLET_RIGHT))
			curr_bullet.x += abs(offset_x - curr_bullet.last_map_x_offset) * curr_bullet.dir;
		// Walking to the bullet.
		else
			curr_bullet.x -= abs(offset_x - curr_bullet.last_map_x_offset) * curr_bullet.dir;
	}

	// Keep bullet y coord on the same position on map.
	curr_bullet.y = curr_bullet.start_y + (offset_y - curr_bullet.start_map_y_offset);

	curr_bullet.last_map_x_offset = offset_x;
	curr_bullet.last_map_y_offset = offset_y;

	// Update 'bullets'.
	bullets[bullet_id] = curr_bullet;

	// Reset 'new_hit_data'.
	new_hit_data.type = HIT_NONE;

	// Bullet tile coord.
	tile_x = ((curr_bullet.x - offset_x) / mdata->scale_tile_size);
	tile_y = ((curr_bullet.y - offset_y) / mdata->scale_tile_size);

	curr_tile = mdata->map->tile(tile_x, tile_y);

	// Add tile to hit data if not none type and on map.
	if (curr_tile.type != Tile::NONE 
			&& tile_x < mdata->map->get_width()
			&& tile_y < mdata->map->get_height()
			&& tile_x >= 0
			&& tile_y >= 0) {

		new_hit_data.things_hit.tile = curr_tile;
		new_hit_data.type |= HIT_TILE;
	}

	// Hits item.
	curr_item = mdata->map->item(tile_x, tile_y);
	item_image = CommonItem::get_image(curr_item, mdata);

	obj_collided = false;

	// Test for collion.
	if (item_image != NULL)
		obj_collided = gameTools::did_collide(
			curr_bullet.x,
			curr_bullet.y,
			b_width,
			b_height,
			(tile_x * mdata->scale_tile_size) + offset_x,
			(tile_y * mdata->scale_tile_size) + offset_y,
			item_image->w(),
			item_image->h()
		);

	// Hits item.
	if (obj_collided && curr_item.id != CommonItem::NONE
			&& tile_x < mdata->map->get_width()
			&& tile_y < mdata->map->get_height()
			&& tile_x >= 0
			&& tile_y >= 0) {

		new_hit_data.things_hit.item = curr_item;
		new_hit_data.type |= HIT_ITEM;
	}

	// Hits npc.
	for (auto n : npc_map->get_npcs_in_use()) {
		if (n == NULL)
			continue;
		if (n == item_holder)
			continue;

		obj_collided = gameTools::did_collide(
			curr_bullet.x,
			curr_bullet.y,
			b_width,
			b_height,
			n->x(),
			n->y(),
			n->get_width(),
			n->get_height()
		);

		if (obj_collided) {
			new_hit_data.things_hit.character = n;
			new_hit_data.type |= HIT_CHARACTER;
			break;
		}
	}

	obj_collided = false;

	// Test for collion.
	obj_collided = gameTools::did_collide(
		curr_bullet.x,
		curr_bullet.y,
		b_width,
		b_height,
		player->x(),
		player->y(),
		player->get_width(),
		player->get_height()
	);

	// Hits player.
	if (obj_collided && item_holder != player) {
		new_hit_data.things_hit.character = player;
		new_hit_data.type |= HIT_CHARACTER;
	}

	// Add bullet hit data.
	if (new_hit_data.type != HIT_NONE) {
		new_hit_data.bullet_id = bullet_id;
		new_hit_data.bullet_count_at_time = bullets.size();
		new_hit_data.bullet = curr_bullet;

		new_hit_data.coord.x = tile_x;
		new_hit_data.coord.y = tile_y;

		bullet_hit_data.push_back(new_hit_data);
	}
}

int Gun::add_bullet() {
	Bullet new_bullet;
	int x_offset;

	if (!can_add_bullet)
		return 0;

	// Out of Ammo.
	if (needs_fuel && fuel <= 0)
		return 0;

	can_add_bullet = false;
	updates_after_bullet_added = 0;

	// No item holder.
	if (item_holder == NULL)
		return -1;
	// At max bullet count.
	if (bullets.size() >= max_bullets && max_bullets != BULLET_MAX_NONE)
		return 0;

	// Set direction.
	if (((Character*)item_holder)->facing_right())
		new_bullet.dir = BULLET_RIGHT;
	else
		new_bullet.dir = BULLET_LEFT;

	if (frame >= bullet_start_locations.size())
		return -1;

	// Set position.
	new_bullet.x = x() + (bullet_start_locations[frame].x * mdata->settings.scale);
	new_bullet.y = y() + (bullet_start_locations[frame].y * mdata->settings.scale);

	new_bullet.start_x = new_bullet.x;
	new_bullet.start_y = new_bullet.y;

	// Map offset.
	new_bullet.start_map_x_offset = mdata->map->offset_x();
	new_bullet.start_map_y_offset = mdata->map->offset_y();
	new_bullet.last_map_x_offset = new_bullet.start_map_x_offset;
	new_bullet.last_map_y_offset = new_bullet.start_map_y_offset;

	new_bullet.removed = false;

	if (needs_fuel)
		fuel--;

	bullets.push_back(new_bullet);
	return 0;
}

int Gun::add_bullets(int num) {
	int i;

	for (i = 0; i < num; i++)
		if (add_bullet() == -1)
			return -1;

	return 0;
}

int Gun::remove_bullet(int num) {
	if (num >= bullets.size() || num < 0)
		return -1;

	bullets[num].removed = true;
	return 0;
}

bool Gun::bullet_out_of_view(Bullet b) {
	Fl_Double_Window * view_win = mdata->view_win;

	if (view_win == NULL)
		return true;

	bool x_out = (b.x < view_win->x()) | (b.x + bullet_w > view_win->x() + view_win->w());
	bool y_out = (b.y < view_win->y()) | (b.y + bullet_h > view_win->y() + view_win->h());

	return x_out | y_out;
}
