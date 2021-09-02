#include "../../platform.h"
#include "sokol_app.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

// Window config
platform_window_details window = {
    .width  = 800,
    .height = 450,
    .title  = "rayfork [core] example - 2d camera platformer"
};

typedef struct player {
    rf_vec2 position;
    float speed;
    bool can_jump;
} player;

typedef struct env_item {
    rf_rec rect;
    int blocking;
    rf_color color;
} env_item;

void update_player(player *player_ptr, env_item *env_items, int env_items_length, float delta);

void update_camera_center(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height);
void update_camera_center_inside_map(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height);
void update_camera_center_smooth_follow(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height);
void update_camera_even_out_on_landing(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height);
void update_camera_player_bounds_push(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height);

// Global vars
player main_player = { 
	.position = (rf_vec2){ 400, 280 },
	.speed = 0,
	.can_jump = false
};

env_item env_items[] = {
    {{ 0, 0, 1000, 400 }, 0, RF_LIGHTGRAY },
    {{ 0, 400, 1000, 200 }, 1, RF_GRAY },
    {{ 300, 200, 400, 10 }, 1, RF_GRAY },
    {{ 250, 300, 100, 10 }, 1, RF_GRAY },
    {{ 650, 300, 100, 10 }, 1, RF_GRAY }
};

int env_items_length = sizeof(env_items)/sizeof(env_items[0]);

rf_camera2d camera = {
	.rotation = 0.0f,
	.zoom = 1.0f
};

// Store pointers to the multiple update camera functions
void (*camera_updaters[])(rf_camera2d*, player*, env_item*, int, float, int, int) = {
    update_camera_center,
    update_camera_center_inside_map,
    update_camera_center_smooth_follow,
    update_camera_even_out_on_landing,
    update_camera_player_bounds_push
};

int camera_option = 0;
int camera_updaters_length = sizeof(camera_updaters)/sizeof(camera_updaters[0]);

char *camera_descriptions[] = {
    "Follow player center",
    "Follow player center, but clamp to map edges",
    "Follow player center; smoothed",
    "Follow player center horizontally; updateplayer center vertically after landing",
    "Player push camera on getting too close to screen edge"
};

double delta_time;
uint64_t frame_start_time;

// Actions vars
bool reset, change_camera, camera_changed, move_left, move_right, jump = false;

// Initialization
extern void game_init(void) {
	camera.target = main_player.position,
	camera.offset = (rf_vec2){ sapp_width()/2.0f, sapp_height()/2.0f };

	// Initialize timer
	stm_setup();
}

// Main Loop
extern void game_update(void) {
	frame_start_time = stm_now();

	// Update
	update_player(&main_player, env_items, env_items_length, delta_time);
	
	if (camera.zoom > 3.0f) camera.zoom = 3.0f;
    else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

	if (reset) {
		camera.zoom = 1.0f;
		main_player.position = (rf_vec2){ 400, 280 }; 
	}

	if (change_camera) {
		if (!camera_changed) {
			camera_option = (camera_option + 1) % camera_updaters_length;
		}
		camera_changed = true;
	} else {
		camera_changed = false;
	}

	// Call update camera function by its pointer
	camera_updaters[camera_option](&camera, &main_player, env_items, env_items_length, delta_time, sapp_width(), sapp_height());

	// Draw
    rf_begin();
        rf_clear(RF_LIGHTGRAY);

		rf_begin_2d(camera);
			for (int i = 0; i < env_items_length; i++) rf_draw_rectangle_rec(env_items[i].rect, env_items[i].color);

			rf_rec player_rect = { main_player.position.x - 20, main_player.position.y - 40, 40, 40 };
            rf_draw_rectangle_rec(player_rect, RF_RED);
		rf_end_2d();
    	
		rf_draw_text("Controls:", 20, 20, 10, RF_BLACK);
        rf_draw_text("- Right/Left to move", 40, 40, 10, RF_DARKGRAY);
        rf_draw_text("- Space to jump", 40, 60, 10, RF_DARKGRAY);
        rf_draw_text("- Mouse Wheel to Zoom in-out, R to reset zoom", 40, 80, 10, RF_DARKGRAY);
        rf_draw_text("- C to change camera mode", 40, 100, 10, RF_DARKGRAY);
        rf_draw_text("Current camera mode:", 20, 120, 10, RF_BLACK);
        rf_draw_text(camera_descriptions[camera_option], 40, 140, 10, RF_DARKGRAY);
    rf_end();

	delta_time = stm_sec(stm_since(frame_start_time));
}

void update_actions(const short keycode, bool value) {
	switch (keycode) {
		case SAPP_KEYCODE_R:
			reset = value;
			break;
		case SAPP_KEYCODE_C:
			change_camera = value;
			break;
		case SAPP_KEYCODE_LEFT:
			move_left = value;
			break;
		case SAPP_KEYCODE_RIGHT:
			move_right = value;
			break;
		case SAPP_KEYCODE_SPACE:
			jump = value;
			break;
		default:
			break;
	}
}

