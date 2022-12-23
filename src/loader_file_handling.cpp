#include "loader_file_handling.h"
#include "character.h"
#include "npc.h"

int load_and_store_image_data(
	MainData * mdata, 
	const char * file_path,
	const char * img_location,
	ImageListLoadingData img_list_data
) {
	std::vector<std::string> path_list;

	// Check everything.
	if (file_path == NULL)
		return -1;
	else if (img_location == NULL)
		return -1;
	else if (img_list_data.img_list_locat == NULL)
		return -1;
	else if (img_list_data.scaled_img_list_locat == NULL)
		return -1;

	// Get path list.
	path_list = gameTools::get_paths_with_label(file_path, img_list_data.label);
	path_list = imageLoader::add_path(path_list, img_location);

	if (path_list.size() <= 0)
		return 0;

	// Load.
	gameTools::set_image_list(
		img_list_data.img_list_locat,
		imageLoader::load_images(path_list)
	);

	// Scale.
	gameTools::set_image_list(
		img_list_data.scaled_img_list_locat,
		gameTools::scale_images(*img_list_data.img_list_locat, mdata->settings.scale)
	);

	return 0;
}

int load_images_from_loader_file(MainData * mdata, const char * file_path, const char * image_folder) {
	int i;

	// This is where the data for loading images is (-------:
	const ImageListLoadingData image_list_load_data[] = {
		// Player.
		ImageListLoadingData("player", &mdata->images.player, &mdata->scaled_images.player),

		// Weapons.
		ImageListLoadingData("sword", &mdata->images.weapons.sword, &mdata->scaled_images.weapons.sword),
		ImageListLoadingData("submachine_gun", &mdata->images.weapons.submachine_gun, &mdata->scaled_images.weapons.submachine_gun),

		// World.
		ImageListLoadingData("basic_world", &mdata->images.basic_world, &mdata->scaled_images.basic_world),
		ImageListLoadingData("current_world", &mdata->images.curr_world, &mdata->scaled_images.curr_world),
		ImageListLoadingData("addon_images", &mdata->images.addon_images, &mdata->scaled_images.addon_images),

		// Npc.
		ImageListLoadingData("evil_puppy", &mdata->images.npc.evil_puppy, &mdata->scaled_images.npc.evil_puppy)
	};

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

	// Load and store all images.
	for (auto i : image_list_load_data)
		load_and_store_image_data(
			mdata,
			file_path,
			imgs_location,
			i
		);

	// Refresh player images.
	if (mdata->player != NULL)
		mdata->player->refresh_images();

	return 0;
}
