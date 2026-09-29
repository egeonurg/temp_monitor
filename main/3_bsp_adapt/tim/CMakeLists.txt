add_library(tim_adapt STATIC timer.cpp tick_timer.cpp)

target_include_directories(tim_adapt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tim_adapt PRIVATE tim project_warnings)
