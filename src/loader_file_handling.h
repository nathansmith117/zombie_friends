#pragma once

#include "program_data.h"
#include "image_loader.h"
#include "game_tools.h"

struct ImageListLoadingData {
	char label[NAME_MAX];
	std::vector<Fl_PNG_Image*> * img_list_locat = NULL;
	std::vector<Fl_PNG_Image*> * scaled_img_list_locat = NULL;

	ImageListLoadingData(
		const char * label,
		std::vector<Fl_PNG_Image*> * img_list_locat,
		std::vector<Fl_PNG_Image*> * scaled_img_list_locat
	) {
		memset(this->label, 0, NAME_MAX);
		strncat(this->label, label, NAME_MAX - 1);

		this->img_list_locat = img_list_locat;
		this->scaled_img_list_locat = scaled_img_list_locat;
	}

};

std::vector<ImageListLoadingData> get_image_list_load_data(MainData * mdata);

int load_and_store_image_data(
	MainData * mdata, 
	const char * file_path,
	const char * img_location,
	ImageListLoadingData img_list_data
);


// Loading stuff.
int load_images_from_loader_file(MainData * mdata, const char * file_path, const char * image_folder=NULL);

void scale_all_image_lists(MainData * mdata);