// Event callback
extern void game_event(const sapp_event* event) {
	switch (event->type) {
		case SAPP_EVENTTYPE_MOUSE_SCROLL:
			camera.zoom = event->scroll_y;
			break;
		case SAPP_EVENTTYPE_KEY_DOWN:
			update_actions(event->key_code, true);
			break;
		case SAPP_EVENTTYPE_KEY_UP:
			update_actions(event->key_code, false);
			break;
		default:
			break;
	}
}

// On Exit
extern void game_exit(void) {
	// Do nothing
}

void update_player(player *player_ptr, env_item *env_items, int env_items_length, float delta) {
	if (move_left) player_ptr->position.x -= PLAYER_HOR_SPD*delta;
    if (move_right) player_ptr->position.x += PLAYER_HOR_SPD*delta;
    if (jump && player_ptr->can_jump) {
        player_ptr->speed = -PLAYER_JUMP_SPD;
    	player_ptr->can_jump = false;
    }

    int hit_obstacle = 0;
    for (int i = 0; i < env_items_length; i++) {
        env_item *ei = env_items + i;
        rf_vec2 *p = &(player_ptr->position);
        if (
			ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y < p->y + player_ptr->speed*delta
		) {
            hit_obstacle = 1;
            player_ptr->speed = 0.0f;
            p->y = ei->rect.y;
        }
    }

    if (!hit_obstacle) {
        player_ptr->position.y += player_ptr->speed*delta;
        player_ptr->speed += G*delta;
        player_ptr->can_jump = false;
    }
    else player_ptr->can_jump = true;
}

void update_camera_center(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height) {
	camera->offset = (rf_vec2){ width/2.0f, height/2.0f };
    camera->target = player_ptr->position;
}

void update_camera_center_inside_map(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height) {
	camera->target = player_ptr->position;
    camera->offset = (rf_vec2){ width/2.0f, height/2.0f };
    float min_x = 1000, min_y = 1000, max_x = -1000, max_y = -1000;

    for (int i = 0; i < env_items_length; i++) {
        env_item *ei = env_items + i;
        min_x = fminf(ei->rect.x, min_x);
        max_x = fmaxf(ei->rect.x + ei->rect.width, max_x);
        min_y = fminf(ei->rect.y, min_y);
        max_y = fmaxf(ei->rect.y + ei->rect.height, max_y);
    }

    rf_vec2 max = rf_get_world_to_screen2d((rf_vec2){ max_x, max_y }, *camera);
    rf_vec2 min = rf_get_world_to_screen2d((rf_vec2){ min_x, min_y }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void update_camera_center_smooth_follow(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height) {
	static float min_speed = 30;
    static float min_effect_length = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (rf_vec2){ width/2.0f, height/2.0f };

    rf_vec2 diff = rf_vec2_sub(player_ptr->position, camera->target);
    float length = rf_vec2_len(diff);

    if (length > min_effect_length) {
        float speed = fmaxf(fractionSpeed*length, min_speed);
        camera->target = rf_vec2_add(camera->target, rf_vec2_scale(diff, speed*delta/length));
    }
}

void update_camera_even_out_on_landing(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height) {
	static float even_out_speed = 700;
    static int evening_out = false;
    static float even_out_target;

    camera->offset = (rf_vec2){ width/2.0f, height/2.0f };
    camera->target.x = player_ptr->position.x;

    if (evening_out) {
        if (even_out_target > camera->target.y) {
            camera->target.y += even_out_speed*delta;

            if (camera->target.y > even_out_target) {
                camera->target.y = even_out_target;
                evening_out = 0;
            }
        } else {
            camera->target.y -= even_out_speed*delta;

            if (camera->target.y < even_out_target) {
                camera->target.y = even_out_target;
                evening_out = 0;
            }
        }
    } else {
        if (player_ptr->can_jump && (player_ptr->speed == 0) && (player_ptr->position.y != camera->target.y)) {
            evening_out = 1;
            even_out_target = player_ptr->position.y;
        }
    }
}
void update_camera_player_bounds_push(rf_camera2d *camera, player *player_ptr, env_item *env_items, int env_items_length, float delta, int width, int height) {
	static rf_vec2 bbox = { 0.2f, 0.2f };

    rf_vec2 bbox_world_min = rf_get_screen_to_world2d((rf_vec2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
    rf_vec2 bbox_world_max = rf_get_screen_to_world2d((rf_vec2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
    camera->offset = (rf_vec2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

    if (player_ptr->position.x < bbox_world_min.x) camera->target.x = player_ptr->position.x;
    if (player_ptr->position.y < bbox_world_min.y) camera->target.y = player_ptr->position.y;
    if (player_ptr->position.x > bbox_world_max.x) camera->target.x = bbox_world_min.x + (player_ptr->position.x - bbox_world_max.x);
    if (player_ptr->position.y > bbox_world_max.y) camera->target.y = bbox_world_min.y + (player_ptr->position.y - bbox_world_max.y);
}