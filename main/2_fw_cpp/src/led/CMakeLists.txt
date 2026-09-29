add_library(led_cpp STATIC LedController.cpp)

target_include_directories(led_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(led_cpp PRIVATE gpio_adapt project_warnings)
