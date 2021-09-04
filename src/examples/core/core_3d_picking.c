#include "../../platform.h"
#include "sokol_app.h"
// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - 3d camera picking"
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
rf_vec3 cube_position = { 0.0f, 1.0f, 0.0f };
rf_vec3 cube_size = { 2.0f, 2.0f, 2.0f };

rf_ray ray = { 0 };
rf_ray_hit_info ray_collision = { 0 };


// Input states for camera state
rf_input_state_for_update_camera camera_input_state = { 0 };

// Action
bool ray_shoot = false;

// Mouse pos
rf_vec2 mouse_pos = { 0 }; 

// FPS
uint64_t frame_start_time;
float delta_time;
char fps_text[10];

// Initialization
extern void game_init() {
	// Setup sokol_time.h
	stm_setup();

	rf_set_camera3d_mode(&camera_state, camera, RF_CAMERA_FREE);
}

// Main Loop
extern void game_update() {
	frame_start_time = stm_now();

	// Update camera
	rf_update_camera3d(&camera, &camera_state, camera_input_state);

	if (ray_shoot) {
        if (!ray_collision.hit) {
			ray = rf_get_mouse_ray((rf_sizei){ sapp_width(), sapp_height() }, mouse_pos, camera);
            // Check collision between ray and box
			ray_collision.hit = rf_check_collision_ray_box(ray, (rf_bounding_box){
				(rf_vec3){ cube_position.x - cube_size.x/2, cube_position.y - cube_size.y/2, cube_position.z - cube_size.z/2 },
				(rf_vec3){ cube_position.x + cube_size.x/2, cube_position.y + cube_size.y/2, cube_position.z + cube_size.z/2 }
			});
        } else {
			ray_collision.hit = false;
		}
		ray_shoot = false;
    }

	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
		rf_begin_3d(camera);
			if (ray_collision.hit) {
                rf_draw_cube(cube_position, cube_size.x, cube_size.y, cube_size.z, RF_RED);
                rf_draw_cube_wires(cube_position, cube_size.x, cube_size.y, cube_size.z, RF_MAROON);

                rf_draw_cube_wires(cube_position, cube_size.x + 0.2f, cube_size.y + 0.2f, cube_size.z + 0.2f, RF_GREEN);
            } else {
                rf_draw_cube(cube_position, cube_size.x, cube_size.y, cube_size.z, RF_GRAY);
                rf_draw_cube_wires(cube_position, cube_size.x, cube_size.y, cube_size.z, RF_DARKGRAY);
            }

            rf_draw_ray(ray, RF_MAROON);
            rf_draw_grid(10, 1.0f);
		rf_end_3d();

		rf_draw_text("Try selecting the box with mouse!", 240, 10, 20, RF_DARKGRAY);
		sprintf(fps_text, "%d FPS", (int)(1/delta_time));
		rf_draw_text(fps_text, 10, 10, 20, RF_LIME);

		if (ray_collision.hit) {
			rf_draw_text("BOX SELECTED",
				(sapp_width() - rf_measure_text(rf_get_default_font(),"BOX SELECTED", 30, 0).width) / 2, (int)(sapp_height() * 0.1f), 30, RF_GREEN
			);
		}
			

    rf_end();

	// Need to reset this or else it won't stop zooming
	camera_input_state.mouse_wheel_move = 0;

	delta_time = stm_sec(stm_since(frame_start_time));
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
		case SAPP_EVENTTYPE_MOUSE_MOVE:
			mouse_pos.x = event->mouse_x;
			mouse_pos.y = event->mouse_y;
			break;
		case SAPP_EVENTTYPE_MOUSE_DOWN:
			switch (event->mouse_button) {
				case SAPP_MOUSEBUTTON_LEFT:
					ray_shoot = true;
					break;
				default:
					break;
			}
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}