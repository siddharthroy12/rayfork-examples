#include "../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - keyboard input"
};

// Global vars
rf_vec2 ball_position;
bool on_left, on_right, on_up, on_down = false;

// Initialization
extern void game_init()
{
	ball_position = (rf_vec2){(float) window.width/2, (float) window.height/2};
}

// Main Loop
extern void game_update()
{
	// Update
	if (on_left) ball_position.x -= 2.0f;
	if (on_right) ball_position.x += 2.0f;
	if (on_up) ball_position.y -= 2.0f;
	if (on_down) ball_position.y += 2.0f;

	// Draw
 	rf_begin();
		rf_clear(RF_RAYWHITE);
    	rf_draw_text("move the ball with arrow keys", 10, 10, 20, RF_DARKGRAY);
		rf_draw_circle_v(ball_position, 50, RF_MAROON);
 	rf_end();
}


void update_input_state(short key_code, bool value)
{
	switch (key_code)
	{
		case SAPP_KEYCODE_RIGHT:
			on_right = value; 
			break;
		case SAPP_KEYCODE_LEFT:
			on_left = value;
			break;
		case SAPP_KEYCODE_UP:
			on_up = value;
			break;
		case SAPP_KEYCODE_DOWN:
			on_down = value;
			break;
		default:
			break;
	}
}

// Event callback
extern void game_event(const sapp_event* event)
{
	switch (event->type)
	{
		case SAPP_EVENTTYPE_KEY_DOWN:
			update_input_state(event->key_code, true);
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			update_input_state(event->key_code, false);
			break;
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) 
{
	// Do nothing
}