#include "game_tools.h"
#include "weapon.h"
#include "chat_box.h"
#include "map.h"
#include "image_loader.h"

namespace gameTools {
	std::vector<Fl_PNG_Image*> scale_images(std::vector<Fl_PNG_Image*> imgs, int s) {
		std::vector<Fl_PNG_Image*> scaled_images;

		for (auto i : imgs)
			scaled_images.push_back(scale_image(i, s));

		return scaled_images;
	}

	Fl_PNG_Image * scale_image(Fl_PNG_Image * img, int s) {
		return (Fl_PNG_Image*)img->copy(img->w() * s, img->h() * s);
	}

	void delete_image_list(std::vector<Fl_PNG_Image*> * imgs) {
		long int i;

		for (i = 0; i < imgs->size(); i++) {
			if (imgs->at(i) == NULL)
				continue;

			delete imgs->at(i);
			imgs->at(i) = NULL;
		}
	}

	std::vector<Fl_PNG_Image*> copy_image_list(std::vector<Fl_PNG_Image*> imgs) {
		std::vector<Fl_PNG_Image*> copy_of_imgs;

		for (auto i : imgs)
			copy_of_imgs.push_back((Fl_PNG_Image*)i->copy());

		return copy_of_imgs;

	}

	void scale_all(MainData * mdata) {
		mdata->scaled_images.player = scale_images(mdata->images.player, mdata->settings.scale);
		mdata->scaled_images.weapons.sword = scale_images(mdata->images.weapons.sword, mdata->settings.scale);
		mdata->scaled_images.basic_world = scale_images(mdata->images.basic_world, mdata->settings.scale);

		mdata->scale_tile_size = mdata->settings.scale * TILE_SIZE;
	}

	bool test_collision(Fl_Widget * obj1, Fl_Widget * obj2) {
		if (obj1->x() + obj1->w() >= obj2->x() 
				&& obj1->x() <= obj2->x() + obj2->w()
				&& obj1->y() + obj1->h() >= obj2->y() 
				&& obj1->y() <= obj2->y() + obj2->h())
			return true;
		return false;
	}

	size_t get_file_size(FILE * fp) {
		size_t old_file_ftell = ftell(fp);
		size_t file_size;

		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);

		// Return to old position.
		fseek(fp, old_file_ftell, SEEK_SET);

