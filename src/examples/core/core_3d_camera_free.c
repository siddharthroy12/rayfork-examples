#include "../../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "raylib [core] example - 3d camera free"
};

// Global vars

rf_camera3d camera = {
	.position = (rf_vec3){ 10.0f, 10.0f, 10.0f },
	.target = (rf_vec3){ 0.0f, 0.0f, 0.0f },
	.up = (rf_vec3){ 0.0f, 1.0f, 0.0f },
	.fovy = 45.0f,
	.type = RF_CAMERA_PERSPECTIVE,
};

rf_camera3d_state camera_state = { 0 };
rf_vec3 cube_position = { 0.0f, 0.0f, 0.0f };


// Input states for camera state
rf_input_state_for_update_camera camera_input_state = { 0 };

// General input state
bool action_reset;


// Initialization
extern void game_init() {
	rf_set_camera3d_mode(&camera_state, camera, RF_CAMERA_FREE);
}

// Main Loop
extern void game_update() {
	if (action_reset) {
		camera.target = (rf_vec3){ 0.0f, 0.0f, 0.0f };
	}

	rf_update_camera3d(&camera, &camera_state, camera_input_state);

	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
		rf_begin_3d(camera);
			rf_draw_cube(cube_position, 2.0f, 2.0f, 2.0f, RF_RED);
			rf_draw_cube_wires(cube_position, 2.0f, 2.0f, 2.0f, RF_MAROON);
			rf_draw_grid(10, 1.0f);
		rf_end_3d();
    	rf_draw_rectangle( 10, 10, 320, 133, rf_fade(RF_SKYBLUE, 0.5f));
        rf_draw_rectangle_outline( (rf_rec){ 10, 10 , 320, 133 }, 1, RF_BLUE);

        rf_draw_text("Free camera default controls:", 20, 20, 10, RF_BLACK);
        rf_draw_text("- Mouse Wheel to Zoom in-out", 40, 40, 10, RF_DARKGRAY);
        rf_draw_text("- Mouse Wheel Pressed to Pan", 40, 60, 10, RF_DARKGRAY);
        rf_draw_text("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, RF_DARKGRAY);
        rf_draw_text("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, RF_DARKGRAY);
        rf_draw_text("- Z to zoom to (0, 0, 0)", 40, 120, 10, RF_DARKGRAY);
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
			camera_input_state.mouse_position.x = event->mouse_x;
			camera_input_state.mouse_position.y = event->mouse_y;
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

	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN:
			if (event->key_code == SAPP_KEYCODE_Z) {
				action_reset = true;
			}
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			if (event->key_code == SAPP_KEYCODE_Z) {
				action_reset = false;
			}
			break;
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}