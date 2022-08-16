#include "debug_window.h"
#include "game_tools.h"

void DebugWindow::main_init(MainData * md, int X, int Y, int W, int H) {
    mdata = md;

	Fl::add_timeout(1.0 / mdata->settings.debug_win_update_speed, update_cb, (void*)this);

	old_stdout_size = 0;
	old_stderr_size = 0;

	text_buffer = new Fl_Text_Buffer();
	buffer(text_buffer);

	do_init = true;
}

void DebugWindow::update_cb(void * d) {
	Fl::lock();
	DebugWindow * win = (DebugWindow*)d;
	win->update();
	Fl::repeat_timeout(1.0 / win->mdata->settings.debug_win_update_speed, update_cb, d);
	Fl::unlock();
}

void DebugWindow::update() {
	long int i;
	char * value_buf;

	size_t new_stdout_size = gameTools::get_file_size(stdout);
	size_t new_stderr_size = gameTools::get_file_size(stderr);

	// First output
    if (do_init) {
		buffer_init(new_stdout_size, new_stderr_size);
	}

	// New output.
	if (new_stdout_size != old_stdout_size)
		update_stream(new_stdout_size, &old_stdout_size, stdout);
	if (new_stderr_size != old_stderr_size)
		update_stream(new_stderr_size, &old_stderr_size, stderr);
}

void DebugWindow::buffer_init(size_t new_stdout_size, size_t new_stderr_size) {
	long int i;
	char * value_buf = NULL;

	do_init = false;

	// Stdout.
	if (new_stdout_size > 0) {
		// Get memory for 'value_buf'.
		value_buf = new char[new_stdout_size];

		if (value_buf == NULL) {
			fputs("Error getting memory for 'value_buf'\n", stderr);
			return;
		}

		memset(value_buf, 0, new_stdout_size);

		// Read stdout.
		fseek(stdout, 0, SEEK_SET);
		fread(value_buf, 1, new_stdout_size, stdout);

		// Setting the last byte to zero stop weird junk from getting in your strings!.
		value_buf[new_stdout_size - 1] = '\0';
		text_buffer->text(value_buf);
		text_buffer->append("\n");

		old_stdout_size = new_stdout_size;
		delete [] value_buf;
		value_buf = NULL;
	}

	// Stderr.
	if (new_stderr_size > 0) {
		// Get memory for 'value_buf'.
		value_buf = new char[new_stderr_size];

		if (value_buf == NULL) {
			fputs("Error getting memory for 'value_buf'\n", stderr);
			return;
		}

		memset(value_buf, 0, new_stderr_size);

		// Read stderr.
		fseek(stderr, 0, SEEK_SET);
		fread(value_buf, 1, new_stderr_size, stderr);

		value_buf[new_stderr_size - 1] = '\0';
		text_buffer->append(value_buf);
		text_buffer->append("\n");
		old_stderr_size = new_stderr_size;

		if (mdata->settings.show_error_as_popup)
			fl_alert("%s", value_buf);

		delete [] value_buf;
		value_buf = NULL;
	}

	// Scroll to bottom.
	scroll_to_bottom();
}

void DebugWindow::update_stream(size_t new_size, size_t * old_size, FILE * fp) {
	char * value_buf = NULL;
	int i;
	char end_char[2];

	size_t new_value_size = new_size - *old_size;
	
	if (new_value_size <= 0) {
		fprintf(stderr, "new_char_size in %s:%d was less then 1\n", __FILE__, __LINE__);
		*old_size = new_size;
		return;
	}

	fseek(fp, -new_value_size, SEEK_CUR);

	// Create 'value_buf'.
	value_buf = new char[new_value_size];

	if (value_buf == NULL) {
		fputs("Error getting memory for 'value_buf'\n", stderr);
		return;
	}
	
	memset(value_buf, 0, new_value_size);

	// Read from stream.
	if (fread(value_buf, 1, new_value_size, fp) == -1) {
		fputs("Error reading from input stream\n", stderr);
		delete [] value_buf;
		return;
	}

	end_char[0] = value_buf[new_value_size - 1];
	end_char[1] = '\0';
	// This line of code caused me a lot of head ace.
	value_buf[new_value_size - 1] = '\0';

	// Stderr only.
	if (fp == stderr && mdata->settings.show_error_as_popup)
		fl_alert("%s", value_buf);

	text_buffer->append(value_buf);
	text_buffer->append(end_char);
	*old_size = new_size;
	delete [] value_buf;
	scroll_to_bottom();
}

void DebugWindow::scroll_to_bottom() {
	scroll(text_buffer->count_lines(0, text_buffer->length()), 0);
}
