#include <algorithm>
#include <chrono>
#include <functional>
#include <thread>
#include "display.h"
#include "display_params.h"
#include "field_params.h"
#include "maneuver.h"

#define COLOR_LIME	al_map_rgb(200,255,140)
#define COLOR_AMARANTH	al_map_rgb(100,0,0)
#define COLOR_GREY	al_map_rgb(96,96,96)
#define COLOR_SKYBLUE	al_map_rgb(135,206,235)
#define COLOR_RED	al_map_rgb(255,0,0)
#define COLOR_GREEN	al_map_rgb(0,102,0)


/**
 * Scale the coordinate to fit in the screen.
 * 
 * @param a the original coordinate
 * @return the transformed coordinate
 */
float op_scale(float a) {
    return a / COORD_PIXEL_RATIO;
}


/**
 * Convert the coordinate to another coordinate system with reversed axis.
 * 
 * @param a the original coordinate
 * @return the transformed coordinate
 */
float op_reverse(float a) {
    return VREF_POINTS * VISUAL_UNIT - a;
}


/**
 * Utility function used in the transformation of an array of (x,y) interleaved 
 * points to reverse the y coordinate only.
 * 
 * @param a the original coordinate
 * @return reversed coordinate if it was 'y', else the original coordinate
 */
float op_reverse_array(float a) {
    static bool even = true;
    even = !even;
    if (even) 
        return op_reverse(a);
    else
        return a;
}


/**
 * Graphics initialization routine.
 * 
 * @param display pointer to Allegro display
 * @return true if success, false otherwise
 */
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


/**
 * Routine for the initialization of Allegro drawing primitives.
 * 
 * @return true if success, false otherwise
 */
bool initialize_primitives() {
    if (!al_init_primitives_addon()) {
        std::cerr << "failed to initialize allegro5 primitives!" << std::endl;
        return false;
    }
    return true;
}


/**
 * Draw a filled polygon given its vertices.
 * 
 * @param n_vertices the number of vertices
 * @param vertices the array of point coordinates, interleaved x and y
 * @param color the color used to fill the area
 */
void draw_polygon(unsigned int n_vertices, float* vertices, ALLEGRO_COLOR color) {
    
    // Scale the coordinates to appropriate pixel lengths
    std::transform(vertices, vertices + 2*n_vertices, vertices, op_scale);
    // Reverse the y axis to fit in this application coordinate system
    std::transform(vertices, vertices + 2*n_vertices, vertices, op_reverse_array);

    al_draw_filled_polygon(vertices, n_vertices, color);
    delete[] vertices;
}


/**
 * Draw a filled circle given position and radius.
 * 
 * @param cx the x coordinate of the center
 * @param cy the y coordinate of the center
 * @param r the radius
 * @param color the color used to fill the area
 */
void draw_circle(float cx, float cy, float r, ALLEGRO_COLOR color) {
    
    // Adjust the coordinates
    cx = op_scale(cx);
    cy = op_reverse(op_scale(cy));
    r = op_scale(r);
    
    al_draw_filled_circle(cx, cy, r, color);
}


/**
 * Draw the field and its obstacles.
 * 
 * @param map a reference to the map
 */
void draw_map(const Map& map) {
    
    // Clear the field
    al_clear_to_color(COLOR_LIME);
    
    // Draw the map
    al_draw_filled_rectangle(0, 0, map.width * VISUAL_UNIT, map.height * VISUAL_UNIT, COLOR_GREY);
    
    // Draw the obstacles
    for (auto ob : map.obstacles)
        draw_polygon(ob.get_n_sides(), ob.get_vertices2(), COLOR_AMARANTH);
}


/**
 * Utility function to store the coordinates of a quadrangle into an array whose
 * format complies with Allegro functions.
 * 
 * @param points the array where to store the points in Allegro format 
 * @param A the first Coordinate
 * @param B the second Coordinate
 * @param C the third Coordinate
 * @param D the fourth Coordinate
 */
