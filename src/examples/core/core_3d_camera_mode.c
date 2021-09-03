#include "../../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - 3d camera mode"
};

// Global vars

rf_camera3d camera = {
	.position = (rf_vec3){ 0.0f, 10.0f, 10.0f },
	.target = (rf_vec3){ 0.0f, 0.0f, 0.0f },
	.up = (rf_vec3){ 0.0f, 1.0f, 0.0f },
	.fovy = 45.0f,
	.type = RF_CAMERA_PERSPECTIVE,
};

rf_vec3 cube_position = { 0.0f, 0.0f, 0.0f };

uint64_t frame_start_time;
float delta_time;
char fps_text[10];

// Initialization
extern void game_init() {
	// Setup sokol_time.h
	stm_setup();
}

// Main Loop
extern void game_update() {
	frame_start_time = stm_now();

	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
		rf_begin_3d(camera);
			rf_draw_cube(cube_position, 2.0f, 2.0f, 2.0f, RF_RED);
			rf_draw_cube_wires(cube_position, 2.0f, 2.0f, 2.0f, RF_MAROON);
			rf_draw_grid(10, 1.0f);
		rf_end_3d();
    	rf_draw_text("Welcome to the third dimension!", 10, 40, 20, RF_DARKGRAY);
		sprintf(fps_text, "%d FPS", (int)(1/delta_time));
		rf_draw_text(fps_text, 10, 10, 20, RF_LIME);
    rf_end();

	delta_time = stm_sec(stm_since(frame_start_time));
}

// Event callback
extern void game_event(const sapp_event* event) {
	// Do nothing
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}