#pragma once

#include "program_data.h"
#include "tile.h"

namespace gameTools {
	struct Coord {
		int x, y;
	};

	struct Direction {
		bool right, left, up, down;

		bool operator==(Direction dir) {
			return right == dir.right && left == dir.left && up == dir.up && down == dir.down;
		}

		bool operator!=(Direction dir) {
			return right != dir.right || left != dir.left || up != dir.up || down != dir.down;
		}
	};

	enum IS_NEG {
		NEGATIVE = -1,
		POSITIVE = 1
	};

	std::vector<Fl_PNG_Image*> scale_images(std::vector<Fl_PNG_Image*> imgs, int s);
	Fl_PNG_Image * scale_image(Fl_PNG_Image * img, int s);

	void delete_image_list(std::vector<Fl_PNG_Image*> * imgs);
	std::vector<Fl_PNG_Image*> copy_image_list(std::vector<Fl_PNG_Image*> imgs);

	void scale_all(MainData * mdata);

	bool test_collision(Fl_Widget * obj1, Fl_Widget * obj2);

	// -1 if negative and 1 if not.
	int is_neg(int value);

	template <typename T>
	inline T make_non_neg_angle(T angle) {
		return (angle >= 0) ? angle : (360 + angle);
	}

	template <typename T>
	inline T rad_to_deg(T num) {
		return num * 180 / M_PI;
	}

	template <typename T>
	inline T deg_to_rad(T num) {
		return num * M_PI / 180;
	}

	template <typename T>
	inline T angle_dis(T a1, T a2) {
		T a, b;
		a = make_non_neg_angle(a1);
		b = make_non_neg_angle(a2);

		return 180 - abs(abs(a - b) - 180);
	}

	template <typename T>
	inline bool is_in_range(T num, T value1, T value2) {
		return num >= value1 && num <= value2;
	}

	// Bit stuff.
	template <typename T>
	inline T set_bit(T bit_data, T bit_to_set) {
		return (bit_data | (0x1 << bit_to_set));
	}

	template <typename T>
	inline T is_bit_set(T bit_data, T bitn) {
		return ((bit_data >> bitn) & 0x1);
	}

	template <typename T>
	inline T ** create_2d_array(size_t width, size_t height) {
		long int i;
		T ** ar;

		// Check stuff.
		if (width <= 0 || height <= 0)
			return NULL;

		// Create memory.
		ar = new T*[height];

		if (ar == NULL)
			return NULL;

		for (i = 0; i < height; i++) {
			ar[i] = new T[width];

			if (ar[i] == NULL) {
				delete [] ar;
				return NULL;
			}
		}

		return ar;
	}

	template <typename T>
	inline int delete_2d_array(T ** ar, size_t width, size_t height) {
		long int i;

		// Check stuff.
		if (width <= 0 || height <= 0)
			return -1;
		if (ar == NULL)
			return -1;

		for (i = 0; i < height; i++)
			delete [] ar[i];

		delete [] ar;
		ar = NULL;
		return 0;
	}

	size_t get_file_size(FILE * fp);

	// Fl_input returns a string but that value might be a int.
	int valuestr_to_int(const Fl_Input * input);

	// Get number of search_char's in str_buf.
	long int get_char_count(const char * str_buf, char search_char, size_t n);

	int get_file_full_path(const char * file_name, char * full_path); // NAME_MAX is the max size.
	int get_file_location(const char * file_path, char * file_locat);
	int get_file_name(const char * file_path, char * file_name);

	// String to char.
	int str_to_int(const char * str, size_t n, int * int_value, bool force_hex=false);
	int char_to_int(char c); // -1 on error.
	int hex_char_to_int(char c);

	std::vector<std::string> get_paths_with_label(const char * image_loader_path, const char * label);

	// File types for game data.
	enum GameDataFileTypes {
		GAME_NONE,
		GAME_MAP,
		GAME_COMMAND_SCRIPT,
		GAME_IMAGE_LOADER,
		GAME_SETTINGS
	};

	typedef unsigned char GAME_DATA_TYPE;

	struct GameFileTypeData {
		char file_ext[NAME_MAX];
		GAME_DATA_TYPE type;
	};

	const size_t FILE_TYPE_DATA_SIZE = 4;
	extern const GameFileTypeData FILE_TYPE_DATA[FILE_TYPE_DATA_SIZE];

	int get_file_ext(const char * file_name, char * file_ext, size_t file_ext_size);
	GAME_DATA_TYPE file_ext_to_datatype(const char * file_ext); // NAME_MAX as
																// limit.
	int load_file(MainData * mdata, const char * file_path, const char * image_folder=NULL);

	bool same_image(const Fl_PNG_Image * img1, const Fl_PNG_Image * img2);
	bool no_failed_images(std::vector<Fl_PNG_Image*> image_list);
	int set_image_list(std::vector<Fl_PNG_Image*> * image_list1, std::vector<Fl_PNG_Image*> image_list2);

	void set_scrollbar_bounds(MainData * mdata, int width, int height);

	// Collision stuff.
	bool did_collide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

	// NAME_MAX for 'split_by'.
	int str_array_to_str(const char ** str_array, int width, int height, const char * split_by, char * str_output, size_t str_output_size, int start_at=0);
}
