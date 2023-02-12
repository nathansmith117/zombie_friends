#include "submachine_gun.h"
#include "character.h"
#include "map.h"
#include "npc_map.h"
#include "id_to_object.h"

void SubmachineGun::main_init(MainData * md, Fl_Widget * item_holder) {
	mdata = md;

	type = TOOL_SUBMACHINE_GUN;

	refresh_images();

	// Bullet settings.
	bullet_w = 2;
	bullet_h = 1;
	bullet_color = FL_YELLOW;
	bullet_speed = 450;
	bullets_per_second = 4;
	max_bullets = BULLET_MAX_NONE;
	needs_fuel = true;
	fuel = 30;

	// Other settings.
	submachine_gun_speed = 0.0001;

	bullet_start_locations = {
		{16, 2},
		{0, 2}
	};

	// Location data.
	item_location({
		{0, 12, 14, 19},
		{16, 12, 0, 19}
	});

	move_to_location();
}

void SubmachineGun::update() {
	int hit_x, hit_y;
	int i;
	Bullet new_bullet;

	last_count++;

	// Slow stuff down.
	if (last_count < (int)roundf(mdata->settings.update_fps * submachine_gun_speed))
		return;
	else
		last_count = 0;

	// Fire bullet.
	if (is_being_used)
		add_bullet();

	update_bullets();

	// Handle bullet hit.
	if (bullet_hit_data.size() <= 0)
		return;

	for (i = 0; i < bullet_hit_data.size(); i++) {
		hit_x = bullet_hit_data[i].coord.x;
		hit_y = bullet_hit_data[i].coord.y;

		// All ready handled.
		if (bullet_hit_data[i].hit_handled)
			continue;

		// Bullet all ready removed from map.
		if (bullet_hit_data[i].bullet_id >= bullets.size())
			continue;

		// Tile.
		if ((bullet_hit_data[i].type & HIT_TILE) == HIT_TILE) {

			// Hits wall or something like that.
			if ((bullet_hit_data[i].things_hit.tile.type & Tile::TYPE_NO_WALKTHROUGH) == Tile::TYPE_NO_WALKTHROUGH) {
				bullet_hit_data[i].hit_handled = true;
				remove_bullet(bullet_hit_data[i].bullet_id);
			}
		}

		// Item.
		if ((bullet_hit_data[i].type & HIT_ITEM) == HIT_ITEM) {
			((Character*)item_holder)->add_item(bullet_hit_data[i].things_hit.item);
 			mdata->map->remove_item(hit_x, hit_y);
			bullet_hit_data[i].hit_handled = true;
			remove_bullet(bullet_hit_data[i].bullet_id);
		}

		// Character/npc.
		if ((bullet_hit_data[i].type & HIT_CHARACTER) == HIT_CHARACTER) {
			bullet_hit_data[i].hit_handled = true;
			remove_bullet(bullet_hit_data[i].bullet_id);
			mdata->map->get_npc_map()->delete_npc_in_use((Npc*)bullet_hit_data[i].things_hit.character);
		}
	}
}

void SubmachineGun::refresh_images() {
	images(gameTools::copy_image_list(mdata->scaled_images.weapons.submachine_gun));
	image(mdata->images.weapons.submachine_gun[0]->copy());
}
