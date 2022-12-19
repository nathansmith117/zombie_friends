file(GLOB LEVEL_EDIT_SRC_FILES *.cpp)

add_executable(level_edit ${LEVEL_EDIT_SRC_FILES})
target_include_directories(level_edit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(level_edit PUBLIC zombie_friends fltk fltk_images)
