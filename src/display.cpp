#include <algorithm>
#include <functional>
#include "display.h"
#include "display_params.h"
#include "field_params.h"
#include "maneuver.h"

#define COLOR_LIME	al_map_rgb(200,255,140)
#define COLOR_AMARANTH	al_map_rgb(100,0,0)
#define COLOR_GREY	al_map_rgb(160,160,160)
#define COLOR_SKYBLUE	al_map_rgb(135,206,235)
#define COLOR_RED	al_map_rgb(255,0,0)
#define COLOR_GREEN	al_map_rgb(0,255,0)


float op_scale(float a) {
    return a / COORD_PIXEL_RATIO;
}

float op_reverse(float a) {
    return VREF_POINTS * VISUAL_UNIT - a;
}

/* This is used in a transformation of an array of (x,y) interleaved 
 * points to reverse the y coordinate only.*/
float op_reverse_array(float a) {
    static bool even = true;
    even = !even;
    if (even) 
        return op_reverse(a);
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
    std::transform(vertices, vertices + 2*n_vertices, vertices, op_reverse_array);

    al_draw_filled_polygon(vertices, n_vertices, color);
    delete[] vertices;
}

void draw_circle(float cx, float cy, float r, ALLEGRO_COLOR color) {
    // Adapt the coordinates to allegro
    cx = op_scale(cx);
    cy = op_reverse(op_scale(cy));
    r = op_scale(r);
    // Draw the circle
    al_draw_filled_circle(cx, cy, r, color);
}

void draw_map(const Map& map) {
    //clear the field
    al_clear_to_color(COLOR_LIME);
    //draw the map
    al_draw_filled_rectangle(0, 0, map.width * VISUAL_UNIT, map.height * VISUAL_UNIT, COLOR_GREY);
    //draw the obstacles
    for (auto ob : map.obstacles)
            draw_polygon(ob.get_n_sides(), ob.get_vertices2(), COLOR_AMARANTH);
}

bool start_graphics(ALLEGRO_DISPLAY *display) {
	return initialize_display(display) && initialize_primitives();
}

void display_all(const Map& map, const Vehicle& car) {
        // Draw the field
        draw_map(map);
        
	// Draw the car
	if (car.verify_collision(map) || !map.is_within_boundaries(car.to_polygon()))
		draw_polygon(4, car.get_vertices2(), COLOR_RED);
	else
		draw_polygon(4, car.get_vertices2(), COLOR_GREEN);
	al_flip_display();
}

ALLEGRO_COLOR compute_reward_color(float reward) {
    unsigned int red = 0, green = 0, blue = 0;
    if (reward == 1000)
        green = 255;
    else if (reward > 0) {
        blue = (unsigned int)(reward * 255 / 1000);
    } else {
        red = (unsigned int)(-reward * 255 / 200);
    }
    return al_map_rgb(red, green, blue);
}

std::vector<float> compute_reward_pos(float x, float y, const Maneuver& mvn) {
    std::vector<float> ret(2);
    y += mvn.get_verse() * mvn.get_displacement() / 50 * 6;
    x += mvn.get_verse() * mvn.get_spin() * (3 - mvn.get_turning_radius() / MIN_RADI) * 6;
                
    ret[0] = x;
    ret[1] = y;
    return ret;
}

void display_rewards(const Q_LearningNetwork& ai) {
    
    draw_map(ai.get_map());
    
    unsigned int n_states = ai.get_n_states();
    unsigned int n_actions  = ai.get_n_actions();
    for (unsigned int s = 0; s < n_states; ++s) {
        std::vector<double> state = Vehicle::decode_vehicle(s);
        double orientation = state[0];
        double x = state[1];
        double y = state[2];
        if (abs(orientation - 1.61) > 0.05)         // consider only states with orientation == pi/2
            continue;
        // Draw a black circle where the state is
        draw_circle(x, y, 8, al_map_rgb(0,0,0));
        for (unsigned int a = 0; a < n_actions; ++a) {
            std::cout << "State: " << s << " Action: " << a << std::endl;
            Maneuver mvn(a);
            // Compute the color of the reward to be displayed
            ALLEGRO_COLOR reward_col = compute_reward_color(ai.get_reward(s, a));
            // Compute the position of the reward on the basis of state and action
            std::vector<float> reward_pos = compute_reward_pos(x, y, mvn);
            // Draw the reward
            draw_circle(reward_pos[0], reward_pos[1], 4, reward_col);
        }
    }
    al_flip_display();
}

void end_graphics(ALLEGRO_DISPLAY *display) {
	al_destroy_display(display);
}