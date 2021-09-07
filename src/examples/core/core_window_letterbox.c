#include "../../platform.h"
#include "sokol_app.h"
#include <time.h>
#include <stdarg.h>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - window scale letterbox"
};

rf_vec2 clamp_value(rf_vec2 value, rf_vec2 min, rf_vec2 max) {
	rf_vec2 result = value;
	result.x = (result.x > max.x)? max.x : result.x;
    result.x = (result.x < min.x)? min.x : result.x;
    result.y = (result.y > max.y)? max.y : result.y;
    result.y = (result.y < min.y)? min.y : result.y;
    return result;
}

const char* text_format(const char* format, ...) {
    // We create an array of buffers so strings don't expire until 4 invocations
    static char buffers[4][100] = { 0 };
    static int index = 0;

    char *currentBuffer = buffers[index];
    memset(currentBuffer, 0, 100);   // Clear buffer before using

    va_list args;
    va_start(args, format);
    vsnprintf(currentBuffer, 100, format, args);
    va_end(args);

    index += 1;     // Move to next buffer for next function call
    if (index >= 4) index = 0;

    return currentBuffer;
}

int get_random_value(int min, int max) {
	return (rand() % (max - min + 1)) + min;
}

int game_screen_width = 640;
int game_screen_height = 480;

rf_render_texture2d target;

rf_color colors[10] = { 0 };

// Actions
bool regen_color = false;
bool did_regen_color = false;

// Mouse

rf_vec2 mouse = { 0 };
rf_vec2 virtual_mouse = { 0 };

// Initialization
extern void game_init() {
	srand(time(0));
	
	// Render texture initialization, used to hold the rendering result so we can easily resize it
	target = rf_load_render_texture(game_screen_width, game_screen_height);
	rf_set_texture_filter(target.texture, RF_FILTER_BILINEAR);

	for (int i; i < 10; i++) {
		colors[i] = (rf_color) { get_random_value(100, 250), get_random_value(50, 150), get_random_value(10, 100), 255 };
	}
}

// Main Loop
extern void game_update() {
	// Compute required framebuffer scaling
    float scale = min((float)sapp_width()/game_screen_width, (float)sapp_height()/game_screen_height);

	if (regen_color) {
		if (!did_regen_color) {
			// Recalculate random colors for the bars
			for (int i = 0; i < 10; i++) {
				colors[i] = (rf_color) { get_random_value(100, 250), get_random_value(50, 150), get_random_value(10, 100), 255 };
			}
		}
		did_regen_color = true;
	} else {
		did_regen_color = false;
	}

	// Update virtual mouse (clamped mouse value behind game screen)
    virtual_mouse.x = (mouse.x - (sapp_width() - (game_screen_width*scale))*0.5f)/scale;
    virtual_mouse.y = (mouse.y - (sapp_height() - (game_screen_height*scale))*0.5f)/scale;
    virtual_mouse = clamp_value(virtual_mouse, (rf_vec2){ 0, 0 }, (rf_vec2){ (float)game_screen_width, (float)game_screen_height });
    
	// Draw everything in the render texture, note this will not be rendered on screen, yet
	rf_begin_render_to_texture(target);
		rf_clear(RF_RAYWHITE);
		for (int i = 0; i < 10; i++) {
			rf_draw_rectangle(0, (game_screen_height/10)*i, game_screen_width, game_screen_height/10, colors[i]);
		}
		rf_draw_text("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, RF_WHITE);
		rf_draw_text(text_format("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, RF_GREEN);
		rf_draw_text(text_format("Virtual Mouse: [%i , %i]", (int)virtual_mouse.x, (int)virtual_mouse.y), 350, 55, 20, RF_YELLOW);

	rf_end_render_to_texture();

	// Draw
    rf_begin();
        rf_clear(RF_BLACK);
		// Draw render texture to screen, properly scaled
		rf_draw_texture_region(
			target.texture,
			(rf_rec){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
			(rf_rec){
				(sapp_width() - ((float)game_screen_width*scale))*0.5f,
				(sapp_height() - ((float)game_screen_height*scale))*0.5f,
				(float)game_screen_width*scale,
				(float)game_screen_height*scale
			},
			(rf_vec2){ 0, 0 },
			0.0f,
			RF_WHITE
		);

    rf_end();
}

// Event callback
extern void game_event(const sapp_event* event) {
	switch (event->type) {
		case SAPP_EVENTTYPE_KEY_DOWN:
			switch (event->key_code) {
				case SAPP_KEYCODE_SPACE:
					regen_color = true;
					break;
				default:
					break;
				}
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			switch (event->key_code) {
				case SAPP_KEYCODE_SPACE:
					regen_color = false;
					break;
				default:
					break;
				}
			break;
		case SAPP_EVENTTYPE_MOUSE_MOVE:
			mouse.x = event->mouse_x;
			mouse.y = event->mouse_y;
			break;
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}