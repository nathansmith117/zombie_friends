#pragma once

#include "program_data.h"
#include "npc.h"
#include "game_tools.h"

#define NPC_DATA_TOOLS_SIZE 5

struct NpcData {
	NPC_TYPE type;
	int16_t x, y;

	int8_t health;
	int8_t coins;

	// Tools that npc will start with.
	int8_t tools[NPC_DATA_TOOLS_SIZE] = {
		0, 0, 0, 0, 0
	};

	int8_t fuel[NPC_DATA_TOOLS_SIZE] = {
		0, 0, 0, 0, 0
	};
};

class NpcMap {
	public:
		NpcMap(MainData * md);
		~NpcMap() { delete_map(); }

		Npc *** map_data() { return map; }
		void map_data(Npc *** map);

		int create_map(int width, int height);
		void delete_map();

		void add_npcs_from_data(NpcData * npc_data, size_t n);

		int get_width() { return width; }
		int get_height() { return height; }

		int npc(Npc * new_npc, int x, int y); // Can set npc to null.
		int npc(Npc * new_npc);
		int npc(NpcData npc_data);
		Npc * npc(int x, int y);
		int remove(int x, int y);

		int npcs_in_use_count() { return npcs_in_use.size(); }
		void clear_npcs_in_use() { npcs_in_use.clear(); }
		std::vector<Npc*> get_npcs_in_use() { return npcs_in_use; }

		void draw();
		void update();

		void put_npc_to_use(int x, int y);
		void remove_npc_from_use(int location);
		void remove_npc_from_use(Npc * unused_npc);

		void delete_npc_in_use(int location);
		void delete_npc_in_use(Npc * unused_npc);

		bool is_in_use(Npc * the_npc);
	private:
		MainData * mdata;
		int width, height;

		Npc *** map;
		std::vector<Npc*> npcs_in_use;

		void update_npcs_in_use();
};

Npc * get_npc_from_type(MainData * mdata, NpcMap * npc_map, NPC_TYPE type);
