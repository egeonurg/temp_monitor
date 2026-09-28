add_library(adc STATIC adc.c)

target_include_directories(adc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(adc PRIVATE platform project_warnings)
