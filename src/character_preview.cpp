#include "character_preview.h"

void CharacterPreview::draw() {
	if (character == NULL)
		return;

	character->draw();
}

void CharacterPreview::set_character(Character * character) {
	CommonTool * tool = NULL;

	free_character();
	this->character = character;

	if (character == NULL)
		return;

	// Handle direction and tool.
	character->facing_right();
	tool = character->get_current_tool();

	if (tool != NULL) {
		tool->move_to_location();
		tool->set_right();
	}

	size(character->get_width(), character->get_height());
	character->position(x(), y());
}

void CharacterPreview::free_character() {
	if (character == NULL)
		return;

	delete character;
	character = NULL;
}

