#include "../../platform.h"
#include "sokol_app.h"
#include <time.h>

#define MAX_COLUMNS 20

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - 3d camera first person"
};

// Helper function
int get_random_value(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

// Global vars
rf_camera3d camera = {
	.position = (rf_vec3){ 4.0f, 2.0f, 4.0f },
	.target = (rf_vec3){ 0.0f, 1.8f, 0.0f },
	.up = (rf_vec3){ 0.0f, 1.0f, 0.0f },
	.fovy = 60.0f,
	.type = RF_CAMERA_PERSPECTIVE,
};

rf_camera3d_state camera_state = { 0 };


// Input states for camera state
rf_input_state_for_update_camera camera_input_state = { 0 };

// Columns
float heights[MAX_COLUMNS] = { 0 };
rf_vec3 positions[MAX_COLUMNS] = { 0 };
rf_color colors[MAX_COLUMNS] = { 0 };


// Initialization
extern void game_init() {
	// Lock the mouse
	sapp_lock_mouse(true);

	// Random should be random
	srand(time(0));

	// Generates some random columns
	for (int i = 0; i < MAX_COLUMNS; i++) {
        heights[i] = (float)get_random_value(1, 12);
        positions[i] = (rf_vec3){ (float)get_random_value(-15, 15), heights[i]/2.0f, (float)get_random_value(-15, 15) };
        colors[i] = (rf_color){ get_random_value(20, 255), get_random_value(10, 55), 30, 255 };
    }	

	rf_set_camera3d_mode(&camera_state, camera, RF_CAMERA_FIRST_PERSON);
}

// Main Loop
extern void game_update() {
	rf_update_camera3d(&camera, &camera_state, camera_input_state);

	// Draw
    rf_begin();

        rf_clear(RF_RAYWHITE);

		rf_begin_3d(camera);

			rf_draw_plane((rf_vec3){ 0.0f, 0.0f, 0.0f }, (rf_vec2){ 32.0f, 32.0f }, RF_LIGHTGRAY); // Draw ground
            rf_draw_cube((rf_vec3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, RF_BLUE);     // Draw a blue wall
            rf_draw_cube((rf_vec3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, RF_LIME);      // Draw a green wall
            rf_draw_cube((rf_vec3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, RF_GOLD);      // Draw a yellow wall

            // Draw some cubes around
            for (int i = 0; i < MAX_COLUMNS; i++)
            {
                rf_draw_cube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                rf_draw_cube_wires(positions[i], 2.0f, heights[i], 2.0f, RF_MAROON);
            }

		rf_end_3d();

        rf_draw_rectangle( 10, 10, 220, 70, rf_fade(RF_SKYBLUE, 0.5f));
        rf_draw_rectangle_outline( (rf_rec){ 10, 10, 220, 70 }, 1, RF_BLUE);

        rf_draw_text("First person camera default controls:", 20, 20, 10, RF_BLACK);
        rf_draw_text("- Move with keys: W, A, S, D", 40, 40, 10, RF_DARKGRAY);
        rf_draw_text("- Mouse move to look around", 40, 60, 10, RF_DARKGRAY);

    rf_end();

	// Need to reset this or else it won't stop zooming
	camera_input_state.mouse_wheel_move = 0;
}

void update_camera_input_bool_states(short keycode, bool state) {
	switch (keycode) {
		case SAPP_KEYCODE_LEFT_ALT:
			camera_input_state.is_camera_alt_control_key_down = state;
			break;
		case SAPP_KEYCODE_LEFT_CONTROL:
			camera_input_state.is_camera_smooth_zoom_control_key = state;
			break;
		case SAPP_KEYCODE_W:
			camera_input_state.direction_keys[0] = state;
			break;
		case SAPP_KEYCODE_S:
			camera_input_state.direction_keys[1] = state;
			break;
		case SAPP_KEYCODE_D:
			camera_input_state.direction_keys[2] = state;
			break;
		case SAPP_KEYCODE_A:
			camera_input_state.direction_keys[3] = state;
			break;
		case SAPP_KEYCODE_E:
			camera_input_state.direction_keys[4] = state;
			break;
		case SAPP_KEYCODE_Q:
			camera_input_state.direction_keys[5] = state;
			break;
		default:
			break;
	}
}

void update_camera_input_states(const sapp_event* event) {
	switch (event->type) {
		case SAPP_EVENTTYPE_MOUSE_MOVE:
			camera_input_state.mouse_position.x += event->mouse_dx;
			camera_input_state.mouse_position.y += event->mouse_dy;
			break;
		case SAPP_EVENTTYPE_MOUSE_SCROLL:
			camera_input_state.mouse_wheel_move = event->scroll_y;
			break;
		case SAPP_EVENTTYPE_MOUSE_DOWN:
			if (event->mouse_button == SAPP_MOUSEBUTTON_MIDDLE)
				camera_input_state.is_camera_pan_control_key_down = true;
			break;
		case SAPP_EVENTTYPE_MOUSE_UP:
			if (event->mouse_button == SAPP_MOUSEBUTTON_MIDDLE)
				camera_input_state.is_camera_pan_control_key_down = false;
			break;
		case SAPP_EVENTTYPE_KEY_DOWN:
			update_camera_input_bool_states(event->key_code, true);
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			update_camera_input_bool_states(event->key_code, false);
			break;
		default:
			break;
	}
}

// Event callback
extern void game_event(const sapp_event* event) {
	update_camera_input_states(event);
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}