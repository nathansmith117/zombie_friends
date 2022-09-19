#pragma once

#include "program_data.h"

class ProgressBar : public Fl_Window {
	public:
		ProgressBar(MainData * md) : Fl_Window(0, 0, 0, 0) {
			main_init(md);
		}

		void show_progress();

		void set_range(float min, float max);

		float get_min() { return progress->minimum(); }
		float get_max() { return progress->maximum(); }

		float progress_value() { return progress->value(); }
		void progress_value(float v) { progress->value(v); }

		void progress_label(const char * l) { progress->copy_label(l); }

		// Set label to percent through;
		void set_value_and_label(float v);
	protected:
		MainData * mdata;

		Fl_Progress * progress = NULL;

		void main_init(MainData * md);
};
