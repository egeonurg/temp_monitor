add_library(temp_sensor STATIC temp_sensor.c)

target_include_directories(temp_sensor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(temp_sensor PRIVATE platform project_warnings)
