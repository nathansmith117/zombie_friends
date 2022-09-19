#include "progress_bar.h"

void ProgressBar::main_init(MainData * md) {
	mdata = md;

	resize(
		Fl::event_x_root(),
		Fl::event_y_root(),
		mdata->settings.progress_bar.width,
		mdata->settings.progress_bar.height
	);

	int progress_height = mdata->settings.progress_bar.height / 2;

	progress = new Fl_Progress(
		0,
		progress_height / 2,
		mdata->settings.progress_bar.width,
		progress_height
	);

	progress->color(0x88888800);
    progress->selection_color(0x4444ff00);
    progress->labelcolor(FL_WHITE);

	end();
}

void ProgressBar::show_progress() {
	show();
	position(Fl::event_x_root(), Fl::event_y_root());
	redraw();
}

void ProgressBar::set_range(float min, float max) {
	progress->minimum(min);
	progress->maximum(max);
}

void ProgressBar::set_value_and_label(float v) {
	int percent;
	float curr_range;
	char buf[NAME_MAX];

	// Get percent.
	curr_range = abs(get_max() - get_min());
	percent = v / curr_range * 100.0;

	// Format.
	memset(buf, 0, NAME_MAX);
	snprintf(buf, NAME_MAX, "%d%%", percent);

	progress_label(buf);
	progress_value(v);
}
