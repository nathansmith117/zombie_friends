#include "loader_file_handling.h"

int load_images_from_loader_file(MainData * mdata, const char * file_path, const char * image_folder) {
	int i;

	std::vector<std::string> path_list;

	size_t imgs_location_size = NAME_MAX + MAIN_DIR_SIZE;
	char imgs_location[imgs_location_size];
	memset(imgs_location, 0, NAME_MAX);

	if (file_path == NULL)
		return -1;

	// Set imgs_location == MAIN_DIR/images/ if null.
	if (image_folder == NULL)
		snprintf(imgs_location, imgs_location_size, "%simages%s", mdata->MAIN_DIR, PSLASH);
	else
		strncat(imgs_location, image_folder, NAME_MAX);

	// Player.
	load_player_images(mdata, file_path, imgs_location);

	// Weapons.
	load_weapon_images(mdata, file_path, imgs_location);

	// World.
	load_world_images(mdata, file_path, imgs_location);

	// Npc.
	load_npc_images(mdata, file_path, imgs_location);

	return 0;
}

int load_player_images(MainData * mdata, const char * file_path, const char * img_location) {
	std::vector<std::string> path_list;

	if (file_path == NULL || img_location == NULL)
		return -1;

	path_list = gameTools::get_paths_with_label(file_path, "player");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() <= 0)
		return 0;

	gameTools::set_image_list(&mdata->images.player, imageLoader::load_images(path_list));
	gameTools::set_image_list(&mdata->scaled_images.player, gameTools::scale_images(mdata->images.player, mdata->settings.scale));
	return 0;
}

int load_weapon_images(MainData * mdata, const char * file_path, const char * img_location) {
	std::vector<std::string> path_list;

	if (file_path == NULL || img_location == NULL)
		return -1;

	// Sword.
	path_list = gameTools::get_paths_with_label(file_path, "sword");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.weapons.sword, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.weapons.sword, gameTools::scale_images(mdata->images.weapons.sword, mdata->settings.scale));
	}

	// Submachine gun.
	path_list = gameTools::get_paths_with_label(file_path, "submachine_gun");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.weapons.submachine_gun, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.weapons.submachine_gun, gameTools::scale_images(mdata->images.weapons.submachine_gun, mdata->settings.scale));
	}

	return 0;
}

int load_world_images(MainData * mdata, const char * file_path, const char * img_location) {
	std::vector<std::string> path_list;

	if (file_path == NULL || img_location == NULL)
		return -1;

	// Basic world.
	path_list = gameTools::get_paths_with_label(file_path, "basic_world");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.basic_world, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.basic_world, gameTools::scale_images(mdata->images.basic_world, mdata->settings.scale));
	}

	// Current world.
	path_list = gameTools::get_paths_with_label(file_path, "current_world");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.curr_world, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.curr_world, gameTools::scale_images(mdata->images.curr_world, mdata->settings.scale));
	}

	// Addon images.
	path_list = gameTools::get_paths_with_label(file_path, "addon_images");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.addon_images, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.addon_images, gameTools::scale_images(mdata->images.addon_images, mdata->settings.scale));
	}

	return 0;
}

int load_npc_images(MainData * mdata, const char * file_path, const char * img_location) {
	std::vector<std::string> path_list;

	if (file_path == NULL || img_location == NULL)
		return -1;

	// Evil puppy.
	path_list = gameTools::get_paths_with_label(file_path, "evil_puppy");
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() > 0) {
		gameTools::set_image_list(&mdata->images.npc.evil_puppy, imageLoader::load_images(path_list));
		gameTools::set_image_list(&mdata->scaled_images.npc.evil_puppy, gameTools::scale_images(mdata->images.npc.evil_puppy, mdata->settings.scale));
	}

	return 0;
}
