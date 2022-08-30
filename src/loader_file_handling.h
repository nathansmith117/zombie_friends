#pragma once

#include "program_data.h"
#include "image_loader.h"
#include "game_tools.h"

// Loading stuff.
int load_images_from_loader_file(MainData * mdata, const char * file_path, const char * image_folder=NULL);

int load_player_images(MainData * mdata, const char * file_path, const char * img_location);
int load_weapon_images(MainData * mdata, const char * file_path, const char * img_location);
int load_world_images(MainData * mdata, const char * file_path, const char * img_location);
int load_npc_images(MainData * mdata, const char * file_path, const char * img_location);
