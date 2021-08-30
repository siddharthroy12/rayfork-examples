#include <stdlib.h>
#include <time.h>
#include "../platform.h"
#include "sokol_app.h"

#define MAX_BUILDINGS   100

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - basic window"
};

// Helper function
int get_random_value(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

// Global vars
rf_rec player = { 400, 280, 40, 40 };
rf_rec buildings[MAX_BUILDINGS] = { 0 };
rf_color build_colors[MAX_BUILDINGS] = { 0 };
int spacing = 0;

rf_camera2d camera = { 0 };

// Controls vars
bool on_left, on_right, on_up, on_down, on_reset = false;


// Initialization
extern void game_init()
{
	camera.target = (rf_vec2){ player.x + 20.0f, player.y + 20.0f };
    camera.offset = (rf_vec2){ sapp_width()/2.0f, sapp_height()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

	// Random should be random
	srand(time(0));

	// Generate buildings
	for (int i = 0; i < MAX_BUILDINGS; i++)
    {
        buildings[i].width = (float)get_random_value(50, 200);
        buildings[i].height = (float)get_random_value(100, 800);
        buildings[i].y = sapp_height() - 130.0f - buildings[i].height;
        buildings[i].x = -6000.0f + spacing;

        spacing += (int)buildings[i].width;

        build_colors[i] = (rf_color){ get_random_value(200, 240), get_random_value(200, 240), get_random_value(200, 250), 255 };
    }
}

// Main Loop
extern void game_update()
{
	// Player movement
    if (on_right) player.x += 2;
    else if (on_left) player.x -= 2;

    // Camera target follows player
    camera.target = (rf_vec2){ player.x + 20, player.y + 20 };

    // Camera rotation controls
    if (on_up) camera.rotation--;
    else if (on_down) camera.rotation++;

    // Limit camera rotation to 80 degrees (-40 to 40)
    if (camera.rotation > 40) camera.rotation = 40;
    else if (camera.rotation < -40) camera.rotation = -40;

    // Camera reset (zoom and rotation)
    if (on_reset)
    {
        camera.zoom = 1.0f;
        camera.rotation = 0.0f;
    }

    rf_begin();

        rf_clear(RF_RAYWHITE);

		rf_begin_2d(camera);

			rf_draw_rectangle(-6000, 320, 13000, 8000, RF_DARKGRAY);

			for (int i = 0; i < MAX_BUILDINGS; i++) rf_draw_rectangle_rec(buildings[i], build_colors[i]);

			rf_draw_rectangle_rec(player, RF_RED);

			rf_draw_line((int)camera.target.x, -sapp_height()*10, (int)camera.target.x, sapp_height()*10, RF_GREEN);
			rf_draw_line(-sapp_width()*10, (int)camera.target.y, sapp_width()*10, (int)camera.target.y, RF_GREEN);

		rf_end_2d();

    	rf_draw_text("SCREEN AREA", 640, 10, 20, RF_RED);

		rf_draw_rectangle(0, 0, sapp_width(), 5, RF_RED);
		rf_draw_rectangle(0, 5, 5, sapp_height() - 10, RF_RED);
		rf_draw_rectangle(sapp_width() - 5, 5, 5, sapp_height() - 10, RF_RED);
		rf_draw_rectangle(0, sapp_height() - 5, sapp_width(), 5, RF_RED);

		rf_draw_rectangle( 10, 10, 250, 113, rf_fade(RF_SKYBLUE, 0.5f));
		rf_draw_rectangle_outline( (rf_rec){10, 10, 250, 113}, 1, RF_BLUE);

		rf_draw_text("Free 2d camera controls:", 20, 20, 10, RF_BLACK);
		rf_draw_text("- Right/Left to move Offset", 40, 40, 10, RF_DARKGRAY);
		rf_draw_text("- Mouse Wheel to Zoom in-out", 40, 60, 10, RF_DARKGRAY);
		rf_draw_text("- A / S to Rotate", 40, 80, 10, RF_DARKGRAY);
		rf_draw_text("- R to reset Zoom and Rotation", 40, 100, 10, RF_DARKGRAY);

    rf_end();
}

void update_control_state(short key_code, bool value)
{
	switch (key_code)
	{
		case SAPP_KEYCODE_RIGHT:
			on_right = value; 
			break;
		case SAPP_KEYCODE_LEFT:
			on_left = value;
			break;
		case SAPP_KEYCODE_A:
			on_up = value;
			break;
		case SAPP_KEYCODE_S:
			on_down = value;
			break;
		case SAPP_KEYCODE_R:
			on_reset = value;
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
			update_control_state(event->key_code, true);
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			update_control_state(event->key_code, false);
			break;
		case SAPP_EVENTTYPE_MOUSE_SCROLL:
			// Camera zoom controls
			camera.zoom += ((float)event->scroll_y*0.05f);
			if (camera.zoom > 3.0f) camera.zoom = 3.0f;
			else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
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