void load_into_array(float* points, const Coordinate& A, const Coordinate& B,
                                    const Coordinate& C, const Coordinate& D) {
    points[0] = A.x;
    points[1] = A.y;
    points[2] = B.x;
    points[3] = B.y;
    points[4] = C.x;
    points[5] = C.y;
    points[6] = D.x;
    points[7] = D.y;
}


/**
 * Draw a filled quadrangle.
 * 
 * @param A the first Coordinate
 * @param B the second Coordinate
 * @param C the third Coordinate
 * @param D the fourth Coordinate
 * @param color the color used to fill the area
 */
void draw_quadrangle(const Coordinate& A, const Coordinate& B,
                     const Coordinate& C, const Coordinate& D,
                     ALLEGRO_COLOR color) {
    float *corners = (float*) malloc(8 * sizeof(float));
    load_into_array(corners, A, B, C, D);
    draw_polygon(4, corners, color);
}


/**
 * Draw a car with all its fancy graphical details.
 * 
 * @param map the map containing the car (used for collision detection)
 * @param car the car to be drawn
 */
void draw_car(const Map& map, const Vehicle& car) {
    
    // Draw the body of the car: red if in a bad position, green otherwise 
    if (car.verify_collision(map) || !map.is_within_boundaries(car.to_polygon()))
        draw_polygon(4, car.get_vertices2(), COLOR_RED);
    else
        draw_polygon(4, car.get_vertices2(), COLOR_GREEN);
    
    // Compute some useful reference points
    std::vector<Coordinate> car_vertices = car.get_vertices1();
    Coordinate circumcenter = (car_vertices[0] + car_vertices[2]) * 0.5;
    Coordinate right_center = (car_vertices[1] + car_vertices[2]) * 0.5;
    Coordinate front_center = car.get_front_center();
    Direction fore = front_center - circumcenter;
    Direction right = right_center -  circumcenter;
    
    // Draw the windows of the car (just for the sake of it)
    // Left window
    Coordinate A = circumcenter -0.9 * right -0.8  * fore;
    Coordinate B = circumcenter -0.8 * right -0.75 * fore;
    Coordinate C = circumcenter -0.8 * right +0.2  * fore;
    Coordinate D = circumcenter -0.9 * right +0.5  * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
    // Right window
    A = circumcenter +0.8 * right -0.75 * fore;
    B = circumcenter +0.9 * right -0.8  * fore;
    C = circumcenter +0.9 * right +0.5  * fore;
    D = circumcenter +0.8 * right +0.2  * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
    // Windscreen
    A = circumcenter -0.9 * right +0.6  * fore;
    B = circumcenter -0.8 * right +0.3  * fore;
    C = circumcenter +0.8 * right +0.3  * fore;
    D = circumcenter +0.9 * right +0.6  * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
    // Rear window
    A = circumcenter -0.9 * right -0.95 * fore;
    B = circumcenter +0.9 * right -0.95 * fore;
    C = circumcenter +0.8 * right -0.8  * fore;
    D = circumcenter -0.8 * right -0.8  * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
    
    // Draw the headlights of the car (just for the sake of it, again)
    // Left headlight
    A = circumcenter -0.9 * right +0.9  * fore;
    B = circumcenter -0.7 * right +0.9  * fore;
    C = circumcenter -0.7 * right +0.95 * fore;
    D = circumcenter -0.9 * right +0.95 * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
    // Right headlight
    A = circumcenter +0.7 * right +0.9  * fore;
    B = circumcenter +0.9 * right +0.9  * fore;
    C = circumcenter +0.9 * right +0.95 * fore;
    D = circumcenter +0.7 * right +0.95 * fore;
    draw_quadrangle(A, B, C, D, COLOR_SKYBLUE);
}


/**
 * Initialize the Allegro graphics.
 * 
 * @param display the pointer to Allegro display
 * @return true if success, false otherwise
 */
bool start_graphics(ALLEGRO_DISPLAY *display) {
	return initialize_display(display) && initialize_primitives();
}


/**
 * Show all the entities within the field.
 * 
 * @param map the map
 * @param car the car within the map
 */
