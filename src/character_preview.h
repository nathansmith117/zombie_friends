#pragma once

#include "program_data.h"
#include "character.h"

class CharacterPreview : public Fl_Widget {
	public:
		CharacterPreview(int X, int Y) : Fl_Widget(X, Y, 0, 0) {}

		virtual void draw();

		void set_character(Character * character);
		Character * get_character() { return character; }

		void free_character();
	private:
		Character * character = NULL;
};
