add_library(gpio_adapt STATIC gpio_pin.cpp)

target_include_directories(gpio_adapt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gpio_adapt PRIVATE gpio project_warnings)
