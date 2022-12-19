cmake_minimum_required(VERSION 3.25.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(zombie_friends VERSION 1.0)

# change this to your fltk build directory
set(FLTK_DIR C:\\msys64\\home\\nathan\\fltk-1.3.8\\build)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find packages.
find_package(FLTK REQUIRED NO_MODULE)

# Find files.
file(GLOB SRC_FILES src/*.cpp)

# Create library.
add_library(zombie_friends STATIC ${SRC_FILES})
target_include_directories(zombie_friends PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR})
target_link_libraries(zombie_friends PUBLIC fltk fltk_images)

add_subdirectory(src/launcher_dlls)
add_subdirectory(src/launcher)
add_subdirectory(src/launcher/cmd_launcher)
add_subdirectory(src/level_edit)
