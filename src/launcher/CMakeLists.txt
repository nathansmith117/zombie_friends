file(GLOB LAUNCHER_SRC_FILES *.cpp)

# Launcher exe.
add_executable(zombie_friends_launcher ${LAUNCHER_SRC_FILES})
target_include_directories(zombie_friends_launcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(zombie_friends_launcher PUBLIC zombie_friends fltk fltk_images)

# Launcher lib.
add_library(launcher STATIC launcher.cpp)
target_include_directories(launcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(launcher PUBLIC zombie_friends fltk fltk_images)
