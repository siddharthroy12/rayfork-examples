#include "../../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - scissor test"
};

rf_rec scissor_area = { 0, 0, 300, 300 };
bool scissor_mode = true;
bool switch_scissor_mode = false;
bool scissor_mode_switched = false;

rf_vec2 mouse_pos = { 0 };

// Initialization
extern void game_init() {
	// Do nothing
}

// Main Loop
extern void game_update() {
	if (switch_scissor_mode) {
		if (!scissor_mode_switched) {
			scissor_mode = !scissor_mode;
		}
		scissor_mode_switched = true;
	} else {
		scissor_mode_switched = false;
	}

	// Centre the scissor area around the mouse position
    scissor_area.x = mouse_pos.x - scissor_area.width/2;
    scissor_area.y = mouse_pos.y - scissor_area.height/2;

	// Draw
	rf_begin();
		rf_clear(RF_RAYWHITE);

		if (scissor_mode) {
			rf_begin_scissor_mode((int)scissor_area.x, (int)scissor_area.y, (int)scissor_area.width, (int)scissor_area.height);
		}

		// Draw full screen rectangle and some text
        // NOTE: Only part defined by scissor area will be rendered
		rf_draw_rectangle(0, 0, sapp_width(), sapp_height(), RF_RED);
		rf_draw_text("Move the mouse around to reveal this text!", 190, 200, 20, RF_ORANGE);

		if (scissor_mode) {
			rf_end_scissor_mode();
		}

		rf_draw_rectangle_outline(scissor_area, 1, RF_BLACK);
		rf_draw_text("Press S to toggle scissor test", 10, 10, 20, RF_BLACK);
	rf_end();
}

// Event callback
extern void game_event(const sapp_event* event) {
	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN:
			switch (event->key_code) {
				case SAPP_KEYCODE_S:
					switch_scissor_mode = true;
					break;
				default:
					break;
			}
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			switch (event->key_code) {
					case SAPP_KEYCODE_S:
						switch_scissor_mode = false;
						break;
					default:
						break;
				}
			break;
		case SAPP_EVENTTYPE_MOUSE_MOVE:
			mouse_pos.x = event->mouse_x;
			mouse_pos.y = event->mouse_y;
			break;
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}