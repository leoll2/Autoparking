#include <algorithm>
#include <functional>
#include "display.h"
#include "display_params.h"
#include "field_params.h"

#define COLOR_LIME		al_map_rgb(200,255,140)
#define COLOR_AMARANTH	al_map_rgb(100,0,0)
#define COLOR_GREY		al_map_rgb(160,160,160)
#define COLOR_SKYBLUE	al_map_rgb(135,206,235)
#define COLOR_RED		al_map_rgb(255,0,0)
#define COLOR_GREEN		al_map_rgb(0,255,0)


float op_scale(float a) {
	return a / COORD_PIXEL_RATIO;
}

/* This is used in a transformation of an array of (x,y) interleaved 
 * points to reverse the y coordinate only.*/
float op_reverse(float a) {
	static bool even = true;
	even = !even;
	if (even) 
		return VREF_POINTS * VISUAL_UNIT - a;
	else
		return a;
}

bool initialize_display(ALLEGRO_DISPLAY *display) {
	if(!al_init()) {
      std::cerr << "failed to initialize allegro!" << std::endl;
      return false;
	}
	display = al_create_display(DISP_WIDTH, DISP_HEIGHT);
	if(!display) {
	  std::cerr << "failed to create display!" << std::endl;
	  return false;
	}
	return true;
}

bool initialize_primitives() {
	if (!al_init_primitives_addon()) {
		std::cerr << "failed to initialize allegro5 primitives!" << std::endl;
		return false;
	}
	return true;
}

void draw_polygon(unsigned int n_vertices, float* vertices, ALLEGRO_COLOR color) {
	//scale the coordinates to appropriate pixel lengths
	std::transform(vertices, vertices + 2*n_vertices, vertices, op_scale);
	//reverse the y axis
	std::transform(vertices, vertices + 2*n_vertices, vertices, op_reverse);
	
	al_draw_filled_polygon(vertices, n_vertices, color);
	delete[] vertices;
}

bool start_graphics(ALLEGRO_DISPLAY *display) {
	return initialize_display(display) && initialize_primitives();
}

void display_all(const Map& map, const Vehicle& car) {
	//clear the field
	al_clear_to_color(COLOR_LIME);
	//draw the map
	al_draw_filled_rectangle(0, 0, HREF_POINTS * VISUAL_UNIT, VREF_POINTS * VISUAL_UNIT, COLOR_GREY);
	//draw the obstacles
	for (auto ob : map.obstacles)
		draw_polygon(ob.get_n_sides(), ob.get_vertices2(), COLOR_AMARANTH);
	//draw the car
	if (car.verify_collision(map) || !map.is_within_boundaries(car.to_polygon()))
		draw_polygon(4, car.get_vertices2(), COLOR_RED);
	else
		draw_polygon(4, car.get_vertices2(), COLOR_GREEN);
	al_flip_display();
}

void end_graphics(ALLEGRO_DISPLAY *display) {
	al_destroy_display(display);
}