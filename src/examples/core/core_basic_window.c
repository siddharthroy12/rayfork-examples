#include "../../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - basic window"
};

// Initialization
extern void game_init() {
	// Do nothing
}

// Main Loop
extern void game_update() {
	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
    	rf_draw_text("Congrats! You created your first window!", 190, 200, 20, RF_LIGHTGRAY);
    rf_end();
}

// Event callback
extern void game_event(const sapp_event* event) {
	// Do nothing
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}