cmake_minimum_required(VERSION 3.1)
project(your-project-name-goes-here LANGUAGES C) # Change the name to your project here

# Use c99
set(CMAKE_C_STANDARD 99)

# Setup GLAD
add_library(glad libs/glad/glad.c)
target_include_directories(glad PUBLIC libs)

# Setup rayfork
add_library(rayfork libs/rayfork/rayfork.c)
target_include_directories(rayfork PUBLIC libs)

# Loop through every .c file in examples dir recursively and build the executable
file(GLOB_RECURSE APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/examples/*.c)
foreach(testsourcefile ${APP_SOURCES})
	# Get relative path
	get_filename_component(testpath ${testsourcefile} DIRECTORY)
	# Get name without extension
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile} src/platform.c)

    # Make sure YourLib is linked to each app
    target_link_libraries(${testname} PRIVATE glad rayfork)
    target_include_directories(${testname} PRIVATE libs/rayfork libs/sokol)
    target_compile_definitions(${testname} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

    # Enable MacOS specific flags and link against it's specific libraries
    if (APPLE)
        target_link_libraries(${testname} PUBLIC "-framework Cocoa" "-framework QuartzCore" "-framework OpenGL")
        target_compile_options(${testname} PUBLIC "-fobjc-arc" "-ObjC")
    endif()

    # Enable Linux specific flags and link against it's specific libraries
    if (UNIX)
        target_link_libraries(${testname} PUBLIC m dl pthread X11 Xi Xcursor)
    endif()

	# Modify its output paths. 
	string(REPLACE "src/examples/" "" finalpath ${testpath})
	set_target_properties(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${finalpath})
endforeach( testsourcefile ${APP_SOURCES} )