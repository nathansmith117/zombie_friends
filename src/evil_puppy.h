#include "program_data.h"
#include "npc.h"
#include "game_tools.h"

enum EVIL_PUPPY_FRAME_DIRECTIONS {
	EVIL_PUPPY_RIGHT = 0,
	EVIL_PUPPY_LEFT = 3
};

class EvilPuppy : public Npc {
	public:
		EvilPuppy(MainData * md) : Npc(md) {
			main_init(md);
		}

		void update();
		void handle_items();

		void go_right();
		void go_left();

		bool facing_right();
		bool facing_left();

		void refresh_images();

	private:
		void main_init(MainData * md);
};
