#include "edit_commands.h"

int edit_create_command(COM_CB_ARGS) {
	int width, height;

	if (arg_count != 3) {
		fputs("Two arguments required\n", stderr);
		return -1;
	}

	// Get width.
	if (gameTools::str_to_int(args[1], NAME_MAX, &width) == -1) {
		fputs("width has to be an interager\n", stderr);
		return -1;
	}

	// Get height.
	if (gameTools::str_to_int(args[2], NAME_MAX, &height) == -1) {
		fputs("height has to be an interager\n", stderr);
		return -1;
	}

	// Create new map.
	return mdata->map->create_map(width, height);
}

int edit_fill_command(COM_CB_ARGS) {
	EditTileCreater * tile_creater = ((EditTabMenu*)mdata->tab_menu)->tile_creater;

	int start_x, start_y, end_x, end_y;

	// Check for errors.
	if (tile_creater == NULL) {
		fputs("Tile creator is null\n", stderr);
		return -1;
	} else if (mdata->map->map_data() == NULL) {
		fputs("Map not created\n", stderr);
		return -1;
	}

	// If no arguments fill the whole map.
	if (arg_count == 1) {
		if (mdata->map->fill_with(tile_creater->tile()) == -1) {
			fputs("Error filling map\n", stderr);
			return -1;
		}

		return 0;
	}

	// 4 arguments.
	if (arg_count != 5) {
		fputs("4 arguments or none at all\n", stderr);
		return -1;
	}

	// Get values from args.
	
	// Start x.
	if (gameTools::str_to_int(args[1], NAME_MAX, &start_x) == -1) {
		fputs("Start x must be a number\n", stderr);
		return -1;
	}

	// Start y.
	if (gameTools::str_to_int(args[2], NAME_MAX, &start_y) == -1) {
		fputs("Start y must be a number\n", stderr);
		return -1;
	}

	// End x.
	if (gameTools::str_to_int(args[3], NAME_MAX, &end_x) == -1) {
		fputs("End x must be a number\n", stderr);
		return -1;
	}

	// End y.
	if (gameTools::str_to_int(args[4], NAME_MAX, &end_y) == -1) {
		fputs("End y must be a number\n", stderr);
		return -1;
	}

	// Fill map.
	if (mdata->map->fill_with(tile_creater->tile(), start_x, start_y, end_x, end_y) == -1) {
		fputs("Error filling map\n", stderr);
		return -1;
	}

	return 0;
}

void add_edit_commands(MainData * mdata, ChatBox * chat_box) {
	ChatCommand command_list[] = {
		{"create_map", edit_create_command, NULL},
		{"fill_map", edit_fill_command, NULL}
	};

	chat_box->add_command_list(command_list, sizeof(command_list) / sizeof(ChatCommand));
}