void display_all_entities(const Map& map, const Vehicle& car) {
    
    // Draw the field
    draw_map(map);

    // Draw the car
    draw_car(map, car);

    al_flip_display();
}


/**
 * Draw all the entities of the field adding an animation to the car to look like
 * when performing a maneuver. The quality of the animation depends on the
 * number of frames. It's also possible to specify its speed. The movement effect
 * is generated using an (approximated) interpolation of position and orientation.
 * 
 * @param map the map
 * @param car_start the initial position of the car
 * @param car_end the end position of the car
 * @param frames the number of frames in the animation
 * @param millis the delay between two frames in the animation
 */
void display_all_entities_enhanced(const Map& map, const Vehicle& car_start, 
                                   const Vehicle& car_end, unsigned int frames,
                                   unsigned int millis) {
    
    Coordinate start_pos = car_start.get_rear_center();
    Coordinate final_pos = car_end.get_rear_center();
    double start_angle = car_start.get_orientation();
    double final_angle = car_end.get_orientation();
    double psi = final_angle - start_angle;
    Direction d_vector = final_pos - start_pos;
    double d_angle = d_vector.get_angle();
    double dist = d_vector.get_modulus();
    
    if (psi == 0) {     // if vehicle is going straight
        for (unsigned int f = 1; f <= frames; ++f) {
            Coordinate new_pos = start_pos + (d_vector * (f/(double)frames));
            Vehicle dummy(car_start.get_length(), car_start.get_width(), new_pos, 
                    start_angle, false);
            display_all_entities(map, dummy);
            std::this_thread::sleep_for(std::chrono::milliseconds(millis));
        }
    } else {            // if the vehicle is steering
        double r = 0.5 * dist / std::sin(std::abs(psi)/2);
        double theta = 0.5 * (pi - std::abs(psi));
        Coordinate rot_center = (psi < 0) ?
                start_pos + Direction::from_angle(d_angle - theta) * r :
                start_pos + Direction::from_angle(d_angle + theta) * r;
        double rot_angle_unit = psi / frames;
        for (unsigned int f = 1; f <= frames; ++f) {
            Coordinate new_pos = start_pos.rotate(rot_center, f * rot_angle_unit);
            Vehicle dummy(car_start.get_length(), car_start.get_width(), new_pos, 
                    start_angle + f * rot_angle_unit, false);
            display_all_entities(map, dummy);
            std::this_thread::sleep_for(std::chrono::milliseconds(millis));
        }
    }
}


/**
 * Generate a color for a reward.
 * 
 * @param reward the reward
 * @return the color associated to that reward
 */
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


/**
 * Computes the offset position of the reward associate to an action with respect
 * to the state center.This is used to get a visual representation of rewards.
 * 
 * @param x the x of state center
 * @param y the y of state center
 * @param mvn the maneuver
 * @return the position
 */
std::vector<float> compute_reward_pos(float x, float y, const Maneuver& mvn) {
    std::vector<float> ret(2);
    y += mvn.get_verse() * mvn.get_displacement() / 50 * 6;
    x += mvn.get_verse() * mvn.get_spin() * (3 - mvn.get_turning_radius() / MIN_RADI) * 6;
                
    ret[0] = x;
    ret[1] = y;
    return ret;
}


/**
 * Provide a visual representation of a certain subset of the rewards.
 * 
 * @param ai the Q-learning network
 */
void display_rewards(const Q_LearningNetwork& ai) {
    
    draw_map(ai.get_map());
    
    unsigned int n_states = ai.get_n_states();
    unsigned int n_actions  = ai.get_n_actions();
    for (unsigned int s = 0; s < n_states; ++s) {
        std::vector<double> state = Vehicle::decode_vehicle(s);
        double orientation = state[0];
        double x = state[1];
        double y = state[2];
        if (std::abs(orientation - 1.61) > 0.05)   // consider only states with orientation == pi/2
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


/**
 * Close the Allegro graphics.
 * 
 * @param display the pointer to Allegro display
 */
void end_graphics(ALLEGRO_DISPLAY *display) {
	al_destroy_display(display);
}
