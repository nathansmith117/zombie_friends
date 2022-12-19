file(GLOB LAUNCHER_DLL_SRC_FILES *.cpp)

add_library(zombie_friends_dll SHARED ${LAUNCHER_DLL_SRC_FILES})
target_include_directories(zombie_friends_dll PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FLTK_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(zombie_friends_dll PUBLIC zombie_friends fltk fltk_images)
