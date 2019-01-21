#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <atomic>
#include <allegro5/allegro.h>

extern std::atomic<bool> should_stop;
extern std::atomic<bool> should_reset;
extern std::atomic<bool> should_train;
extern std::atomic<bool> should_store_cache;
extern std::atomic<bool> should_load_cache;
extern std::atomic<bool> should_change_map;
extern std::atomic<bool> paused;

extern std::atomic<unsigned int> current_map;

bool start_keyboard(); 

void stop_keyboard();

void poll_commands();

#endif