#pragma once

#include "program_data.h"

#define MAX_LINE_SIZE 255

class DebugWindow : public Fl_Text_Display {
    public:
        DebugWindow(MainData * md, int X, int Y, int W, int H) : Fl_Text_Display(X, Y, W, H) {
            main_init(md, X, Y, W, H);
        }

		static void update_cb(void * d);

        void main_init(MainData * md, int X, int Y, int W, int H);
        void update();

		void scroll_to_bottom();
    private:
        MainData * mdata;
		
		size_t old_stdout_size;
		size_t old_stderr_size;

		Fl_Text_Buffer * text_buffer = NULL;
		bool do_init;

		void buffer_init(size_t new_stdout_size, size_t new_stderr_size);
		void update_stream(size_t new_size, size_t * old_size, FILE * fp);
};
