add_library(temp_sensor_cpp STATIC TempController.cpp)

target_include_directories(temp_sensor_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(temp_sensor_cpp PRIVATE platform project_warnings)
