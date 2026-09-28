add_library(gpio STATIC gpio.c)

# PUBLIC: callers of this component include gpio_ifa.h from here.
target_include_directories(gpio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gpio PRIVATE platform project_warnings)
