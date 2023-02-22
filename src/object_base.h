#pragma once

#include <FL/Fl_PNG_Image.H>
#include "vector"

class ObjectBase {
	public:
		ObjectBase();
		virtual ~ObjectBase();

		virtual void draw() = 0;
		virtual void refresh_images() = 0;
		virtual void update() = 0;

		// Frame.
		int get_frame() { return frame; }
		void set_frame(int frame) { this->frame = frame; }
		Fl_PNG_Image * get_current_image();

		// Size.
		int w() { return width; }
		void w(int width) { this->width = width; }

		int h() { return height; }
		void h(int height) { this->height = height; }

		void resize(int screen_x, int screen_y, int width, int height) {
			x(screen_x);
			y(screen_y);
			w(width);
			h(height);
		}

		void size(int width, int height) {
			w(width);
			h(height);
		}

		// Screen position.
		int x() { return screen_x; }
		void x(int screen_x) { this->screen_x = screen_x; }

		int y() { return screen_y; }
		void y(int screen_y) { this->screen_y = screen_y; }

		void position(int screen_x, int screen_y) {
			x(screen_x);
			y(screen_y);
		}

		// Use these for getting object size.
		virtual int get_width();
		virtual int get_height();

		void delete_images();

		std::vector<Fl_PNG_Image*> images() { return image_frames; }
		void images(std::vector<Fl_PNG_Image*> image_frames);

		Fl_Image * image() { return object_image; }
		void image(Fl_Image * object_image) { this->object_image = object_image; }
	private:
		std::vector<Fl_PNG_Image*> image_frames;

		Fl_Image * object_image = NULL;

		int screen_x, screen_y;
		int width, height;
	protected:
		int frame;
};
