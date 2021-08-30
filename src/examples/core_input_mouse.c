#include "../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - mouse input"
};

// Global vars
rf_vec2 ball_position;
rf_color ball_color;

// Initialization
extern void game_init()
{
	ball_position = (rf_vec2){(float) sapp_width() /2, (float) sapp_height() /2};
}

// Main Loop
extern void game_update()
{
	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
        rf_draw_text("move ball with mouse and click mouse button to change color", 10, 10, 20, RF_DARKGRAY);
		rf_draw_circle_v(ball_position, 50, RF_MAROON);
    rf_end();
}

// Event callback
extern void game_event(const sapp_event* event)
{
	switch (event->type)
	{
		case SAPP_EVENTTYPE_MOUSE_MOVE:
			ball_position.x = event->mouse_x;
			ball_position.y = event->mouse_y;
		case SAPP_EVENTTYPE_MOUSE_DOWN:
			switch (event->mouse_button)
			{
				case SAPP_MOUSEBUTTON_LEFT:
					ball_color = RF_MAROON;
					break;
				case SAPP_MOUSEBUTTON_RIGHT:
					ball_color = RF_DARKBLUE;
					break;
				case SAPP_MOUSEBUTTON_MIDDLE:
					ball_color = RF_LIME;
					break;
				default:
					break;
			}
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) 
{
	// Do nothing
}