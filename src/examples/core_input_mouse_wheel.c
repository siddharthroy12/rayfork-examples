#include "../platform.h"
#include "sokol_app.h"

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - input mouse wheel"
};

// Global vars
int box_position_y;
int scroll_speed;
char output_text[100];

// Initialization
extern void game_init()
{
	box_position_y = sapp_height() / 2 - 40;
	scroll_speed = 4;
}

// Main Loop
extern void game_update()
{
	// Update output text
	sprintf(output_text, "Box position Y: %03i", box_position_y);
	// Draw
 	rf_begin();
		rf_clear(RF_RAYWHITE);
		// DrawRectangle(screenWidth/2 - 40, boxPositionY, 80, 80, MAROON);
        // DrawText("Use mouse wheel to move the cube up and down!", 10, 10, 20, GRAY);
        // DrawText(TextFormat("Box position Y: %03i", boxPositionY), 10, 40, 20, LIGHTGRAY
		rf_draw_rectangle(sapp_width()/2 - 40, box_position_y, 80, 80, RF_MAROON);
		rf_draw_text("Use mouse wheel to move the cube up and down!", 10, 10, 20, RF_GRAY);
		rf_draw_text(output_text, 10, 40, 20, RF_LIGHTGRAY);
	 rf_end();
}

// Event callback
extern void game_event(const sapp_event* event)
{
	switch (event->type)
	{
		case SAPP_EVENTTYPE_MOUSE_SCROLL:
			box_position_y += (event->scroll_y * scroll_speed);
			break;
	}
}

// On Exit
extern void game_exit(void) 
{
	// Do nothing
}