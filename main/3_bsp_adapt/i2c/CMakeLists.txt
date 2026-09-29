add_library(i2c_adapt STATIC i2c.cpp)

target_include_directories(i2c_adapt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(i2c_adapt PRIVATE i2c project_warnings)
