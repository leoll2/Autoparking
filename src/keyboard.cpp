#include <iostream>

#include "keyboard.h"

std::atomic<bool> should_stop = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_reset = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_train = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_store_cache = ATOMIC_VAR_INIT(false);
std::atomic<bool> should_load_cache = ATOMIC_VAR_INIT(false);
std::atomic<bool> paused = ATOMIC_VAR_INIT(false);

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


void poll_commands() {

	ALLEGRO_EVENT ev;

	while (al_get_next_event(event_queue, &ev)) {
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_P:
					paused = !paused;
					break;
				case ALLEGRO_KEY_M:
				    // CHANGE MAP
					break;
				case ALLEGRO_KEY_Q:
					should_stop = true;
					break;
				case ALLEGRO_KEY_R:
					should_reset = true;
					break;
				case ALLEGRO_KEY_T:
					should_train = !should_train;
					break;
				case ALLEGRO_KEY_S:
					should_store_cache = true;
					break;
				case ALLEGRO_KEY_L:
					should_load_cache = true;
					break;
				default:
					break;
	         }
		}
	}
}