		return file_size;
	}

	int valuestr_to_int(const Fl_Input * input) {
		int res = 0; // Return value.

		if (input == NULL)
			return -1;

		// Get int from string
		if (str_to_int(input->value(), input->size(), &res) == -1)
			return -1;

		return res;
	}

	long int get_char_count(const char * str_buf, char search_char, size_t n) {
		long int i;
		long int count = 0;

		// Check for errors.
		if (str_buf == NULL || n <= 0)
			return -1;

		// Get search_char count.
		for (i = 0; i < n; i++) {
			if (str_buf[i] == '\0')
				break;
			else if (str_buf[i] == search_char)
				count++;
		}

		return count;
	}

	int get_file_full_path(const char * file_name, char * full_path) {
		if (file_name == NULL || full_path == NULL)
			return -1;

#ifdef _WIN32
		if (GetFullPathNameA(file_name, NAME_MAX, full_path, NULL) <= 0)
			return -1;
#else
		if (realpath(file_name, full_path) == NULL)
			return -1;
#endif

		return 0;
	}

	int get_file_location(const char * file_path, char * file_locat) {
		char real_path[NAME_MAX];

		int i;
		size_t real_path_size;

		// Check for errors.
		if (file_path == NULL || file_locat == NULL)
			return -1;

		// Fill memory buffers with null bytes.
		memset(file_locat, 0, NAME_MAX);
		memset(real_path, 0, NAME_MAX);

		// Get real file path for file_path and check for errors.
		if (get_file_full_path(file_path, real_path) == -1)
			return -1;

		// Get real path size.
		real_path_size = strnlen(real_path, NAME_MAX);

		// Remove file name from path.
		for (i = real_path_size; i > 0; i--) {
			if (real_path[i] == PSLASH[0]) // End of file name.
				break;

			real_path[i] = '\0';
		}

		strncat(file_locat, real_path, NAME_MAX); // Set output.
		return 0;
	}

	int get_file_name(const char * file_path, char * file_name) {
		int i, j;
		size_t file_path_size;

		size_t file_name_buf_size;
		char file_name_buf[NAME_MAX];

		if (file_path == NULL || file_name == NULL)
			return -1;

		memset(file_name_buf, 0, NAME_MAX);

		// Get size of 'file_path'.
		file_path_size = strnlen(file_path, NAME_MAX);

		j = 0;

		// Get file name from 'file_path'.
		for (i = file_path_size - 1; i >= 0; i--) {
			if (file_path[i] == PSLASH[0]) // End of 'file_path'.
				break;

			file_name_buf[j] = file_path[i];
			j++;
		}

		file_name_buf_size = j;
		j = 0;

		// Invert 'file_name_buf' and add to 'file_name'.
		for (i = file_name_buf_size - 1; i >= 0; i--) {
			file_name[j] = file_name_buf[i];
			j++;
		}

		return 0;
	}

	int str_to_int(const char * str, size_t n, int * int_value, bool force_hex) {
		int i;
		int cvalue;
		int value_muliplier = 1;
		int mulpile_by = 10; // 10 for decimal and 16 for hex.
		int res_value = 0;
		int neg = 1; // -1 for negative and 1 for whole.
		size_t str_len; // String length.
		int end_at = 0; // Where loop should end.
		
		bool hex_mode = false;

		if (str == NULL || int_value == NULL || n <= 0)
			return -1;

		// Get string length
		str_len = strnlen(str, n);

		if (str_len <= 0)
			return -1;

		// Is negative.
		if (str[0] == '-') {
			neg = -1;
			end_at = 1; // If negative 0 item in 'str' is skipped.
		}

		// Hex mode.
		if ((str[0] == '0' && str[1] == 'x') || force_hex) {
			hex_mode = true;
			neg = 1;
			end_at = 0;
			mulpile_by = 16;
		}

		// Do the math.
		for (i = str_len - 1; i >= end_at; i--) {
			cvalue = hex_mode ? hex_char_to_int(str[i]) : char_to_int(str[i]);

			// 0x in hex string.
			if (hex_mode && str[i] == 'x')
				break;

			// Character not a number.
			if (cvalue == -1)
				return -1;

			// Do the same math that is down below.
			res_value += cvalue * value_muliplier;
			value_muliplier *= mulpile_by;
		}

		/*
		 * "436"
		 * res_value = (6 * 1) + (3 * 10) + (4 * 100)
		*/

		*int_value = (res_value * neg);
		return 0;
	}

	int char_to_int(char c) {
		int cvalue = (int)c;

		// Not a number.
		// 48 to 57 is 0 to 9 in ascii.
		if (cvalue < 48 || cvalue > 57)
			return -1;

		return cvalue - 48; // 48 is the value of zero in ascii.
	}

	int hex_char_to_int(char c) {
		int cvalue;
		cvalue = char_to_int(c); // Between 0 and 9

		if (cvalue != -1)
			return cvalue;

		cvalue = tolower(c);

		// Not a hex number.
		// 97 to 102 is 'a' to 'f' in ascii.
		if (cvalue < 97 || cvalue > 102)
			return -1;

		return cvalue - 87; // Weird magic number made for your pain.
	}

	std::vector<std::string> get_paths_with_label(const char * image_loader_path, const char * label) {
		std::vector<std::string> path_list;
		imageLoader::CommandData com;

		if (image_loader_path == NULL || label == NULL)
			return path_list;

		// Set command.
		imageLoader::clear_command(&com);

		com.command = imageLoader::START_COM;
		strncat(com.args[0], label, NAME_MAX - 1);
		com.arg_count = 1;

		// Get image paths.
		path_list = imageLoader::get_paths(image_loader_path, com);
		return path_list;
	}

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

		path_list = get_paths_with_label(file_path, "player");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() <= 0)
			return 0;

		set_image_list(&mdata->images.player, imageLoader::load_images(path_list));
		set_image_list(&mdata->scaled_images.player, scale_images(mdata->images.player, mdata->settings.scale));
		return 0;
	}

	int load_weapon_images(MainData * mdata, const char * file_path, const char * img_location) {
		std::vector<std::string> path_list;

		if (file_path == NULL || img_location == NULL)
			return -1;

		// Sword.
		path_list = get_paths_with_label(file_path, "sword");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.weapons.sword, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.weapons.sword, scale_images(mdata->images.weapons.sword, mdata->settings.scale));
		}

		// Submachine gun.
		path_list = get_paths_with_label(file_path, "submachine_gun");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.weapons.submachine_gun, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.weapons.submachine_gun, scale_images(mdata->images.weapons.submachine_gun, mdata->settings.scale));
		}

		return 0;
	}

	int load_world_images(MainData * mdata, const char * file_path, const char * img_location) {
		std::vector<std::string> path_list;

		if (file_path == NULL || img_location == NULL)
			return -1;

		// Basic world.
		path_list = get_paths_with_label(file_path, "basic_world");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.basic_world, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.basic_world, scale_images(mdata->images.basic_world, mdata->settings.scale));
		}

		// Current world.
		path_list = get_paths_with_label(file_path, "current_world");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.curr_world, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.curr_world, scale_images(mdata->images.curr_world, mdata->settings.scale));
		}

		// Addon images.
		path_list = get_paths_with_label(file_path, "addon_images");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.addon_images, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.addon_images, scale_images(mdata->images.addon_images, mdata->settings.scale));
		}

		return 0;
	}

	int load_npc_images(MainData * mdata, const char * file_path, const char * img_location) {
		std::vector<std::string> path_list;

		if (file_path == NULL || img_location == NULL)
			return -1;

		// Evil puppy.
		path_list = get_paths_with_label(file_path, "evil_puppy");
		path_list = imageLoader::add_path(path_list, img_location);

		if (path_list.size() > 0) {
			set_image_list(&mdata->images.npc.evil_puppy, imageLoader::load_images(path_list));
			set_image_list(&mdata->scaled_images.npc.evil_puppy, scale_images(mdata->images.npc.evil_puppy, mdata->settings.scale));
		}

		return 0;
	}

	int get_file_ext(const char * file_name, char * file_ext, size_t file_ext_size) {
		long int i, i2;
		long int file_ext_i = 0;
		size_t file_name_size;
		size_t ext_char_count = 0;

		char file_ext_cp[file_ext_size];

		bool dot_found = false;

		if (file_name == NULL || file_ext == NULL || file_ext_size <= 0)
			return -1;

		memset(file_ext, 0, file_ext_size);
		memset(file_ext_cp, 0, file_ext_size);

		// Get file name size.
		file_name_size = strnlen(file_name, NAME_MAX);

		if (file_name_size <= 0)
			return -1;

		// Get file ext.
		for (i = file_name_size; i >= 0; i--) {
			if (file_name[i] == PSLASH[0])
				break;
			if (file_ext_i >= file_ext_size)
				return -1;
			if (file_name[i] == '\0')
				continue;
			if (file_name[i] == '.') {
				dot_found = true;
				break;
			}

			file_ext[file_ext_i] = file_name[i];
			file_ext_i++;
		}

		// Reverse file_ext;
		strncat(file_ext_cp, file_ext, file_ext_size);
		ext_char_count = strnlen(file_ext, file_ext_size);

		if (ext_char_count <= 0)
			return -1;

		i2 = ext_char_count - 1;

		for (i = 0; i < ext_char_count; i++) {
			if (file_ext[i] == '\0')
				continue;

			file_ext[i] = file_ext_cp[i2];
			i2--;
		}

		if (dot_found)
			return 0;
		return -1;
	}

	GAME_DATA_TYPE file_ext_to_datatype(const char * file_ext) {
		if (strncmp(file_ext, "mp", NAME_MAX) == 0) // Map.
			return GAME_MAP;
		else if (strncmp(file_ext, "cms", NAME_MAX) == 0) // command script.
			return GAME_COMMAND_SCRIPT;
		else if (strncmp(file_ext, "il", NAME_MAX) == 0) // Image loader.
			return GAME_IMAGE_LOADER;

		return GAME_NONE;
	}

	int load_file(MainData * mdata, const char * file_path, const char * image_folder) {
		GAME_DATA_TYPE data_type;
		char file_ext[NAME_MAX];

		if (file_path == NULL)
			return -1;

		// Get file ext.
		if (get_file_ext(file_path, file_ext, NAME_MAX) == -1)
			return -1;

		// Get data type.
		data_type = file_ext_to_datatype(file_ext);

		// Handle file.
		switch (data_type) {
			case GAME_NONE:
				return -1;
			case GAME_MAP:
				return mdata->map->load(file_path, NAME_MAX);
			case GAME_COMMAND_SCRIPT:
				return mdata->chat_box->run_script(file_path);
			case GAME_IMAGE_LOADER:
				return load_images_from_loader_file(mdata, file_path, image_folder);
			default:
				return -1;
		}
	}

	bool same_image(const Fl_PNG_Image * img1, const Fl_PNG_Image * img2) {
		long int i;
		size_t img1_size;
		size_t img2_size;

		// Memory address are the same.
		if (img1 == img2)
			return true;

		// null.
		if (img1 == NULL || img2 == NULL)
			return false;

		// Get size.
		img1_size = img1->w() * img1->h() * img1->d();
		img2_size = img2->w() * img2->h() * img2->d();

		// Sizes not the same.
		if (img1_size != img2_size)
			return false;

		if (img1_size <= 0 || img1->array == NULL || img2->array == NULL)
			return false;

		// Compare bytes.
		for (i = 0; i < img1_size; i++)
			if (img1->array[i] != img2->array[i])
				return false;

		return true;
	}

	bool no_failed_images(std::vector<Fl_PNG_Image*> image_list) {
		for (auto i : image_list) {
			if (i == NULL)
				return false;
			else if (i->fail())
				return false;
		}

		return true;
	}

	int set_image_list(std::vector<Fl_PNG_Image*> * image_list1, std::vector<Fl_PNG_Image*> image_list2) {
		long int i;

		// Different sizes.
		if (image_list1->size() != image_list2.size() && no_failed_images(image_list2)) {
			delete_image_list(image_list1);
			*image_list1 = image_list2;
			return 0;
		}

		for (i = 0; i < image_list1->size(); i++) {
			if (same_image(image_list1->at(i), image_list2[i]) || image_list2[i]->fail()) {
				delete image_list2[i];
			} else {
				delete image_list1->at(i);
				image_list1->at(i) = image_list2[i];
			}
		}

		return 0;
	}

	void set_scrollbar_bounds(MainData * mdata, int width, int height) {
		// Set scrollbar range.
		if (mdata->map_vertical_scrollbar != NULL) { // Vertical.
			mdata->map_vertical_scrollbar->bounds(-height, height);
			mdata->map_vertical_scrollbar->value(0);
			mdata->map_vertical_scrollbar_output->value("0");
		} if (mdata->map_horizontal_scrollbar != NULL) { // Horizontal.
			mdata->map_horizontal_scrollbar->bounds(-width, width);
			mdata->map_horizontal_scrollbar->value(0);
			mdata->map_horizontal_scrollbar_output->value("0");
		}
	}

	bool did_collide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
		return x1 + w1 >= x2
			&& x1 <= x2 + w2
			&& y1 + h1 >= y2
			&& y1 <= y2 + h2;
	}

	int str_array_to_str(const char ** str_array, int width, int height, const char * split_by, char * str_output, size_t str_output_size, int start_at) {
		int i;
		size_t split_by_size;
		size_t output_buf_size;
		char * output_buf = NULL;

		if (str_array == NULL || str_output == NULL)
			return -1;
		if (width <= 0 || height <= 0 || str_output_size <= 0)
			return -1;

		if (split_by == NULL)
			split_by_size = 0;
		else
			split_by_size = strnlen(split_by, NAME_MAX);

		// Create 'output_buf'.
		output_buf_size = width * height;
		output_buf = new char[output_buf_size];

		if (output_buf == NULL) {
			fputs("Cant get memory for 'output_buf'\n", stderr);
			return -1;
		}

		// Clear output buffer.
		memset(output_buf, 0, output_buf_size);

		// Put str_array in output_buf.
		for (i = start_at; i < height; i++) {
			strncat(output_buf, str_array[i], width);

			if (i != height - 1 && split_by_size > 0) // Add end str if not end.
				strncat(output_buf, split_by, split_by_size); // 2 for space and zero byte at end.
		}

		memset(str_output, 0, str_output_size);
		strncat(str_output, output_buf, str_output_size);

		delete [] output_buf;
		return 0;
	}
}
