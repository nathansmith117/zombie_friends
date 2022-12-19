file(GLOB CMD_LAUNCHER_SRC_FILES *.cpp)

add_executable(cmd_launcher ${CMD_LAUNCHER_SRC_FILES})
target_include_directories(cmd_launcher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR}/../../)
target_link_libraries(cmd_launcher PUBLIC zombie_friends launcher fltk fltk_images)
