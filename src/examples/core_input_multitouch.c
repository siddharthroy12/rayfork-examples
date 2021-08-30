#include "../platform.h"
#include "sokol_app.h"

#define MAX_TOUCH_POINTS 5

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - mouse input"
};

// Global vars
rf_vec2 ball_position;
rf_color ball_color = RF_BEIGE;

int touch_counter = 0;
rf_vec2 touch_position = { 0.0f };

rf_vec2 touch_positions[MAX_TOUCH_POINTS] = { 0 };

char touch_index_number_text[10];

// Initialization
extern void game_init()
{
	ball_position = (rf_vec2){(float) sapp_width() /2, (float) sapp_height() /2};
}

// Main Loop
extern void game_update()
{
	ball_color = RF_BEIGE;

	if (touch_counter > 0) touch_counter--;

	// Draw
    rf_begin();
        rf_clear(RF_RAYWHITE);
		// Multitouch
        for (int i = 0; i < MAX_TOUCH_POINTS; ++i)
        {
            touch_position = touch_positions[i]; // Get the touch point
            if ((touch_position.x >= 0) && (touch_position.y >= 0)) // Make sure point is not (-1,-1) as this means there is no touch for it
            {
                // Draw circle and touch index number
                rf_draw_circle_v(touch_position, 34, RF_ORANGE);
				sprintf(touch_index_number_text, "%d", i);
                rf_draw_text(touch_index_number_text, (int)touch_position.x - 10, (int)touch_position.y - 70, 40, RF_BLACK);
            }
        }

		// Draw the normal mouse location
		rf_draw_circle_v(ball_position, 30 + (touch_counter*3.0f), ball_color);
        rf_draw_text("move ball with mouse and click mouse button to change color", 10, 10, 20, RF_DARKGRAY);
		rf_draw_text("touch the screen at multiple locations to get multiple balls", 10, 30, 20, RF_DARKGRAY);
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
			break;
		case SAPP_EVENTTYPE_MOUSE_DOWN:
			switch (event->mouse_button)
			{
				case SAPP_MOUSEBUTTON_LEFT:
					ball_color = RF_MAROON;
					touch_counter = 10;
					break;
				case SAPP_MOUSEBUTTON_RIGHT:
					ball_color = RF_DARKBLUE;
					touch_counter = 10;
					break;
				case SAPP_MOUSEBUTTON_MIDDLE:
					ball_color = RF_LIME;
					touch_counter = 10;
					break;
				default:
					break;
			}
		case SAPP_EVENTTYPE_TOUCHES_BEGAN:
		case SAPP_EVENTTYPE_TOUCHES_MOVED:
			for (int i = 0; i < MAX_TOUCH_POINTS; i++)
			{
				touch_positions[i].x = event->touches[i].pos_x;
				touch_positions[i].y = event->touches[i].pos_y;
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