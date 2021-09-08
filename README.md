## Raylib examples ported to rayfork

I'm porting all the examples from Raylib's website to rayfork using sokol_h backend

> **Note:** The rayfork included in this repo is a patched version by me that fixes some bugs

## Rayfork's patch notes:
- Fixed scissor mode bug

## Dependencies

### Window
	none

### Mac
	Cocoa, QuartzCore, OpenGL

### Linux
	X11, Xi, Xcursor

## Building

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Category: core

- [core_basic_window](./src/examples/core_basic_window.c)
- [core_input_keys](./src/examples/core_input_keys.c)
- [core_input_mouse](./src/examples/core_input_mouse.c)
- [core_input_mouse_wheel](./src/examples/core_input_mouse_wheel.c)
- [core_input_multitouch](./src/examples/core_input_multitouch.c)
- [core_2d_camera](./src/examples/core_2d_camera.c)
- [core_2d_camera_platformer](./src/examples/core/core_2d_camera_platformer.c)
- [core_3d_camera_mode](./src/examples/core/core_3d_camera_mode.c)
- [core_3d_camera_mode_free](./src/examples/core/core_3d_camera_free.c)
- [core_3d_camera_first_person](./src/examples/core/core_3d_camera_first_person.c)
- [core_3d_picking](./src/examples/core/core_3d_picking.c)
- [core_word_screen](./src/examples/core/core_world_screen.c)
- [core_window_letterbox](./src/examples/core/core_window_letterbox.c)
- [core_scissor_test](./src/examples/core/core_scissor_test.c)

Contributions are welcome, feel free to send new examples!