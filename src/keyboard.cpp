#include <iostream>

#include "keyboard.h"

ALLEGRO_EVENT_QUEUE *event_queue = NULL;

bool start_keyboard() {

	if (!al_install_keyboard())
		return false;

	event_queue = al_create_event_queue();
	if(!event_queue)
		return false;

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	return true;
}


void stop_keyboard() {

	al_uninstall_keyboard();
}


void poll_command() {

	ALLEGRO_EVENT ev;

	while (al_get_next_event(event_queue, &ev)) {
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
	            case ALLEGRO_KEY_Q:
	               std::cout << "Q key pressed" << std::endl;
	               break;
	            case ALLEGRO_KEY_W:
	               std::cout << "W key pressed" << std::endl;
	               break;
	            case ALLEGRO_KEY_E:
	               std::cout << "E key pressed" << std::endl;
	               break;
	            case ALLEGRO_KEY_R:
	               std::cout << "R key pressed" << std::endl;
	               break;
	            case ALLEGRO_KEY_ESCAPE:
	               std::cout << "ESC key pressed" << std::endl;
	               break;
	            default:
	            	break;
	         }
		}
	}
}